#include "tile.h"
#include <QDomDocument>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QFile>
#include <QFontMetrics>




void Tile::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget  )
{
    QRectF rectangle(4.0, 4.0, rect.width()-8, rect.height()-8);

    painter->setBrush(Qt::white);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(rectangle, 8.0, 8.0);

    painter->setPen(QColor::fromRgb(0x20ACB8));
    titleLayout.draw(painter, QPoint(0, 0));
    painter->setPen(Qt::black);
    contentLayout.draw(painter,QPoint(0,0));

    if(layoutType==TOP)
    {
        painter->drawImage((rect.width()-teaser.width())*0.5,10,teaser);
    }
    else if(layoutType==TOP_LEFT)
    {
        painter->drawImage(10,10,teaser);
    }
}


void Tile::initialize()
{
    if(feed.getImageSize())
    {
        teaser=feed.getImage(feed.getTeaserID());

        if(teaser.width()>rect.width()-20)
        {
            teaser=teaser.scaledToWidth(rect.width()-20,Qt::SmoothTransformation);
        }

        if(teaser.height()>0.8*(rect.height()-20))
            teaser=teaser.scaledToHeight((rect.height()-20)*0.8);

        if(teaser.width()>rect.width()-100)
        {
            layoutType=TOP;

            titleLayout.setText(feed.getTitle());
            QFontMetrics titleFontMetrics(titleLayout.font());

            int leading = titleFontMetrics.leading();
            qreal height = teaser.height()+10;
            titleLayout.beginLayout();
            while (height<(rect.height()-20))
            {
                QTextLine line = titleLayout.createLine();
                if (!line.isValid())
                    break;

                line.setLineWidth(rect.width()-20);
                height += leading;
                line.setPosition(QPointF(10, height));
                height += line.height();
            }
            titleLayout.endLayout();


            contentLayout.setText(feed.getContent());
            QFontMetrics contentFontMetrics(contentLayout.font());

            leading = contentFontMetrics.leading();
            contentLayout.beginLayout();
            while (height<rect.height()-20)
            {
                QTextLine line = contentLayout.createLine();
                if (!line.isValid())
                    break;

                line.setLineWidth(rect.width()-20);
                height += leading;
                line.setPosition(QPointF(10, height));
                height += line.height();
            }
            contentLayout.endLayout();



        }
        else
        {
            layoutType=TOP_LEFT;

            titleLayout.setText(feed.getTitle());
            QFontMetrics titleFontMetrics(titleLayout.font());

            int leading = titleFontMetrics.leading();
            qreal height = 10;
            titleLayout.beginLayout();
            while (height<rect.height()-20)
            {
                QTextLine line = titleLayout.createLine();
                if (!line.isValid())
                    break;

                line.setLineWidth(rect.width()-20-teaser.width()-5);
                height += leading;
                line.setPosition(QPointF(teaser.width()+10+5, height));
                height += line.height();
            }
            titleLayout.endLayout();


            contentLayout.setText(feed.getContent());
            QFontMetrics contentFontMetrics(contentLayout.font());

            leading = contentFontMetrics.leading();
            contentLayout.beginLayout();
            while (height<rect.height()-20)
            {
                QTextLine line = contentLayout.createLine();
                if (!line.isValid())
                    break;


                height += leading;

                if(height>teaser.height())
                {
                  line.setLineWidth(rect.width()-20);
                   line.setPosition(QPointF(10, height));
                }
                else
                {
                    line.setLineWidth(rect.width()-20-teaser.width()-5);
                    line.setPosition(QPointF(teaser.width()+10+5, height));
                }

                height += line.height();
            }
            contentLayout.endLayout();
        }
    }

    else
    {
        layoutType=TEXT_ONLY;

        titleLayout.setText(feed.getTitle());
        QFontMetrics titleFontMetrics(titleLayout.font());

        int leading = titleFontMetrics.leading();
        qreal height = 10;
        titleLayout.beginLayout();
        while (height<rect.height()-20)
        {
            QTextLine line = titleLayout.createLine();
            if (!line.isValid())
                break;

            line.setLineWidth(rect.width()-20);
            height += leading;
            line.setPosition(QPointF(10, height));
            height += line.height();
        }
        titleLayout.endLayout();


        contentLayout.setText(feed.getContent());
        QFontMetrics contentFontMetrics(contentLayout.font());

        leading = contentFontMetrics.leading();
        contentLayout.beginLayout();
        while (height<rect.height()-20)
        {
            QTextLine line = contentLayout.createLine();
            if (!line.isValid())
                break;

            line.setLineWidth(rect.width()-20);
            height += leading;
            line.setPosition(QPointF(10, height));
            height += line.height();
        }
        contentLayout.endLayout();
    }
}
