#include "subscriptiontab.h"
#include <QPainter>
#include <QTextLayout>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

void SubscriptionTab::setText(const QString &_text)
{
    QTextLayout textLayout;

    textLayout.setFont(QFont("Arial",24));

    textLayout.setText(_text);
    QTextOption textOption=textLayout.textOption();

    textOption.setWrapMode(QTextOption::WrapAnywhere);

    textLayout.setTextOption(textOption);
     textLayout.beginLayout();
    QTextLine line = textLayout.createLine();

    if (!line.isValid())
    {
        text=_text;
        textLayout.endLayout();
        return;
    }

    line.setLineWidth(150);

    if(line.textLength()<_text.length())
        text=_text.left(line.textLength()-3).append("...");
    else
        text=_text;

    textLayout.endLayout();
}

void SubscriptionTab::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
    if(tabStatus!=ACTIVE)
    tabStatus=HOVER;

    update();
}

void SubscriptionTab::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
    if(tabStatus!=ACTIVE)
    tabStatus=INACTIVE;

    update();
}

void SubscriptionTab::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    mousePressedPoisition=QPoint(event->scenePos().x(),event->scenePos().y());


    emit subscriptionTabPressed(QPoint(event->scenePos().x(),event->scenePos().y()));
    update();
}


void SubscriptionTab::deactivate()
{
    setZValue(0);
    tabStatus=INACTIVE;
    update();
}

void SubscriptionTab::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{

    switch(tabStatus)
    {

    case ACTIVE:
    {

        painter->fillRect(rect,QBrush(Qt::blue));




       // painter->setPen(QColor::fromRgb(255,255,255));
        painter->setPen(tabColor);
        painter->setFont(QFont("Arial", 24));
        painter->drawText(QRect(6,8,180,30), Qt::AlignLeft, text);
    }

        break;

    case HOVER:
    {
        painter->fillRect(rect,QBrush(Qt::green));

        painter->setPen(QColor::fromRgb(255,255,255));
        painter->setFont(QFont("Arial", 24));
        painter->drawText(QRect(6,8,180,30), Qt::AlignLeft, text);
    }
    break;


    case INACTIVE:
    default:
    {

        painter->fillRect(rect,QBrush(Qt::gray));

     //   painter->setPen(QColor::fromRgb(16,15,15));
        painter->setPen(QColor::fromRgb(68,68,68));

        painter->setFont(QFont("Arial", 24));
        painter->drawText(QRect(6,8,180,30), Qt::AlignLeft, text);
    }
    break;

}
}

void SubscriptionTab::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    emit subscriptionTabMoved(QPoint(event->scenePos().x(),event->scenePos().y()));
}

void SubscriptionTab::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    if((mousePressedPoisition.y()-event->scenePos().y())==0)
    {
        tabStatus=ACTIVE;
        setZValue(10);
        emit onActivated(this);
        update();
    }
}
