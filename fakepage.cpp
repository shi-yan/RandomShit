#include "fakepage.h"
#include <QPainter>

void FakePage::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget  )
{
    QLinearGradient topLinearGrad(QPointF(0, 0), QPointF(0, 20));
    topLinearGrad.setColorAt(0, QColor::fromRgba(0x00000000));
    topLinearGrad.setColorAt(1, QColor::fromRgba(0x33000000));


    painter->fillRect(0,0,rect.width()-20,20,QBrush(topLinearGrad));

    QRadialGradient topRightRadialGrad(QPointF(rect.width()-20, 20), 20);
    topRightRadialGrad.setColorAt(0,  QColor::fromRgba(0x33000000));
    topRightRadialGrad.setColorAt(1, QColor::fromRgba(0x00000000));


    painter->fillRect(rect.width()-20,0,20,20,QBrush(topRightRadialGrad));

    QLinearGradient rightLinearGrad(QPointF(rect.width()-20, 0), QPointF(rect.width(), 0));
    rightLinearGrad.setColorAt(0,  QColor::fromRgba(0x33000000));
    rightLinearGrad.setColorAt(1,   QColor::fromRgba(0x00000000));


    painter->fillRect(rect.width()-20,20,20,rect.height()-40,QBrush(rightLinearGrad));

    QRadialGradient bottomRightRadialGrad(QPointF(rect.width()-20, rect.height()-20), 20);
    bottomRightRadialGrad.setColorAt(0,  QColor::fromRgba(0x33000000));
    bottomRightRadialGrad.setColorAt(1, QColor::fromRgba(0x00000000));
    painter->fillRect(rect.width()-20,rect.height()-20,20,20,QBrush(bottomRightRadialGrad));

    QLinearGradient bottomLinearGrad(QPointF(0, rect.height()-20), QPointF(0, rect.height()));
    bottomLinearGrad.setColorAt(0,  QColor::fromRgba(0x33000000));
    bottomLinearGrad.setColorAt(1, QColor::fromRgba(0x00000000));


    painter->fillRect(0,rect.height()-20,rect.width()-20,20,QBrush(bottomLinearGrad));


    QPainterPath tabPath;
    tabPath.moveTo(0,10);
    tabPath.lineTo(rect.width()-20,10);
    tabPath.arcTo(rect.width()-30,10,20,20,90,-90);
    tabPath.lineTo(rect.width()-10,rect.height()-20);
    tabPath.arcTo(rect.width()-30,rect.height()-30,20,20,0,-90);
    tabPath.lineTo(0,rect.height()-10);

    painter->setBrush(Qt::black);
    painter->fillPath(tabPath,QBrush(Qt::black));

}
