#ifndef FAKEPAGE_H
#define FAKEPAGE_H
#include <QGraphicsObject>

class FakePage:public QGraphicsObject
{
    Q_OBJECT

    QRect rect;

public:
    FakePage():QGraphicsObject()
    {}

    void setRect(const QRect &_rect)
    {
        rect=_rect;
    }

    QRectF boundingRect() const
    {
        return rect;
    }

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

};

#endif // FAKEPAGE_H
