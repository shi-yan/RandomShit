#ifndef FETCHER_H
#define FETCHER_H

#include <QTimer>
#include <QObject>
#include "feed.h"
#include <QFile>
#include "subscription.h"
#include <QVector>

class Fetcher:public QObject
{
    Q_OBJECT

    QTimer fetchTimer;

    QFile log;

private:
    Fetcher();

    ~Fetcher();

    QVector<Subscription> subscriptionList;

public:
    static Fetcher &getSingleton()
    {
        static Fetcher obj;
        return obj;
    }

    void startFetcher(int interval);
    void stopFetcher();
    void getSubscriptionListFromServer();
    void getSubscriptionListFromDatabase();

    QVector<Subscription> & getSubscriptionList()
    {
        return subscriptionList;
    }


public slots:
    void doFetch();
    void onReadListUpdated(QVector<Feed>&);
    void onSubscriptionListUpdated(QVector<Subscription>&);

    void onFeedListForSubscriptionUpdated(QVector<Feed>&);

};

#endif // FETCHER_H
