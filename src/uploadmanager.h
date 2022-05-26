#pragma once

#include <QObject>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QFile>
#include <QHttpMultiPart>

// https://github.com/rivmar/Qt-5-ftp-upload/blob/master/ftp.h
// https://wiki.qt.io/WIP-How_to_create_a_simple_chat_application
// https://github.com/rivmar/Qt-5-ftp-upload

// https://code.google.com/archive/p/datacod-qt-tools/downloads

enum class RequestType
{
    none = 0x00,
    csv,
    binary,
    auth
};

class UploadManager : public QObject
{
    Q_OBJECT
public :
    UploadManager() = delete;
    UploadManager(QObject *parent = nullptr) = delete;
    explicit UploadManager(QString url, QString fileAddress, QString fileName, QObject* parent = nullptr);

    QNetworkReply* RequestLoginToken();

    void ChangeUploadAddress();
    void ChangeUploadAddress(QString address);
    void UploadFileToAddress(QString file, QString address);
    QNetworkReply* post(QString fileAddress, QString fileName);

    QHttpPart createPart(const QString &header, const QString &body);

    QByteArray GetUserLogin();
    QByteArray GetUserTokenLogin();

private slots:
    void replyFinished(QNetworkReply* reply);

private :
    bool m_login = false;

    QNetworkReply* reply;

    QString m_serverAddress;
    QString m_loginToken;

    QNetworkAccessManager* m_manager;

    RequestType m_request;
};
