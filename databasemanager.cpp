#include "databasemanager.h"
#include <QDebug>
#include <QSqlError>

DatabaseManager::DatabaseManager()
{
}


DatabaseManager::~DatabaseManager()
{

}

void DatabaseManager::open()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("reader");
    db.setDatabaseName("feeds.dat");
    bool ok = db.open();

    if(ok)
    {
        qDebug()<<"open success!";
        insertAnImageQuery=QSqlQuery(db);
        insertAFeedQuery=QSqlQuery(db);
        insertASubscriptionQuery=QSqlQuery(db);
        fetchAnImageQuery=QSqlQuery(db);
        fetchSubscriptionListQuery=QSqlQuery(db);
        fetchFeedListQuery=QSqlQuery(db);
        fetchImageListByFeedIDQuery=QSqlQuery(db);
    }
}

void DatabaseManager::close()
{
    db.close();
}

void DatabaseManager::firstTimeInitialization()
{

}

QVector<QImage> DatabaseManager::fetchImageListByFeedID(int feedID)
{
    fetchImageListByFeedIDQuery.bindValue(0,feedID);
    fetchImageListByFeedIDQuery.exec();

    QVector<QImage> imageList;
    QImage teaser;

    //url,width,height,image,is_teaser

    while(fetchImageListByFeedIDQuery.next())
    {
        QImage resultImage;
        QString url=fetchImageListByFeedIDQuery.value(0).toString();
        int width=fetchImageListByFeedIDQuery.value(1).toInt();
        int height=fetchImageListByFeedIDQuery.value(2).toInt();

        qDebug()<<"image url:"<<url<<"width:"<<width<<"height:"<<height;
        QByteArray imagedata=fetchImageListByFeedIDQuery.value(3).toByteArray();
        QImage temp((uchar*)imagedata.data(),width,height,QImage::Format_ARGB32);
        resultImage=temp.copy();
        bool isTeaser=fetchImageListByFeedIDQuery.value(3).toBool();

        if(isTeaser)
        {
            teaser=resultImage.copy();
        }
        else
        {
            imageList.push_back(resultImage);
        }
    }

    if(!teaser.isNull())
        imageList.push_back(teaser);
    return imageList;
}

QVector<Subscription> DatabaseManager::fetchSubscriptionList()
{
   QVector<Subscription> subscriptionList;
   fetchSubscriptionListQuery.exec();
   while (fetchSubscriptionListQuery.next())
   {
       QString googleReaderID = fetchSubscriptionListQuery.value(0).toString();
       QString title=fetchSubscriptionListQuery.value(1).toString();
       QString url=fetchSubscriptionListQuery.value(2).toString();
       qDebug()<<"subscription id:"<<googleReaderID<<"title:"<<title<<"url:"<<url;
       Subscription sub(googleReaderID,title,url);
       subscriptionList.push_back(sub);
   }
   return subscriptionList;
}

int DatabaseManager::insertASubscription( Subscription &sub)
{
    int rowid=-1;
    insertASubscriptionQuery.bindValue(0,sub.getGoogleReaderID());
    insertASubscriptionQuery.bindValue(1,sub.getTitle());
    insertASubscriptionQuery.bindValue(2,sub.getUrl());

    if(insertASubscriptionQuery.exec())
    {
        return insertASubscriptionQuery.lastInsertId().toInt();
    }
    else
    {
        qDebug()<<insertASubscriptionQuery.lastError();
        return -1;
    }
    return rowid;
}

bool DatabaseManager::needFirstTimeInitialization()
{
    int tableCounter=0;

    QSqlQuery checkTableQuery("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;",db);   
    while (checkTableQuery.next())
    {
        QString tableName = checkTableQuery.value(0).toString();
        qDebug()<<tableName;
        tableCounter++;
    }

    qDebug()<<"Found"<<tableCounter<<"tables!";

    QSqlQuery createFeedTableQuery(db);

    if(createFeedTableQuery.exec("CREATE TABLE feeds(id INTEGER PRIMARY KEY, origin TEXT,title TEXT,link TEXT UNIQUE,author TEXT,content TEXT,original_content TEXT, time DATE);"))
        qDebug()<<"successfully created feed table";
    else
        qDebug()<<"creating feed table failed";

    QSqlQuery createImageTableQuery(db);

    if(createImageTableQuery.exec("CREATE TABLE images(id INTEGER PRIMARY KEY,url TEXT,feed_id INTEGER,width INTEGER,height INTEGER,image BLOB,is_teaser INTEGER);"))
        qDebug()<<"successfully created image table";
    else
        qDebug()<<"creating image table failed";

    QSqlQuery createSubscriptionTableQuery(db);

    if(createSubscriptionTableQuery.exec("CREATE TABLE subscriptions(id INTEGER PRIMARY KEY, googlereaderid TEXT UNIQUE, title TEXT,url TEXT);"))

        qDebug()<<"successfully created subscription table";
    else
        qDebug()<<"creating subscription table failed";

    if(insertAFeedQuery.prepare("INSERT INTO feeds (origin,title,link,author,content,original_content) values (?,?,?,?,?,?);"))
    {
        qDebug()<<"successfully created the feed insertion statement";
    }
    else
        qDebug()<<"failed to create the feed insertion statement";

    if(fetchFeedListQuery.prepare("SELECT id,origin,title,link,author,content,original_content FROM feeds LIMIT ?"))
    {
        qDebug()<<"successfully created the feed fetching statement";
    }
    else
        qDebug()<<"failed to create the feed fetching statement";

    if(insertASubscriptionQuery.prepare("INSERT INTO subscriptions (googlereaderid,title,url) values (?,?,?);"))
    {
        qDebug()<<"successfully created the subscription insertion statement";
    }
    else
        qDebug()<<"failed to create the subscription insertion statement";

    if(insertAnImageQuery.prepare("INSERT INTO images (url,feed_id,image,width,height,is_teaser) values (?,?,?,?,?,?);"))
        qDebug()<<"successfully created the image insertion statement";
    else
        qDebug()<<"failed to create the image insertion statement";

    if(fetchAnImageQuery.prepare("SELECT url,width,height,image FROM images WHERE id=?;"))
    {
        qDebug()<<"successfully created the image fetching statement";
    }
    else
        qDebug()<<"failed to create the image fetching statement";

    if(fetchImageListByFeedIDQuery.prepare("SELECT url,width,height,image,is_teaser FROM images WHERE feed_id=?;"))
    {
        qDebug()<<"successfully created the image list fetching statement";
    }
    else
        qDebug()<<"failed to create the image list fetching statement";

    if(fetchSubscriptionListQuery.prepare("SELECT googlereaderid,title,url FROM subscriptions;"))
        qDebug()<<"successfully created the subscription fetching statement";
    else
        qDebug()<<"failed to create the subscription fetching statement";

    return true;
}


