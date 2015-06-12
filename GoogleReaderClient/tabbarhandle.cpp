#include "tabbarhandle.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

TabBarHandle::TabBarHandle(QGraphicsItem *parent):QGraphicsObject(parent),isDragging(false),rect(0,0,0,0)
{
    setZValue(10);
}

void TabBarHandle::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->fillRect(rect,QBrush(Qt::black));
}

void TabBarHandle::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    if(event->buttons()|Qt::LeftButton)
    {
        qDebug()<<"dragging"<<event->scenePos().x()-event->lastScenePos().x();

        speed=event->scenePos().x()-event->lastScenePos().x();

        qDebug()<<"dragging - "<<event->scenePos().x()<<event->pos().x();

        emit draggedTo(event->scenePos().x());
    }

    event->accept();

}

void TabBarHandle::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    speed=0;
    event->accept();
}

void TabBarHandle::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    qDebug()<<event->pos().x();
    if(speed>0)
    {
        emit barReleased(OPEN,speed);
    }
    else if(speed<0)
    {
        emit barReleased(CLOSE,speed);
    }
    else
    {
        emit barReleased(NODIRECTION,1);
    }
    event->accept();
}
