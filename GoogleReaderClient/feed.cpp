#include "feed.h"
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QApplication>
#include <QUrl>
#include <QDebug>
#include <QHttp>
#include <QBuffer>
#include <QByteArray>
#include "databasemanager.h"

Feed::Feed():isInitialized(false),isInloadingImage(false)
{
}

Feed::~Feed()
{
}

void Feed:: setImages(QVector<QImage> &_imageList)
{
    images.clear();

    for(int i=0;i<_imageList.size();++i)
        images.push_back(_imageList[i]);
}

void Feed::Initialize()
{
    chooseTheTeaserPicture();

    contentSize=content.length()>500?500:content.length();

    if(images.size())
    {
        contentSize+=0.1f*(images[teaserID].size().width()+images[teaserID].size().height());
    }
}

void Feed::onImageFinished(int _requestID,bool)
{
    if(request==_requestID)
        isInloadingImage=false;
}

void Feed::chooseTheTeaserPicture()
{
    QWebPage frame;
    frame.mainFrame()->setHtml(originalContent);
    content=frame.mainFrame()->toPlainText();
    QWebElement doc=frame.mainFrame()->documentElement();

    QVector<QString> imageList;
    collectImages(doc,imageList);
    qDebug()<<"number of images"<<imageList.size();

    for(int i=0;i<imageList.size();++i)
    {
        QUrl url(imageList[i]);
        QHttp http;
        connect(&http, SIGNAL(requestFinished(int, bool)),this, SLOT(onImageFinished(int,bool)));
        isInloadingImage=true;

        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        http.setHost(url.host());
        request=http.get (url.path(),&buffer);

        //qDebug()<<"before image";
        while(isInloadingImage)
            QApplication::processEvents();
        //qDebug()<<"after image";

        QImage img;
        img.loadFromData(bytes);
        //segmentation fault

        if(img.width()*img.height()>3000)
        {
            images.push_back(img);

          //  DatabaseManager::getSingleton().insertAnImage(img,imageList[i]);
        }

        //qDebug()<<"finish Loading one image:"<<img.width()<<img.height();
        request=0;
    }

    teaserID=0;
    int largestArea=0;

    for(int i=0;i<images.size();++i)
    {
        if(images[i].width()*images[i].height()>largestArea)
        {
            teaserID=i;
            largestArea=images[i].width()*images[i].height();
        }
    }

    isInitialized=true;

    qDebug()<<"finsih initialization";
}



void Feed::collectImages(QWebElement &currentNode,QVector<QString> &imageList)
{
   // qDebug()<<currentNode.tagName();
    QWebElement node=currentNode.firstChild();
    while(!node.isNull())
    {
       // qDebug()<<node.tagName();
        if(node.tagName()=="IMG")
        {
            imageList.push_back(node.attribute("src"));

           // qDebug()<<"Ok find one image:"<<node.attribute("src");
        }
        else if(!node.firstChild().isNull())
        {
            collectImages(node,imageList);
        }
        node=node.nextSibling();
    }
}

