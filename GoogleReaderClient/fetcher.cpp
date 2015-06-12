#include "fetcher.h"
#include "googlereaderapi.h"
#include <QDebug>
#include "databasemanager.h"
#include <QApplication>


Fetcher::Fetcher():QObject(),fetchTimer(this),log("log.txt")
{
    connect(&fetchTimer,SIGNAL(timeout()),this,SLOT(doFetch()));
    connect(&GoogleReaderAPI::getSingleton(),SIGNAL(feedListUpdated(QVector<Feed>&)),this,SLOT(onReadListUpdated(QVector<Feed>&)));

    connect(&GoogleReaderAPI::getSingleton(),SIGNAL(subscriptionListUpdated(QVector<Subscription>&)),this,SLOT(onSubscriptionListUpdated(QVector<Subscription>&)));

    connect(&GoogleReaderAPI::getSingleton(),SIGNAL(feedListForSubscriptionUpdate(QVector<Feed>&)),this,SLOT(onFeedListForSubscriptionUpdated(QVector<Feed>&)));
}

void Fetcher::onFeedListForSubscriptionUpdated(QVector<Feed> &feedList)
{
    foreach(Feed f, feedList)
    {

        f.Initialize();

        int feedDatabaseID=-1;

        if((feedDatabaseID=DatabaseManager::getSingleton().insertAFeed(f))>0)
        {
            int imageSize=f.getImageSize();

            QString imageurl("www.testimageurl.com/test.png");

            for(int e=0;e<imageSize;++e)
            {

                DatabaseManager::getSingleton().insertAnImage(f.getImage(e),imageurl,feedDatabaseID,f.getTeaserID()==e?true:false);
            }

            qDebug()<<"inserted a new feed at"<<feedDatabaseID<<"!";

            QByteArray out;

            out.append(QString("inserted a new feed at row %1 with title: %2\n").arg(feedDatabaseID).arg(f.getTitle()));

            log.write(out);
        }
        else
            qDebug()<<"save to database failed!";
    }
}


void Fetcher::startFetcher(int interval)
{
    doFetch();
    fetchTimer.start(interval*60000);
}

void Fetcher::stopFetcher()
{
    fetchTimer.stop();
}

void Fetcher::doFetch()
{
    foreach(Subscription sub , subscriptionList)
    {


        qDebug()<<"fetching:"<<sub.getGoogleReaderID();
        while(GoogleReaderAPI::getSingleton().isNetworkBusy())
            QApplication::processEvents();
        GoogleReaderAPI::getSingleton().getFeedListOfSubscription(sub);

        //break;
    }
}

void Fetcher::getSubscriptionListFromServer()
{
    qDebug()<<"getting the subscription list from the server.";

    GoogleReaderAPI::getSingleton().getSubscriptionList();
}

void Fetcher::getSubscriptionListFromDatabase()
{
    qDebug()<<"getting the subscription list from the database.";

    subscriptionList.clear();

    subscriptionList=DatabaseManager::getSingleton().fetchSubscriptionList();
}

void Fetcher::onSubscriptionListUpdated(QVector<Subscription> &_subscriptionList)
{
    qDebug()<<"#############################on subscription list updated";

    subscriptionList.clear();

    foreach(Subscription sub, _subscriptionList)
    {
        subscriptionList.push_back(sub);

        int subscriptionDatabaseID=-1;

        if((subscriptionDatabaseID=DatabaseManager::getSingleton().insertASubscription(sub))>0)
        {
            qDebug()<<"sucessfully inserted a subscription at"<<subscriptionDatabaseID;
        }
        else
            qDebug()<<"save to database failed!";
    }
}

void Fetcher::onReadListUpdated(QVector<Feed> &feedList)
{

    qDebug()<<"size"<<feedList.size();


    log.open(QFile::Append);




    for(int i=0;i<feedList.size();++i)
    {
        qDebug()<<"saving feed"<<i;

        feedList[i].Initialize();

        int feedDatabaseID=-1;

        if((feedDatabaseID=DatabaseManager::getSingleton().insertAFeed(feedList[i]))>0)
        {
            int imageSize=feedList[i].getImageSize();

            QString imageurl("www.testimageurl.com/test.png");

            for(int e=0;e<imageSize;++e)
            {

                DatabaseManager::getSingleton().insertAnImage(feedList[i].getImage(e),imageurl,feedDatabaseID,feedList[i].getTeaserID()==e?true:false);
            }

            qDebug()<<"inserted a new feed!";

            QByteArray out;

            out.append(QString("inserted a new feed at row %1 with title: %2\n").arg(feedDatabaseID).arg(feedList[i].getTitle()));

            log.write(out);
        }
        else
            qDebug()<<"save to database failed!";
    }

    log.close();
}


Fetcher::~Fetcher()
{

}