QImage DatabaseManager::fetchAnImageByID(int id)
{
    QImage resultImage;

    fetchAnImageQuery.bindValue(0,1);

    fetchAnImageQuery.exec();

    if(fetchAnImageQuery.first())
    {
        QString url=fetchAnImageQuery.value(0).toString();
        int width=fetchAnImageQuery.value(1).toInt();
        int height=fetchAnImageQuery.value(2).toInt();

        qDebug()<<"image url:"<<url<<"width:"<<width<<"height:"<<height;
        QByteArray imagedata=fetchAnImageQuery.value(3).toByteArray();
        QImage temp((uchar*)imagedata.data(),width,height,QImage::Format_ARGB32);
        resultImage=temp.copy();
    }

    qDebug()<<fetchAnImageQuery.lastError();
    return resultImage;
}

void DatabaseManager::dropEveryThing()
{
    QSqlQuery dropFeedTableQuery(db);

    if(dropFeedTableQuery.exec("DROP TABLE feeds;"))
    {
        qDebug()<<"successfully dropped feed table";
    }
    else
        qDebug()<<"dropping feed table failed";

    QSqlQuery dropImageTableQuery(db);

    if(dropImageTableQuery.exec("DROP TABLE images;"))
    {
        qDebug()<<"successfully dropped image table";
    }
    else
    {
        qDebug()<<"dropping image table failed";
    }

    QSqlQuery dropSubscriptionTableQuery(db);

    if(dropSubscriptionTableQuery.exec("DROP TABLE subscriptions;"))
    {
        qDebug()<<"successfully dropped subscription table";
    }
    else
    {
        qDebug()<<"dropping subscription table failed";
    }
}

int DatabaseManager::insertAFeed(Feed &feed)
{
    int rowid=-1;
    insertAFeedQuery.bindValue(0,feed.getFrom());
    insertAFeedQuery.bindValue(1,feed.getTitle());
    insertAFeedQuery.bindValue(2,feed.getLink());
    insertAFeedQuery.bindValue(3,feed.getAuthor());
    insertAFeedQuery.bindValue(4,feed.getContent());
    insertAFeedQuery.bindValue(5,feed.getOriginalContent());

    if(insertAFeedQuery.exec())
    {
        qDebug()<<"inserting feed successfully";
        return insertAFeedQuery.lastInsertId().toInt();
    }
    else
    {
        qDebug()<<"feed insertion failed!"<<insertAFeedQuery.lastError();
        return -1;
    }

    return rowid;
}

QVector<Feed> DatabaseManager::fetchFeedList(int time,int size)
{
    fetchFeedListQuery.bindValue(0,size);

    fetchFeedListQuery.exec();

    QVector<Feed> feedList;

    while(fetchFeedListQuery.next())
    {
        Feed f;

        //id,origin,title,link,author,content,original_content

        int feedID=fetchFeedListQuery.value(0).toInt();

        f.setFrom(fetchFeedListQuery.value(1).toString());
        f.setTitle(fetchFeedListQuery.value(2).toString());
        f.setLink(fetchFeedListQuery.value(3).toString());
        f.setAuthor(fetchFeedListQuery.value(4).toString());
        f.setContent(fetchFeedListQuery.value(5).toString());

        QVector<QImage> imageList=fetchImageListByFeedID(feedID);

        if(imageList.size()>0)
        {
            f.setImages(imageList);
            f.setTeaserID(imageList.size()-1);
        }

        feedList.push_back(f);
    }

    return feedList;
}

int DatabaseManager::insertAnImage(QImage &image,QString &url,int feedID,bool isTeaser)
{
    QImage toBeInserted=image.convertToFormat(QImage::Format_ARGB32);

    int rowid=-1;

    QByteArray imagedata((const char*)toBeInserted.bits(),toBeInserted.byteCount());

    insertAnImageQuery.bindValue(0,url);
    insertAnImageQuery.bindValue(1,feedID);
    insertAnImageQuery.bindValue(2,imagedata);
    insertAnImageQuery.bindValue(3,toBeInserted.width());
    insertAnImageQuery.bindValue(4,toBeInserted.height());
    insertAnImageQuery.bindValue(5,isTeaser);


    if(insertAnImageQuery.exec())
    {
        qDebug()<<"successfully inserted an image at row:"<<insertAnImageQuery.lastInsertId().toInt();

    }
    else
    {
        qDebug()<<"inserting image failed";
        return -1;
    }
    return rowid;
}
