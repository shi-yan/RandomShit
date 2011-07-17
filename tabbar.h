#ifndef TABBAR_H
#define TABBAR_H

#include <QGraphicsObject>

#include <QVector>
#include "subscriptiontab.h"
#include "tabbarhandle.h"
#include <QTimeLine>
#include <QGraphicsItemAnimation>

class TabBar:public QGraphicsObject
{
    Q_OBJECT

    QRect rect;
    SubscriptionTab *currentTab;

    TabBarHandle *handle;

    int currentHandlePos;

    int tabBarWidth;

    QGraphicsItemAnimation openCloseAnimation;

    QTimeLine openCloseAnimationTimer;

    enum TabBarHandleStatus
    {
        CLOSED,
        OPENNED,
    };

    enum TabBarHandleStatus currentTabBarHandleStatus;

    int currentSubscriptionTabOffset;

    int subscriptionTabOffset;

    QVector<SubscriptionTab*> subscriptionTabList;


    TabBar():QGraphicsObject(),currentTab(NULL),openCloseAnimation(),openCloseAnimationTimer(),currentTabBarHandleStatus(CLOSED),subscriptionTabOffset(0)
    {
        setZValue(10);

        handle=new TabBarHandle(this);
        handle->setPos(0,0);

        connect(handle,SIGNAL(barReleased(enum TabBarHandle::Direction,int)),this, SLOT(onBarReleased(enum TabBarHandle::Direction,int)));

        connect(handle,SIGNAL(draggedTo(int)),this,SLOT(onBarDraggedTo(int)));
    }
public:

    static TabBar & getSingleton()
    {
        static TabBar obj;
        return obj;
    }

    void setTabBarWidth(int _tabBarWidth)
    {
        tabBarWidth=_tabBarWidth;
    }

    void addNewTab(const QString &tabName);

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void setHeight(const int height)
    {

        rect=QRect(0,0,tabBarWidth,height);

        setPos(currentHandlePos+10-tabBarWidth,0);

        handle->setRect(QRect(0,0,10,rect.height()));

        handle->setPos(tabBarWidth-10,0);
    }

    QRectF boundingRect() const
    {
        return rect;
    }

    void wheelEvent ( QGraphicsSceneWheelEvent * event );

public slots:
    void onActivated(SubscriptionTab *);

    void onBarReleased(enum TabBarHandle::Direction,int);

    void onBarDraggedTo(int pos);

    void onSubscriptionTabPressed(QPoint mousePos);

    void onSubscriptionTabMoved(QPoint mousePos);
};

#endif // TABBAR_H
