#include "uploadmanager.h"
#include "Utils.h"

#include <QHttpMultiPart>
#include <QInputDialog>
#include <QNetworkCookie>
#include <QNetworkProxy>
#include <QNetworkCookieJar>
#include <QUrl>

// homeAddress/algorithm/csv
// homeAddress/algorithm/binary


// https://stackoverflow.com/questions/38179706/uploading-a-file-in-multipart-form-data-in-qt5
// https://stackoverflow.com/questions/16877363/file-upload-error-with-qnetworkaccessmanager?rq=1
// https://appfluence.com/productivity/how-to-authenticate-qt-google-sso/

// 200, 404


// https://cpp.hotexamples.com/examples/-/QHttpPart/-/cpp-qhttppart-class-examples.html
// https://github.com/samizzo/canon-tweet/blob/master/src/yfrogUpload.cpp
// https://cpp.hotexamples.com/examples/-/QHttpMultiPart/-/cpp-qhttpmultipart-class-examples.html
// https://stackoverflow.com/questions/37067014/qt-upload-put-a-file

/*

Reply for All files
{
  "Inputcsv_url": "media/Input_csv/Santa%20Marta-Moin.csv",
  "NAME": "Santa Marta-Moin.csv",
  "Outputcsv_url": "media/Output_csv/Santa Marta-Moin_optimised.csv"
}

*/

static const QString tokenName = "X-CSRFToken";

UploadManager::UploadManager(QString url, QString fileAddress, QString fileName, QObject* parent)
    :QObject(parent), m_serverAddress(url)
{

    // Code to stay in constructor


    m_manager = new QNetworkAccessManager();


    //

    // Code to move to post function

//    QFile file(fileName);
//    file.open(QIODevice::ReadOnly);
//    QHttpPart imagePart;
//    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, "mutl");
//    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
//    imagePart.setBody(file.readAll());

    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(replyFinished()), this, SLOT(replyFinished()));
}

QByteArray UploadManager::GetUserLogin()
{
    if(!m_login)
    {
        bool ok;
        QString UserId = QInputDialog::getText(NULL, tr("QInputDialog::getText()"), tr("User name:"), QLineEdit::Normal, "UserID", &ok);
        if(ok && UserId.isEmpty())
        {
            CreateMessageBoxCrit(NULL, "Id Box Empty", "No Id Entered!");
            return NULL;
        }
        QString Password = QInputDialog::getText(NULL, tr("QInputDialog::getText()"), tr("User name:"), QLineEdit::Normal,"PassWord", &ok);
        if(ok && Password.isEmpty())
        {
            CreateMessageBoxCrit(NULL, "Password Box Empty", "No Password Entered!");
            return NULL;
        }
        QString part = UserId + '&' + Password;
        return QByteArray(part.toStdString().c_str());
    }
    else
    {
        return GetUserTokenLogin();
    }
}

QHttpPart UploadManager::createPart(const QString &header, const QString &body)
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(header));
    part.setBody(body.toUtf8());
    return part;
}

QByteArray UploadManager::GetUserTokenLogin()
{
    return QByteArray(m_loginToken.toStdString().c_str());
}

void UploadManager::ChangeUploadAddress()
{
    bool ok;
    QString text = QInputDialog::getText(NULL, tr("QInputDialog::getText()"), tr("User name:"), QLineEdit::Normal,"Server Address", &ok);
    if(!ok)
    {
        CreateMessageBoxWarn(NULL, "Empty Box", "Server Address Not given");
    }
    else if(ok && !text.isEmpty())
    {
        m_serverAddress = text;
    }
}

QNetworkReply* UploadManager::RequestLoginToken()
{
    if(m_serverAddress.size() == 0)
    {
        ChangeUploadAddress();
    }

    QNetworkRequest request(m_serverAddress);

    QHttpMultiPart multi;

    QHttpPart login;
    login.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("Authorization"));
    login.setBody(GetUserLogin());

    multi.append(login);
    QNetworkReply *reply = m_manager->post(request, &multi);

    QString buffer = reply->readAll();

    m_login = true;
    return reply;
}

QNetworkReply* UploadManager::post(QString fileAddress, QString fileName)
{
    if(m_login)
    {
        QHttpMultiPart* multi = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        QHttpPart keyPart;
        keyPart.setHeader(QNetworkRequest::ContentDispositionHeader, "Authorization");
        keyPart.setBody(GetUserTokenLogin());

        QHttpPart enctype;
        enctype.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("enctype"));
        enctype.setBody("multipat/form-data");

        QHttpPart fileForm;
        QFile file(fileName);
        if (!file.exists())
        {
            CreateMessageBoxCrit(NULL, "File Error", "File Does Not Exist\n Error");
            delete multi;
            return NULL;
        }
        file.open(QIODevice::ReadOnly);
        fileForm.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("file"));
        fileForm.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("File"));   // Type of File sending to server eg - csv etc
        fileForm.setHeader(QNetworkRequest::ContentLengthHeader, file.size());
        fileForm.setBodyDevice(&file);  // fileForm.setBody(file.readAll());

        multi->append(keyPart);
        multi->append(enctype);
        multi->append(fileForm);

        if(m_serverAddress.isEmpty() == true)
        {
            ChangeUploadAddress();
        }

        QNetworkRequest request((QUrl(m_serverAddress)));

        QNetworkReply* reply = m_manager->post(request, multi);
        multi->setParent(reply);
    }
    else
    {
        // Make so that the file is uploaded after login
        return RequestLoginToken();
    }
}

void UploadManager::ChangeUploadAddress(QString address)
{
    m_serverAddress = address;
}

void UploadManager::UploadFileToAddress(QString file, QString address)
{
}

void UploadManager::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        CreateMessageBoxCrit(NULL, "Error While Downloading", reply->errorString());
        return;
    }

//    //Cookies//
//    QUrl url(m_serverAddress);
//    QList<QNetworkCookie>  cookies = m_manager->cookieJar()->cookiesForUrl(url);
//    qDebug() << "COOKIES for" << url.host() << cookies;

//    QString str;
//    QDebug dStream(&str);

//    dStream << url.host() << cookies;

//    //End Cookies//


//    int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//    if (v >= 200 && v < 300) // Success
//    {
//        getFile(reply);
//         // Here we got the final reply
//       return;
//    }
//    else if (v >= 300 && v < 400) // Redirection
//    {
//        /* Use Cookies for Login */
//        qDebug() << "REDIRECTING";
//        QUrl rUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

//        if(rUrl != url)
//        {
//            m_manager->post(QNetworkRequest(rUrl), login);
//            return;
//        }

//        QDebug() << QString("redirected: " + rUrl.toEncoded()) << endl;

//        QNetworkRequest r(m_url);
//        QVariant var;
//        var.setValue(cookies);

//        r.setHeader(QNetworkRequest::CookieHeader, var);
//        m_manager->get(r);
//        return;
//    }
}
