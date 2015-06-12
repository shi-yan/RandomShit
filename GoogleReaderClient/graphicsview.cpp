#include "graphicsview.h"
#include <QResizeEvent>

void GraphicsView::resizeEvent ( QResizeEvent * event )
{
    scene()->setSceneRect(0,0,event->size().width(),event->size().height());

    emit resized(event->size().width(),event->size().height());
}
