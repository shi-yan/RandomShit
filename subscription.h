#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <QObject>
#include <QString>

class Subscription :public QObject
{
    Q_OBJECT

private:
    QString googleReaderID;
    QString url;
    QString title;

public:
    Subscription();
    Subscription(const QString &_googleReaderID,const QString &_title,const QString &_url);
    Subscription(const Subscription &);

    void operator=(const Subscription &in)
    {
        googleReaderID=in.googleReaderID;
        url=in.url;
        title=in.title;
    }

    ~Subscription();

    QString & getTitle()
    {
        return title;
    }

    QString & getUrl()
    {
        return url;
    }

    QString & getGoogleReaderID()
    {
        return googleReaderID;
    }

    void setTitle(const QString &_title)
    {
        title=_title;
    }

    void setUrl(const QString &_url)
    {
        url=_url;
    }

    void setGoogleReaderID(const QString &_googleReaderID)
    {
        googleReaderID=_googleReaderID;
    }
};

#endif // SUBSCRIPTION_H
