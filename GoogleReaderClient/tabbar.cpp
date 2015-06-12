#include "tabbar.h"
#include <QDebug>
#include <QGraphicsSceneWheelEvent>

void TabBar::addNewTab(const QString &tabName)
{
    SubscriptionTab *sTab=new SubscriptionTab(this);
    sTab->setText(tabName);
    sTab->setPos(0,subscriptionTabList.size()*46+subscriptionTabOffset);

    subscriptionTabList.push_back(sTab);

    connect(sTab,SIGNAL(onActivated(SubscriptionTab*)),this,SLOT(onActivated(SubscriptionTab*)));
    connect(sTab,SIGNAL(subscriptionTabPressed(QPoint)),this,SLOT(onSubscriptionTabPressed(QPoint)));
    connect(sTab,SIGNAL(subscriptionTabMoved(QPoint)),this,SLOT(onSubscriptionTabMoved(QPoint)));
}

void TabBar::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
}

void TabBar::  onActivated(SubscriptionTab *activatedTab)
{
    if(currentTab!=NULL)
    {
        currentTab->deactivate();
    }

    currentTab=activatedTab;
}


void TabBar::onSubscriptionTabPressed(QPoint mousePos)
{
    currentSubscriptionTabOffset=mousePos.y();
}

void TabBar::onSubscriptionTabMoved(QPoint mousePos)
{
    int offset=mousePos.y()-currentSubscriptionTabOffset;


    subscriptionTabOffset+=offset;

    if(subscriptionTabOffset<rect.height()-subscriptionTabList.size()*46)
        subscriptionTabOffset=rect.height()-subscriptionTabList.size()*46;
    else if(subscriptionTabOffset>0)
        subscriptionTabOffset=0;

    for(int i=0;i<subscriptionTabList.size();++i)
    {
        subscriptionTabList[i]->setPos(0,i*46+subscriptionTabOffset);
    }

    currentSubscriptionTabOffset=mousePos.y();
}

void TabBar::onBarDraggedTo(int pos)
{
    openCloseAnimationTimer.stop();

    if((pos-tabBarWidth+5)>0)
    {
        setPos(QPointF(0,0));
    }
    else if((pos-tabBarWidth+5)<-tabBarWidth+10.0f)
    {
        setPos(QPointF(-tabBarWidth+10.0f,0));
    }
    else
    {
        setPos(QPointF(pos-tabBarWidth+5,0));
    }
}

void TabBar::onBarReleased(enum TabBarHandle::Direction dir,int speed)
{
    openCloseAnimationTimer.stop();

    if(dir==TabBarHandle::OPEN)
    {
        openCloseAnimationTimer.setDuration(10000.0/(float)speed);

        openCloseAnimationTimer.setFrameRange(0,100);

        openCloseAnimation.setItem(this);

        openCloseAnimation.setTimeLine(&openCloseAnimationTimer);

        openCloseAnimation.setPosAt(0.0,QPointF(pos().x(),0.0f));

        openCloseAnimation.setPosAt(1.0,QPointF(0.0f,0.0f));

        currentTabBarHandleStatus=OPENNED;

    }
    else if(dir==TabBarHandle::CLOSE)
    {

        openCloseAnimationTimer.setDuration(10000.0/(float)-speed);

        openCloseAnimationTimer.setFrameRange(0,100);

        openCloseAnimation.setItem(this);

        openCloseAnimation.setTimeLine(&openCloseAnimationTimer);

        openCloseAnimation.setPosAt(0.0,QPointF(pos().x(),0.0f));

        openCloseAnimation.setPosAt(1.0,QPointF(-tabBarWidth+10,0.0f));

        currentTabBarHandleStatus=CLOSED;

    }

    else if(dir==TabBarHandle::NODIRECTION)
    {

        if(currentTabBarHandleStatus==CLOSED)
        {
            openCloseAnimationTimer.setDuration(2*tabBarWidth);

            openCloseAnimationTimer.setFrameRange(0,100);

            openCloseAnimation.setItem(this);

            openCloseAnimation.setTimeLine(&openCloseAnimationTimer);

            openCloseAnimation.setPosAt(0.0,QPointF(pos().x(),0.0f));

            openCloseAnimation.setPosAt(1.0,QPointF(0.0f,0.0f));

            currentTabBarHandleStatus=OPENNED;
        }
        else if(currentTabBarHandleStatus==OPENNED)
        {
            openCloseAnimationTimer.setDuration(2*tabBarWidth);

            openCloseAnimationTimer.setFrameRange(0,100);

            openCloseAnimation.setItem(this);

            openCloseAnimation.setTimeLine(&openCloseAnimationTimer);

            openCloseAnimation.setPosAt(0.0,QPointF(pos().x(),0.0f));

            openCloseAnimation.setPosAt(1.0,QPointF(-tabBarWidth+10,0.0f));

            currentTabBarHandleStatus=CLOSED;
        }
    }

    openCloseAnimationTimer.start();
}


void TabBar::wheelEvent ( QGraphicsSceneWheelEvent * event )
{
    int offset=event->delta();


    subscriptionTabOffset-=offset;

    if(subscriptionTabOffset<rect.height()-subscriptionTabList.size()*46)
        subscriptionTabOffset=rect.height()-subscriptionTabList.size()*46;
    else if(subscriptionTabOffset>0)
        subscriptionTabOffset=0;

    for(int i=0;i<subscriptionTabList.size();++i)
    {
        subscriptionTabList[i]->setPos(0,i*46+subscriptionTabOffset);
    }
}
