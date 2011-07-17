#include "googlereaderapi.h"
#include <QXmlReader>
#include "googleoauthdialog.h"
#include <QDebug>
#include <QDomDocument>
#include <QVariantMap>
#include "json/parser.h"
#include "subscription.h"
#include "xmlerrorhandler.h"

GoogleReaderAPI::GoogleReaderAPI():_networkBusy(false)
{
    connect(&GoogleOauthDialog::getSingleton(),SIGNAL(refreshSuccess()),this,SLOT(onIdentityRefreshFinished()));
}

GoogleReaderAPI::~GoogleReaderAPI()
{

}

void GoogleReaderAPI::onIdentityRefreshFinished()
{
    isRefreshing=false;
}

void GoogleReaderAPI::onGetReadListFinished(QNetworkReply* reply)
{

    QVector<Feed> feedList;

    disconnect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onReadListFinished(QNetworkReply*)));

    if(reply->isFinished())
    {


        qDebug()<<"Error code:"<<reply->error()<<" "<<reply->errorString();

        if(reply->error()==204)
        {
            //Need Refresh
            if(retryCount<3)
            {
                isRefreshing=true;

                GoogleOauthDialog::getSingleton().refreshIdentity();

                while(isRefreshing)
                    QApplication::processEvents();

                retryCount++;

                qDebug()<<"retry one more time due to authentication expired!";

                getReadList();

            }
        }

        else
        {
            retryCount=0;

            feedList.clear();

              QString content=reply->readAll();
           // qDebug()<<content;

              QDomDocument document;



             qDebug()<<"result of parsing"<< document.setContent(content);

             QFile qf("test.xml");
             qf.open(QFile::WriteOnly);
             QByteArray out;
             out.append(content);
             qf.write(out);
             qf.close();

             QDomNode top=document.firstChild();

             while(!top.isNull())
             {
                 if(top.nodeType()==QDomNode::ElementNode)
                 {
                    qDebug()<<"type:"<<top.nodeType()<<"name:"<<top.nodeName();

                    if(top.nodeName()=="feed")
                    {
                        QDomNode feed=top.firstChild();

                        while(!feed.isNull())
                        {
                            if(feed.nodeType()==QDomNode::ElementNode && feed.nodeName()=="entry")
                            {
                                qDebug()<<"\ttype:"<<feed.nodeType()<<"name:"<<feed.nodeName();

                                QDomNode entry=feed.firstChild();

                                Feed f;

                                while(!entry.isNull())
                                {
                                  //  qDebug()<<"\t\ttype:"<<entry.nodeType()<<"name:"<<entry.nodeName()<<"value:"<<entry.firstChild().nodeValue();

                                    if(entry.nodeName()=="title")
                                    {
                                        f.setTitle(QString().fromUtf8(entry.firstChild().nodeValue().toStdString().c_str()));
                                        qDebug()<<"\t\ttitle:"<<f.getTitle();
                                    }
                                    else
                                    if(entry.nodeName()=="link")
                                    {
                                        f.setLink(entry.attributes().namedItem("href").nodeValue().toStdString().c_str());
                                        qDebug()<<"\t\tlink:"<<f.getLink();
                                    }
                                    else if(entry.nodeName()=="author")
                                    {
                                        f.setAuthor(QString().fromUtf8(entry.firstChild().firstChild().nodeValue().toStdString().c_str()));
                                        qDebug()<<"\t\tauthor:"<<f.getAuthor();
                                    }
                                    else if(entry.nodeName()=="summary" || entry.nodeName()=="content")
                                    {
                                        qDebug()<<"summary";
                                        f.setOriginalContent(QString().fromUtf8(entry.firstChild().nodeValue().toStdString().c_str()));
                                        feedList.push_back(f);
                                    }
                                    else if(entry.nodeName()=="source")
                                    {
                                        QDomNode from=entry.firstChild();

                                        while(!from.isNull())
                                        {

                                            if(from.nodeName()=="link")
                                            {
                                                qDebug()<<"from";

                                                f.setFrom(from.attributes().namedItem("href").nodeValue());
                                            }
                                            from=from.nextSibling();
                                        }
                                    }
                                    else
                                        qDebug()<<entry.nodeName();
                                    entry=entry.nextSibling();
                                }
                            }

                            feed=feed.nextSibling();
                        }
                    }
                 }

                 top=top.nextSibling();
             }
        }


        emit feedListUpdated(feedList);


    }
    else
    {
        qDebug()<<"Error code:"<<reply->error()<<" "<<reply->errorString();
    }
}

