#ifndef TABBARHANDLE_H
#define TABBARHANDLE_H

#include <QGraphicsObject>
#include <QPoint>

class TabBarHandle:public QGraphicsObject
{
    Q_OBJECT

    bool isDragging;

    QRect rect;

    int speed;

public:

    enum Direction
    {
        OPEN,
        CLOSE,
        NODIRECTION,
    };

    TabBarHandle(QGraphicsItem *parent=0);

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void setRect(const QRect &_rect)
    {
        rect=_rect;
    }

    QRectF boundingRect() const
    {
        return rect;
    }

     void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

     void mousePressEvent ( QGraphicsSceneMouseEvent * event );

     void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );


signals:

     void draggedTo(int position);

     void barReleased(enum TabBarHandle::Direction dir,int speed);


};

#endif // TABBARHANDLE_H
