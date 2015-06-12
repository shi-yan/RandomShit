#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
//#include "sqlite/sqlite3.h"
#include <QImage>
#include <QString>
#include "feed.h"
#include <QFile>
#include "subscription.h"
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>

class DatabaseManager:public QObject
{
    Q_OBJECT
private:
    DatabaseManager();
    ~DatabaseManager();

   // sqlite3 *db;

    QSqlDatabase db;

 /* sqlite3_stmt *insertAnImageStatement;
    sqlite3_stmt *fetchAnImageStatement;
    sqlite3_stmt *insertAFeedStatement;
    sqlite3_stmt *insertASubscriptionStatement;
    sqlite3_stmt *fetchSubscriptionListStatement;*/

    QSqlQuery insertAnImageQuery;
    QSqlQuery fetchAnImageQuery;
    QSqlQuery insertAFeedQuery;
    QSqlQuery insertASubscriptionQuery;
    QSqlQuery fetchSubscriptionListQuery;
    QSqlQuery fetchFeedListQuery;
    QSqlQuery fetchImageListByFeedIDQuery;

public:

    static DatabaseManager &getSingleton()
    {
        static DatabaseManager obj;
        return obj;
    }

    void open();
    void close();
    void firstTimeInitialization();
    bool needFirstTimeInitialization();

    int insertAnImage(QImage &image,QString &url,int feedID,bool isTeaser);
    int insertAFeed(Feed &feed);
    int insertASubscription( Subscription&);

    QImage fetchAnImageByID(int id);

    QVector<Subscription> fetchSubscriptionList();

    QVector<Feed> fetchFeedList(int time,int size);

    QVector<QImage> fetchImageListByFeedID(int feedID);

    void dropImageTable();
    void dropEveryThing();
};

#endif // DATABASEMANAGER_H