void GoogleReaderAPI::getSubscriptionList()
{

    _networkBusy=true;

    QNetworkRequest request(QUrl(QString("http://www.google.com/reader/api/0/subscription/list?oauth_token=%1&output=json").arg(GoogleOauthDialog::getSingleton().getTempToken())));

    qDebug()<<"XXXXX";
    qDebug()<<request.url().toString();
    qDebug()<<"xxxxx";

    connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onGetSubscriptionListFinished(QNetworkReply*)));

    manager.get(request);

}

void GoogleReaderAPI::onGetSubscriptionListFinished(QNetworkReply *reply)
{
    disconnect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onGetSubscriptionListFinished(QNetworkReply*)));

    QVector<Subscription> subscriptionList;
    if(reply->isFinished())
    {
        qDebug()<<"Error code:"<<reply->error()<<" "<<reply->errorString();

        if(reply->error()==204)
        {
            //Need Refresh
            if(retryCount<3)
            {
                isRefreshing=true;

                GoogleOauthDialog::getSingleton().refreshIdentity();

                while(isRefreshing)
                    QApplication::processEvents();

                retryCount++;

                qDebug()<<"retry one more time due to authentication expired!";

                getReadList();

            }
        }

        else
        {
            QString substriptionListJSON=reply->readAll();
            QJson::Parser parser;
            QByteArray json;
            json.append(substriptionListJSON);
            bool ok=false;
            QVariantMap result = parser.parse (json, &ok).toMap();

            if (!ok)
            {
                qDebug("An error occurred during parsing");
                return;
            }

            foreach (QVariant subscription, result["subscriptions"].toList())
            {
                QVariantMap aSubscription=subscription.toMap();
                qDebug()<<aSubscription["id"].toString();
                qDebug()<<aSubscription["title"].toString();
                qDebug()<<aSubscription["htmlUrl"].toString();
                Subscription aNewSub(QString().fromUtf8(aSubscription["id"].toString().toStdString().c_str()),aSubscription["title"].toString(),aSubscription["htmlUrl"].toString());
                subscriptionList.push_back(aNewSub);
            }
            emit subscriptionListUpdated(subscriptionList);
        }
    }
    else
    {
        qDebug()<<"Error code:"<<reply->error()<<" "<<reply->errorString();
    }


}


