#ifndef GOOGLEOAUTHDIALOG_H
#define GOOGLEOAUTHDIALOG_H

#include <QDialog>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtWebKit/QWebView>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class GoogleOauthDialog:public QDialog
{
    Q_OBJECT

    QHBoxLayout *horizontalLayout;
    QWebView *webView;

    QString applicationAccessToken;
   QString tempToken;
   QString refreshToken;

     QNetworkAccessManager manager;

private:
    GoogleOauthDialog(QWidget *parent=0);
    ~GoogleOauthDialog();
    void showEvent ( QShowEvent * event );
    //void check

public:
    static GoogleOauthDialog & getSingleton()
    {
        static GoogleOauthDialog obj;
        return obj;
    }



    QString &getTempToken()
    {
           return tempToken;
    }

    void saveIdentity();
    bool loadIdentity();
    void refreshIdentity();

 signals:
    void refreshSuccess();


public slots:
    void onLoadFinished(bool);
    void readyRead(QNetworkReply *reply);
};

#endif // GOOGLEOAUTHDIALOG_H
