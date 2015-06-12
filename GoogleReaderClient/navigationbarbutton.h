#ifndef NAVIGATIONBARBUTTON_H
#define NAVIGATIONBARBUTTON_H

#include <QGraphicsObject>



class NavigationBarButton:public QGraphicsObject
{
    Q_OBJECT

    QRect rect;

public:
    NavigationBarButton(QGraphicsItem *parent=0):QGraphicsObject(parent),rect(0,0,16,16)
    {

    }

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    QRectF boundingRect() const
    {
        return rect;
    }

     void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
     void mousePressEvent ( QGraphicsSceneMouseEvent * event );
     void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

signals:
     void released();
};

#endif // NAVIGATIONBARBUTTON_H
