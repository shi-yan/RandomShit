#ifndef PAGE_H
#define PAGE_H

#include <QGraphicsObject>
#include <QRect>
#include "feed.h"
#include <QGraphicsScene>
#include "layoutpanelrect.h"
#include "layouttreenode.h"

class Page:public QGraphicsObject
{
    int maximumFeedCount;

    LayoutTreeNode *layoutTree;
    void cleanLayoutTree( LayoutTreeNode *);
    QVector<Feed*> feedList;
    void assignFeedToLayoutPanels();
    void assignFeedToLayoutPanelsHelper(LayoutTreeNode *node);
    void computeLayoutTree(LayoutTreeNode *tree,LayoutPanelRect &);
    float easeTension(LayoutTreeNode *node);
    void recomputePanelSize(LayoutTreeNode *node);

    QRectF size;
    void gatherPanelSizes(LayoutTreeNode *node);

public:
    Page();
    ~Page();

    void updateSize(int top,int bottom,int left,int right);
    bool addFeed(Feed *aFeed);
    void generateTiles(QGraphicsScene *);

    QRectF boundingRect() const
    {
        return size;
    }

    QVector<LayoutTreeNode*> layoutTreeLeaves;
    void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);
};

#endif // PAGE_H
