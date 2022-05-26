#include "auth.h"
#include "ui_auth.h"
#include "Utils.h"

Auth::Auth(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Auth)
{
    ui->setupUi(this);

//    connect(&m_manager, &QNetworkAccessManager::authenticationRequired, this, &Auth::authenticationRequired);

}

Auth::~Auth()
{
    delete ui;
}

void Auth::on_AuthLoginButton_clicked()
{
    QString username = ui->lineEdit_Userid->text();
    QString password = ui->lineEdit_Pass->text();



    if(1/*username && password*/)
    {
        CreateMessageBoxInfo(NULL, "Login Info", "Login Successful");
    }
    else
    {
        CreateMessageBoxCrit(NULL, "Login Error", "Login Failed");
    }

}

void Auth::readyRead()
{

}

void Auth::authenticationRequired(QNetworkReply *reply, QAuthenticator *author)
{

}

void Auth::encrypted(QNetworkReply *reply)
{

}

void Auth::finished(QNetworkReply *reply)
{

}

void Auth::networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible)
{

}

void Auth::preSharedKeyAythenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *author)
{

}

void Auth::sslErrors(QNetworkReply *reply, const QList<QSslError> &errs)
{

}

