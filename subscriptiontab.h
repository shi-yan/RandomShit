#ifndef SUBSCRIPTIONTAB_H
#define SUBSCRIPTIONTAB_H

#include <QGraphicsObject>
#include "subscription.h"


class SubscriptionTab:public QGraphicsObject
{
    Q_OBJECT

    Subscription subscription;
    QString text;

    enum Status
    {
        INACTIVE,
        ACTIVE,
        HOVER,
    };

    QRect rect;

    enum Status tabStatus;

    QColor tabColor;

    QPoint mousePressedPoisition;



public:
    SubscriptionTab(QGraphicsItem *parent=0):QGraphicsObject(parent),rect(0,0,180,46),tabStatus(INACTIVE),tabColor(QColor::fromRgb(255,0,0))
    {
        setAcceptHoverEvents(true);
    }

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    void setText(const QString &_text);

    void setRect(const QRect &_rect)
    {
        rect=_rect;
    }

    void setTabColor(const QColor &_tabColor)
    {
        tabColor=_tabColor;
    }

    QRectF boundingRect() const
    {
        return rect;
    }

    void deactivate();

    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

    signals:

    void onActivated(SubscriptionTab *);
    void subscriptionTabPressed(QPoint mousePosition);
    void subscriptionTabMoved(QPoint mousePosition);

    };

#endif // SUBSCRIPTIONTAB_H
