#ifndef FEED_H
#define FEED_H

#include <QString>
#include <QVector>
#include <QImage>
#include <QWebElement>
#include <QObject>

class Feed:public QObject
{
    Q_OBJECT

private:
    QString title;
    QString from;
    QString link;
    QString author;
    QString content;
    QString originalContent;
    QVector<QImage> images;
    int teaserID;

    bool isInitialized;

    bool isInloadingImage;

    int request;

    float contentSize;


public:
    Feed();
    ~Feed();

    void setImages(QVector<QImage> &_imageList);

    void setTeaserID(int _id)
    {
        teaserID=_id;
    }


    Feed(const Feed &in):QObject(),title(in.title),from(in.from),link(in.link),author(in.author),content(in.content),originalContent(in.originalContent),teaserID(in.teaserID),isInitialized(in.isInitialized)
    {
       for(int i=0;i<in.images.size();++i)
        {
            images.push_back(in.images[i]);
        }
    }

    void operator=(const Feed &in)
    {
        title=in.title;
        link=in.link;
        from=in.from;
        author=in.author;
        content=in.content;
        originalContent=in.originalContent;
        teaserID=in.teaserID;
        isInitialized=in.isInitialized;

       for(int i=0;i<in.images.size();++i)
        {
            images.push_back(in.images[i]);
        }
    }

    void setOriginalContent(const QString &_originalContent)
    {
        originalContent=_originalContent;
    }

    void setTitle(const QString _title)
    {
        title=_title;
    }

    void setAuthor(const QString _author)
    {
        author = _author;
    }

    void setFrom(const QString _from)
    {
        from=_from;
    }

    void setLink(const QString &_link)
    {
        link=_link;
    }

    void Initialize();

    QString &getTitle()
    {
        return title;
    }

    QString &getLink()
    {
        return link;
    }

    QString &getAuthor()
    {
        return author;
    }

    QString &getContent()
    {
        return content;
    }

    void setContent(const QString &_content)
    {
        content=_content;
    }

    QString &getOriginalContent()
    {
        return originalContent;
    }

    QString &getFrom()
    {
        return from;
    }

    QImage &getImage(int id)
    {
        return images[id];
    }

    unsigned int getImageSize()
    {
        return images.size();
    }

    int getTeaserID()
    {
        return teaserID;
    }

    void chooseTheTeaserPicture();
    void collectImages(QWebElement &currentNode,QVector<QString> &imageList);

    float getContentSize()
    {
        return contentSize;
    }

    bool operator<(const Feed &n2)
    {
        return contentSize<n2.contentSize;
    }

    bool operator>(const Feed &n2)
    {
        return contentSize>n2.contentSize;
    }

    bool operator==(const Feed &n2)
    {
        return contentSize==n2.contentSize;
    }

private slots:

    void onImageFinished(int,bool);
};

#endif // FEED_H