void GoogleReaderAPI::onGetFeedListOfSubscriptionFinished(QNetworkReply *reply)
{

    QVector<Feed> feedList;

    disconnect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onGetFeedListOfSubscriptionFinished(QNetworkReply*)));

    if(reply->isFinished())
    {


        qDebug()<<"Error code:"<<reply->error()<<" "<<reply->errorString();

        if(reply->error()==204)
        {
            //Need Refresh
            if(retryCount<3)
            {
                isRefreshing=true;

                GoogleOauthDialog::getSingleton().refreshIdentity();

                while(isRefreshing)
                    QApplication::processEvents();

                retryCount++;

                qDebug()<<"retry one more time due to authentication expired!";

                getReadList();

            }
        }

        else
        {
            retryCount=0;

            feedList.clear();

              QString content=reply->readAll();
           // qDebug()<<content;

              QDomDocument document;

              QString errorMsg;
              int errorLine;
              int errorColumn;

              if(!document.setContent(content,&errorMsg,&errorLine,&errorColumn))
              {

                  qDebug()<<"error parsing the xml at line:"<<errorLine<<" and column:"<<errorColumn<<" Msg:"<<errorMsg;
              }

              QXmlSimpleReader reader;

              QXmlInputSource xmlSource;

            XmlErrorHandler *handler = new XmlErrorHandler;
                 //  reader.setContentHandler(handler);
                   reader.setErrorHandler(handler);

               xmlSource.setData(content);

               if(!reader.parse(xmlSource))

               {
                  qDebug()<<"still see the error!";

              }



             QFile qf("fuck.xml");
             qf.open(QFile::WriteOnly);
             QByteArray out;
             out.append(content);
             qf.write(out);
             qf.close();

             QDomNode top=document.firstChild();

             while(!top.isNull())
             {
                 if(top.nodeType()==QDomNode::ElementNode)
                 {
                    qDebug()<<"type:"<<top.nodeType()<<"name:"<<top.nodeName();

                    if(top.nodeName()=="feed")
                    {
                        QDomNode feed=top.firstChild();
qDebug()<<"type:"<<top.nodeType()<<"name:"<<top.nodeName();
                        while(!feed.isNull())
                        {
                            if(feed.nodeType()==QDomNode::ElementNode && feed.nodeName()=="entry")
                            {
                                qDebug()<<"\ttype:"<<feed.nodeType()<<"name:"<<feed.nodeName();

                                QDomNode entry=feed.firstChild();

                                Feed f;

                                while(!entry.isNull())
                                {
                                  //  qDebug()<<"\t\ttype:"<<entry.nodeType()<<"name:"<<entry.nodeName()<<"value:"<<entry.firstChild().nodeValue();

                                    if(entry.nodeName()=="title")
                                    {
                                        f.setTitle(QString().fromUtf8(entry.firstChild().nodeValue().toStdString().c_str()));
                                        qDebug()<<"\t\ttitle:"<<f.getTitle();
                                    }
                                    else
                                    if(entry.nodeName()=="link")
                                    {
                                        f.setLink(entry.attributes().namedItem("href").nodeValue().toStdString().c_str());
                                        qDebug()<<"\t\tlink:"<<f.getLink();
                                    }
                                    else if(entry.nodeName()=="author")
                                    {
                                        f.setAuthor(QString().fromUtf8(entry.firstChild().firstChild().nodeValue().toStdString().c_str()));
                                        qDebug()<<"\t\tauthor:"<<f.getAuthor();
                                    }
                                    else if(entry.nodeName()=="summary" || entry.nodeName()=="content")
                                    {
                                        qDebug()<<"summary";
                                        f.setOriginalContent(QString().fromUtf8(entry.firstChild().nodeValue().toStdString().c_str()));
                                        feedList.push_back(f);
                                    }
                                    else if(entry.nodeName()=="source")
                                    {
                                        QDomNode from=entry.firstChild();

                                        while(!from.isNull())
                                        {

                                            if(from.nodeName()=="link")
                                            {
                                                qDebug()<<"from";

                                                f.setFrom(from.attributes().namedItem("href").nodeValue());
                                            }
                                            from=from.nextSibling();
                                        }
                                    }
                                    else
                                        qDebug()<<entry.nodeName();
                                    entry=entry.nextSibling();
                                }
                            }

                            feed=feed.nextSibling();
                        }
                    }
                 }

                 top=top.nextSibling();
             }
        }


        emit feedListForSubscriptionUpdate(feedList);


    }
    else
    {
        qDebug()<<"Error code:"<<reply->error()<<" "<<reply->errorString();
    }

    _networkBusy=false;

}

void GoogleReaderAPI::getFeedListOfSubscription(Subscription &sub)
{
    _networkBusy=true;

    QNetworkRequest request(QUrl(QString("http://www.google.com/reader/atom/%1?oauth_token=%2").arg(sub.getGoogleReaderID()).arg(GoogleOauthDialog::getSingleton().getTempToken())));


    qDebug()<<"#####";
    qDebug()<<request.url().toString();
    qDebug()<<"#####";

    connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onGetFeedListOfSubscriptionFinished(QNetworkReply*)));


    manager.get(request);


}

void GoogleReaderAPI::getReadList()
{
    QNetworkRequest request(QUrl(QString("https://www.google.com/reader/atom/user/-/state/com.google/reading-list?oauth_token=%1&n=50").arg(GoogleOauthDialog::getSingleton().getTempToken())));
   // QNetworkRequest request(QUrl(QString("http://www.google.com/reader/atom/feed/http%3A%2F%2Fsongshuhui.net%2F%3Ffeed%3Drss2")));

    qDebug()<<"#####";
    qDebug()<<request.url().toString();
    qDebug()<<"#####";

    connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onGetReadListFinished(QNetworkReply*)));


    manager.get(request);
}
