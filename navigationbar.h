#ifndef NAVIGATEBAR_H
#define NAVIGATEBAR_H

#include <QGraphicsObject>
#include "navigationbarbutton.h"
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QDebug>

class NavigationBar:public QGraphicsObject
{
    Q_OBJECT

    int navigationAreaWidth;
    int navigationAreaHeight;
    int navigationBarWidth;
    int screenWidth;

    QRect rect;

    NavigationBarButton *settingButton;
    NavigationBarButton *prePageButton;
    NavigationBarButton *nxtPageButton;
    QTimeLine openCloseAnimationTimer;
    QGraphicsItemAnimation openCloseAnimation;

    enum NavigationBarStatus
    {
        OPENNED,
        CLOSED,
    };

    enum NavigationBarStatus navigationBarStatus;

    NavigationBar(QGraphicsItem *parent=0):QGraphicsObject(parent),navigationAreaWidth(400),
        navigationBarWidth(16),navigationBarStatus(CLOSED)
    {
        settingButton=new NavigationBarButton(this);
        settingButton->setPos(0,0);
        prePageButton=new NavigationBarButton(this);
        prePageButton->setPos(0,navigationAreaHeight-100);
        nxtPageButton=new NavigationBarButton(this);
        nxtPageButton->setPos(0,navigationAreaHeight-50);

        connect(settingButton,SIGNAL(released()),this,SLOT(onSettingButtonClicked()));
    }

public:

    static NavigationBar & getSingleton()
    {
        static NavigationBar obj;
        return obj;
    }

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setScreenSize(const int _width,const int _height);

    QRectF boundingRect() const
    {
        return rect;
    }

     void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
     void mousePressEvent ( QGraphicsSceneMouseEvent * event );
     void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

public slots:
     void onSettingButtonClicked();
};

#endif // NAVIGATEBAR_H
