#ifndef GOOGLEREADERAPI_H
#define GOOGLEREADERAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QVector>
#include "feed.h"
#include <QNetworkReply>
#include "subscription.h"

class GoogleReaderAPI:public QObject
{
    Q_OBJECT

    bool isRefreshing;
    int retryCount;

private:
    GoogleReaderAPI();
    ~GoogleReaderAPI();

    QNetworkAccessManager manager;


    bool _networkBusy;


public:
    static GoogleReaderAPI &getSingleton()
    {
        static GoogleReaderAPI obj;
        return obj;
    }

    void getReadList();
    void getSubscriptionList();
    void getFeedListOfSubscription(Subscription &sub);

    bool isNetworkBusy()
    {
        return _networkBusy;
    }

private slots:
    void onGetReadListFinished(QNetworkReply*);
    void onIdentityRefreshFinished();
    void onGetSubscriptionListFinished(QNetworkReply*);
    void onGetFeedListOfSubscriptionFinished(QNetworkReply*);



signals:
    void feedListUpdated(QVector<Feed>&);
    void feedListForSubscriptionUpdate(QVector<Feed>&);
    void subscriptionListUpdated(QVector<Subscription>&);

};

#endif // GOOGLEREADERAPI_H
