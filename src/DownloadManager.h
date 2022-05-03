#pragma once

#include <QUrl>
#include <QtCore>
#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "textprogressbar.h"

//static const QString link = "https://code.qt.io/cgit/qt/qtbase.git/commit/examples/network/downloadmanager?h=5.15";

class DownloadManager: public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = nullptr);

    void append(const QUrl &url);
    void append(const QStringList &urls);
    static QString saveFileName(const QUrl &url);

signals:
    void finished();

private slots:
    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

private:
    bool isHttpRedirect() const;
    void reportRedirect();

    QNetworkAccessManager m_manager;
    QQueue<QUrl> m_downloadQueue;
    QNetworkReply* m_currentDownload = nullptr;
    QFile m_output;
    QElapsedTimer m_downloadTimer;
    TextProgressBar m_progressBar;

    int m_downloadedCount = 0;
    int m_totalCount = 0;
};
