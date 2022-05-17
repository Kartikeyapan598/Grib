#include "DownloadManager.h"
#include "Utils.h"

#include <QTextStream>
#include <cstdio>

using namespace std;

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent)
{
}

void DownloadManager::append(const QStringList &urls)
{
    for (const QString &urlAsString : urls)
    {
        append(QUrl::fromEncoded(urlAsString.toLocal8Bit()));
    }

    if (m_downloadQueue.isEmpty())
    {
        QTimer::singleShot(0, this, &DownloadManager::finished);
    }
}

void DownloadManager::append(const QUrl &url)
{
    if (m_downloadQueue.isEmpty())
    {
        QTimer::singleShot(0, this, &DownloadManager::startNextDownload);
    }
    m_downloadQueue.enqueue(url);
    ++m_totalCount;
}

QString DownloadManager::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
    {
        basename = "download";
    }

    if (QFile::exists(basename))
    {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
        {
            ++i;
        }
        basename += QString::number(i);
    }
    return basename;
}

void DownloadManager::startNextDownload()
{
    if (m_downloadQueue.isEmpty())
    {
        CreateMessageBoxInfo(NULL, "Files Downloaded Successfylly", "Files Downloaded - " + QString::number(m_downloadedCount) + " " + QString::number(m_totalCount));
        //printf("%d/%d files downloaded successfully\n", m_downloadedCount, m_totalCount);
        emit finished();
        return;
    }

    QUrl url = m_downloadQueue.dequeue();

    QString filename = saveFileName(url);
    m_output.setFileName(filename);
    if (!m_output.open(QIODevice::WriteOnly))
    {
        CreateMessageBoxCrit(NULL, "Download Failed", "Failed to download file - " + filename + '\n' + url.toEncoded().constData() + '\n' + "Error - " + m_output.errorString());
        //fprintf(stderr, "Problem opening save file '%s' for download '%s': %s\n", qPrintable(filename), url.toEncoded().constData(), qPrintable(m_output.errorString()));
        startNextDownload();
        return;                 // skip this download
    }

    QNetworkRequest request(url);
    m_currentDownload = m_manager.get(request);
    connect(m_currentDownload, &QNetworkReply::downloadProgress, this, &DownloadManager::downloadProgress);
    connect(m_currentDownload, &QNetworkReply::finished, this, &DownloadManager::downloadFinished);
    connect(m_currentDownload, &QNetworkReply::readyRead, this, &DownloadManager::downloadReadyRead);

    // prepare the output
    printf("Downloading %s...\n", url.toEncoded().constData());
    m_downloadTimer.start();
}

void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_progressBar.setStatus(bytesReceived, bytesTotal);

    // calculate the download speed
    double speed = bytesReceived * 1000.0 / m_downloadTimer.elapsed();
    QString unit;
    if (speed < 1024)
    {
        unit = "bytes/sec";
    }
    else if (speed < 1024*1024)
    {
        speed /= 1024;
        unit = "kB/s";
    }
    else
    {
        speed /= 1024*1024;
        unit = "MB/s";
    }

    m_progressBar.setMessage(QString::fromLatin1("%1 %2").arg(speed, 3, 'f', 1).arg(unit));
    m_progressBar.update();
}

void DownloadManager::downloadFinished()
{
    m_progressBar.clear();
    m_output.close();

    if (m_currentDownload->error())
    {
        // download failed
        CreateMessageBoxCrit(NULL, "Failed to download File", m_currentDownload->errorString());
        //fprintf(stderr, "Failed: %s\n", qPrintable(m_currentDownload->errorString()));
        m_output.remove();
    }
    else
    {
        // let's check if it was actually a redirect
        if (isHttpRedirect())
        {
            reportRedirect();
            m_output.remove();
        }
        else
        {
            printf("Succeeded.\n");
            ++m_downloadedCount;
        }
    }
    m_currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    m_output.write(m_currentDownload->readAll());
}

bool DownloadManager::isHttpRedirect() const
{
    int statusCode = m_currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303 || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::reportRedirect()
{
    int statusCode = m_currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QUrl requestUrl = m_currentDownload->request().url();
    QTextStream(stderr) << "Request: " << requestUrl.toDisplayString() << " was redirected with code: " << statusCode << '\n';

    QVariant target = m_currentDownload->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (!target.isValid())
    {
        return;
    }
    QUrl redirectUrl = target.toUrl();
    if (redirectUrl.isRelative())
        redirectUrl = requestUrl.resolved(redirectUrl);
    QTextStream(stderr) << "Redirected to: " << redirectUrl.toDisplayString()
                        << '\n';
}
