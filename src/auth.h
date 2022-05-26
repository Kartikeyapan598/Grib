#pragma once

#include <QWidget>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

namespace Ui
{
    class Auth;
}

// https://www.youtube.com/watch?v=G06jT3X3H9E
// https://www.youtube.com/watch?v=6_elY8O20I8

class Auth : public QWidget
{
    Q_OBJECT

public:
    explicit Auth(QWidget *parent = nullptr);
    ~Auth();

    void ChangeApiCall(QString& api) { m_apiCall = api; }

private slots:
    void on_AuthLoginButton_clicked();

    void readyRead();
    void authenticationRequired(QNetworkReply* reply, QAuthenticator* author);
    void encrypted(QNetworkReply* reply);
    void finished(QNetworkReply* reply);
    void networkAccessibleChanged(QNetworkAccessManager::Accessible);
    void preSharedKeyAythenticationRequired(QNetworkReply* reply, QSslPreSharedKeyAuthenticator* author);
    void sslErrors(QNetworkReply* reply, const QList<QSslError>& errs);


public slots:
    //void get(QString location);
    //void post(QString location, QByteArray data);

private:
    Ui::Auth *ui;
    QString m_apiCall;
    bool login = false;
};

