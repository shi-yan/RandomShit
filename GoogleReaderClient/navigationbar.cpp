#include "navigationbar.h"
#include <QPainter>

void NavigationBar::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->fillRect(rect,QBrush(Qt::black));
}

void NavigationBar::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{

}

void NavigationBar::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{

}

void NavigationBar::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{

}

void NavigationBar::onSettingButtonClicked()
{
    openCloseAnimationTimer.stop();
    if(navigationBarStatus==CLOSED)
    {
        navigationBarStatus=OPENNED;
        int duration=5*(screenWidth-16-pos().x());

        openCloseAnimationTimer.setDuration(duration);
        openCloseAnimationTimer.setFrameRange(0,100);
        openCloseAnimation.setItem(this);
        openCloseAnimation.setTimeLine(&openCloseAnimationTimer);
        openCloseAnimation.setPosAt(0.0,QPointF(pos().x(),0.0f));
        openCloseAnimation.setPosAt(1.0,QPointF(screenWidth-navigationAreaWidth,0.0f));
    }
    else if(navigationBarStatus==OPENNED)
    {
        navigationBarStatus=CLOSED;

        int duration=5*(screenWidth-16-pos().x());

        openCloseAnimationTimer.setDuration(duration);
        openCloseAnimationTimer.setFrameRange(0,100);
        openCloseAnimation.setItem(this);
        openCloseAnimation.setTimeLine(&openCloseAnimationTimer);
        openCloseAnimation.setPosAt(0.0,QPointF(pos().x(),0.0f));
        openCloseAnimation.setPosAt(1.0,QPointF(screenWidth-16,0.0f));
    }

    openCloseAnimationTimer.start();
}

void NavigationBar::setScreenSize(const int _width,const int _height)
{
    openCloseAnimationTimer.stop();
    setPos(QPointF(_width-(screenWidth-pos().x()),0));
    screenWidth=_width;
    rect=QRect(0,0,navigationAreaWidth,navigationAreaHeight=_height);
    settingButton->setPos(0,0);
    prePageButton->setPos(0,navigationAreaHeight-100);
    nxtPageButton->setPos(0,navigationAreaHeight-50);

    if(pos().x()<screenWidth-16 && navigationBarStatus==CLOSED)
    {
        int duration=5*(screenWidth-16-pos().x());
        openCloseAnimationTimer.setDuration(duration);
        openCloseAnimationTimer.setFrameRange(0,100);
        openCloseAnimation.setItem(this);
        openCloseAnimation.setTimeLine(&openCloseAnimationTimer);
        openCloseAnimation.setPosAt(0.0,QPointF(pos().x(),0.0f));
        openCloseAnimation.setPosAt(1.0,QPointF(screenWidth-16,0.0f));
        openCloseAnimationTimer.start();
    }
    else if(pos().x()>(screenWidth-navigationAreaWidth) && navigationBarStatus==OPENNED)
    {
        int duration=5*(pos().x()-screenWidth+navigationAreaWidth);
        openCloseAnimationTimer.setDuration(duration);
        openCloseAnimationTimer.setFrameRange(0,100);
        openCloseAnimation.setItem(this);
        openCloseAnimation.setTimeLine(&openCloseAnimationTimer);
        openCloseAnimation.setPosAt(0.0,QPointF(pos().x(),0.0f));
        openCloseAnimation.setPosAt(1.0,QPointF(screenWidth-navigationAreaWidth,0.0f));
        openCloseAnimationTimer.start();
    }
}
