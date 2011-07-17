#include "navigationbarbutton.h"
#include <QPainter>
#include <QDebug>


void NavigationBarButton::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{

}

void NavigationBarButton::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{

}

void NavigationBarButton::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    emit released();
}

void NavigationBarButton::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->fillRect(rect,QBrush(Qt::red));
}
