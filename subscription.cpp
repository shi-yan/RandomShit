#include "subscription.h"

Subscription::Subscription():googleReaderID(),title(),url()
{
}


Subscription::Subscription(const QString &_googleReaderID,const QString &_title,const QString &_url):
    googleReaderID(_googleReaderID),title(_title),url(_url)
{

}

Subscription::Subscription(const Subscription &in)
    :googleReaderID(in.googleReaderID),title(in.title),url(in.url)
{

}


Subscription::~Subscription()
{

}
