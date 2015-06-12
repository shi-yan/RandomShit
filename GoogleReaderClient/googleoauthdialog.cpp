#include "googleoauthdialog.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>
#include <QDebug>
#include "json/parser.h"
#include <QTime>
#include <QDate>
#include "json/serializer.h"
#include <QFile>
#include <QVariantMap>



GoogleOauthDialog::GoogleOauthDialog(QWidget *parent):QDialog(parent)
{
    setObjectName(QString::fromUtf8("GoogleOauthDialog"));
    setFixedSize(400,600);
    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    webView = new QWebView(this);
    webView->setObjectName(QString::fromUtf8("webView"));
    webView->setUrl(QUrl("about:blank"));
    horizontalLayout->addWidget(webView);
    QMetaObject::connectSlotsByName(this);


    connect(webView,SIGNAL(loadFinished(bool)),this,SLOT(onLoadFinished(bool)));
    connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(readyRead(QNetworkReply*)));
}

GoogleOauthDialog::~GoogleOauthDialog()
{

}

void GoogleOauthDialog::showEvent ( QShowEvent * event )
{
    qDebug()<<"fuck";
    webView->load(QUrl("https://accounts.google.com/o/oauth2/auth?response_type=code&scope=http://www.google.com/reader/api%20http://www.google.com/reader/atom&redirect_uri=urn:ietf:wg:oauth:2.0:oob&client_id=461842547323.apps.googleusercontent.com"));
}

void GoogleOauthDialog::onLoadFinished(bool isFinished)
{
    qDebug()<<"finished";
    if(isFinished)
    {

        qDebug()<<webView->title();

       if(webView->title().startsWith("Success code="))
       {
           applicationAccessToken=webView->title();
           webView->setHtml("");
           applicationAccessToken=applicationAccessToken.right(applicationAccessToken.length()-13);
           qDebug()<<"access token"<<applicationAccessToken;
           QString re=QString("client_id=461842547323.apps.googleusercontent.com&client_secret=Jvdw3JMCQcrkhJ0wg7qAp5cf&code=%1&redirect_uri=urn:ietf:wg:oauth:2.0:oob&grant_type=authorization_code").arg(applicationAccessToken);
           QNetworkRequest request(QUrl("https://accounts.google.com/o/oauth2/token"));
           QByteArray dataToSend;
           dataToSend.append(re);
           manager.post(request,dataToSend);
       }
     //  else
       //    qDebug()<<webView->
    }
}

void GoogleOauthDialog::readyRead(QNetworkReply *reply)
{
    //show();
    QString text = reply->readAll();
    qDebug("reply:%s,%s",reply->errorString().toStdString().c_str(),text.toStdString().c_str());
    bool ok=false;
    QJson::Parser parser;
    QByteArray json ;
    json.append(text);
    QVariantMap result = parser.parse (json, &ok).toMap();

    if (!ok) {
        qDebug("An error occurred during parsing");
        return;
    }

    qDebug() << "access_token:" << (tempToken=result["access_token"].toString());
    qDebug() << "token_type:" << result["token_type"].toString();
    qDebug() << "expires_in:" << result["expires_in"].toString();
    qDebug() << "refresh_token:" << result["refresh_token"].toString();

    if(!result["refresh_token"].toString().isNull() && !result["refresh_token"].toString().isEmpty())
        refreshToken=result["refresh_token"].toString();

    saveIdentity();

    qDebug()<<QString("https://www.google.com/reader/api/0/user-info?oauth_token=%1").arg(GoogleOauthDialog::getSingleton().getTempToken());

   // webView->load(QString("https://www.google.com/reader/api/0/subscription/list?oauth_token=%1").arg(GoogleOauthDialog::getSingleton().getTempToken()));
   // webView->load(QString("http://www.google.com/reader/atom/user/-/state/com.google/read?oauth_token=%1").arg(GoogleOauthDialog::getSingleton().getTempToken()));

    emit refreshSuccess();
    accept();
}


void GoogleOauthDialog::saveIdentity()
{
    QVariantMap identity;

    identity.insert("Application_Token",applicationAccessToken);
    identity.insert("Temp_Token",tempToken);
    identity.insert("Refresh_Token",refreshToken);
    identity.insert("Last_Refresh_time",QTime::currentTime());
    identity.insert("Last_Refresh_Date",QDate::currentDate());

     QJson::Serializer serializer;
     QByteArray json = serializer.serialize(identity);

     qDebug()<<json;

     QFile file("identity.json");
     file.open(QFile::WriteOnly);
     file.write(json);
     file.close();

}

bool GoogleOauthDialog::loadIdentity()
{
    QFile file("identity.json");

    if(!file.exists())
        return false;


    file.open(QFile::ReadOnly);

    QJson::Parser parser;

    bool ok=false;
    QString fileContent=file.readAll();
    file.close();
    QByteArray json;
    json.append(fileContent);
    QVariantMap result=parser.parse(json,&ok).toMap();

    if(ok)
    {
        applicationAccessToken=result["Application_Token"].toString();
        tempToken=result["Temp_Token"].toString();
        refreshToken=result["Refresh_Token"].toString();


        qDebug()<<"Application_Token:"<<applicationAccessToken;
        qDebug()<<"Temp_Token:"<<tempToken;
        qDebug()<<"Refresh_Token:"<<refreshToken;
        qDebug()<<"Last_Refresh_time:"<<result["Last_Refresh_time"].toString();
        qDebug()<<"Last_Refresh_Date:"<<result["Last_Refresh_Date"].toString();

        if(applicationAccessToken.isNull() || tempToken.isNull() || refreshToken.isNull())
        {
            return false;
        }
        else if(applicationAccessToken.isEmpty() || tempToken.isEmpty() || refreshToken.isEmpty())
        {
            return false;
        }
        else
        {
            refreshIdentity();
            return true;
        }
       }
    else
    {
        qDebug("An error occurred during parsing");
        return false;
    }


}

void GoogleOauthDialog::refreshIdentity()
{
    QString re=QString("client_id=461842547323.apps.googleusercontent.com&client_secret=Jvdw3JMCQcrkhJ0wg7qAp5cf&refresh_token=%1&grant_type=refresh_token").arg(refreshToken);
    QNetworkRequest request(QUrl("https://accounts.google.com/o/oauth2/token"));
    QByteArray dataToSend;
    dataToSend.append(re);
    manager.post(request,dataToSend);
}
