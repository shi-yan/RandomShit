#ifndef PAGE_H
#define PAGE_H

#include <QGraphicsObject>
#include <QRect>
#include "feed.h"
#include <QGraphicsScene>


class LayoutPanelRect
{
public:
    int top;
    int bottom;
    int left;
    int right;

    LayoutPanelRect(int _top,int _bottom,int _left,int _right):top(_top),bottom(_bottom),left(_left),right(_right)
    {

    }

    LayoutPanelRect():top(0),bottom(0),left(0),right(0)
    {
    }

    LayoutPanelRect(const LayoutPanelRect &in):top(in.top),bottom(in.bottom),left(in.left),right(in.right)
    {

    }

    void operator=(const LayoutPanelRect &in)
    {
        top=in.top;
        bottom=in.bottom;
        left=in.left;
        right=in.right;
    }
};

class LayoutTreeNode
{
public:
    enum SeperatorType
    {
        NONE,
        HORIZONTAL,
        VERTICAL,
    };

    LayoutPanelRect boundingBox;
    enum SeperatorType seperator;
    float seperatorPosition;

    LayoutTreeNode *n1;
    LayoutTreeNode *n2;

    LayoutTreeNode():n1(0),n2(0),seperator(NONE),seperatorPosition(0),feed(0)
    {
    }

    Feed *feed;


    LayoutTreeNode(const LayoutTreeNode &in):boundingBox(in.boundingBox),seperator(in.seperator),seperatorPosition(in.seperatorPosition),n1(in.n1),n2(in.n2),feed(in.feed)
    {

    }

    void operator=(const LayoutTreeNode &in)
    {
        boundingBox=in.boundingBox;
        seperator=in.seperator;
        seperatorPosition=in.seperatorPosition;
        n1=in.n1;
        n2=in.n2;
        feed=in.feed;
    }


    bool operator<(const LayoutTreeNode &n2)
    {
        int n1width=boundingBox.right-boundingBox.left;
        int n1height=boundingBox.bottom-boundingBox.top;
        int n2width=n2.boundingBox.right-n2.boundingBox.left;
        int n2height=n2.boundingBox.bottom-n2.boundingBox.top;

        return (n1width+n1height)<(n2width+n2height);
    }

    bool operator>(const LayoutTreeNode &n2)
    {
        int n1width=boundingBox.right-boundingBox.left;
        int n1height=boundingBox.bottom-boundingBox.top;
        int n2width=n2.boundingBox.right-n2.boundingBox.left;
        int n2height=n2.boundingBox.bottom-n2.boundingBox.top;

        return (n1width+n1height)>(n2width+n2height);
    }

    bool operator==(const LayoutTreeNode &n2)
    {
        int n1width=boundingBox.right-boundingBox.left;
        int n1height=boundingBox.bottom-boundingBox.top;
        int n2width=n2.boundingBox.right-n2.boundingBox.left;
        int n2height=n2.boundingBox.bottom-n2.boundingBox.top;

        return (n1width+n1height)==(n2width+n2height);
    }


};

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

    // QVector<QRect> debuglist;


    void paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*);

};

#endif // PAGE_H
