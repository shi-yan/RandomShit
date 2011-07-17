#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView:public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent=0):QGraphicsView(parent){}

    void resizeEvent ( QResizeEvent * event );

    signals:

    void resized(int width,int height);
};

#endif // GRAPHICSVIEW_H
