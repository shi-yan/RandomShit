#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDomNode>
#include <QBuffer>
#include <QHttp>
#include <QWebElement>
#include <QGraphicsObject>
#include "feed.h"

#include <QTextLayout>

class Tile:public QGraphicsObject
{
    Q_OBJECT

    Feed feed;
    QImage teaser;

    enum LayoutType
    {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        TOP_LEFT,
        TEXT_ONLY,
    };

    enum LayoutType layoutType;

    QTextLayout titleLayout;
    QTextLayout contentLayout;

    QRect rect;

public:
    Tile():QGraphicsObject()
    {}
    ~Tile(){}

    void setFeed(const Feed &_in)
    {
        feed=_in;
    }

    void initialize();

    void setRect(QRect _r)
    {
        rect=_r;
    }

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    QRectF boundingRect() const
    {
        return rect;
    }

};

#endif // TILE_H
