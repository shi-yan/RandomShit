#include "page.h"
#include <QDebug>
#include "tile.h"
#include <QPainter>
#include <QTime>
static int feedListID=0;

Page::Page():layoutTree(0),QGraphicsObject()
{
}

Page::~Page()
{
    if(layoutTree)
        cleanLayoutTree(layoutTree);
}

void Page::computeLayoutTree(LayoutTreeNode *tree,LayoutPanelRect &bbox)
{
    srand(QTime::currentTime().hour()+QTime::currentTime().minute()+QTime::currentTime().second());

    tree->boundingBox=bbox;

    int width=tree->boundingBox.right-tree->boundingBox.left;
    int height=tree->boundingBox.bottom-tree->boundingBox.top;

    int area=width*height;

    if(area>100000)
    {

        float ratio = (float)rand()/(float)RAND_MAX;
        ratio*=0.4;
        ratio+=0.3;


        tree->seperatorPosition=ratio;

        if(width>height)
        {
            tree->seperator=LayoutTreeNode::HORIZONTAL;

             LayoutPanelRect n1Rect(tree->boundingBox.top,tree->boundingBox.bottom,tree->boundingBox.left,(int)(tree->boundingBox.left+width*tree->seperatorPosition));
             LayoutPanelRect n2Rect(tree->boundingBox.top,tree->boundingBox.bottom,n1Rect.right,tree->boundingBox.right);

             tree->n1=new  LayoutTreeNode();
             tree->n2=new  LayoutTreeNode();

             computeLayoutTree(tree->n1,n1Rect);
             computeLayoutTree(tree->n2,n2Rect);
        }
        else
        {

            tree->seperator=LayoutTreeNode::VERTICAL;

             LayoutPanelRect n1Rect(tree->boundingBox.top,(tree->boundingBox.top+height*tree->seperatorPosition),tree->boundingBox.left,tree->boundingBox.right);
             LayoutPanelRect n2Rect(n1Rect.bottom,tree->boundingBox.bottom,tree->boundingBox.left,tree->boundingBox.right);

             tree->n1=new  LayoutTreeNode();
             tree->n2=new  LayoutTreeNode();

             computeLayoutTree(tree->n1,n1Rect);
             computeLayoutTree(tree->n2,n2Rect);
        }
    }
    else
    {
        maximumFeedCount++;

        layoutTreeLeaves.push_back(tree);

    //    debuglist.push_back( QRect(tree->boundingBox.left,tree->boundingBox.top,tree->boundingBox.right-tree->boundingBox.left,tree->boundingBox.bottom-tree->boundingBox.top));
    }
}

void Page::updateSize(int top,int bottom,int left,int right)
{
    LayoutPanelRect startRect(top,bottom,left,right);

    if(layoutTree)
        cleanLayoutTree(layoutTree);

    layoutTree=new  LayoutTreeNode();
    maximumFeedCount=0;

    computeLayoutTree(layoutTree,startRect);

    qDebug()<<"maximum feed count:"<<maximumFeedCount;

    size.setX(left);
    size.setY(top);
    size.setWidth(right-left);
    size.setHeight(bottom-top);
}

bool Page::addFeed(Feed *aFeed)
{
    if(feedList.size()<maximumFeedCount)
    {
        feedList.push_back(aFeed);

        return true;
    }
    else
        return false;
}

void Page::assignFeedToLayoutPanelsHelper(LayoutTreeNode *node)
{
    if(node->seperator!=LayoutTreeNode::NONE)
    {
        assignFeedToLayoutPanelsHelper(node->n1);
        assignFeedToLayoutPanelsHelper(node->n2);
    }
    else
    {
        node->feed=feedList[feedListID++];
    }
}

bool layoutTreeNodeLessThan(LayoutTreeNode *n1, LayoutTreeNode *n2)
{
    int n1width=n1->boundingBox.right-n1->boundingBox.left;
    int n1height=n1->boundingBox.bottom-n1->boundingBox.top;
    int n2width=n2->boundingBox.right-n2->boundingBox.left;
    int n2height=n2->boundingBox.bottom-n2->boundingBox.top;

    return (n1width+n1height)<(n2width+n2height);
}

bool feedLessThan(Feed *n1, Feed *n2)
{

    return n1->getContentSize()<n2->getContentSize();
}


void Page::assignFeedToLayoutPanels()
{
    feedListID=0;

    //assignFeedToLayoutPanelsHelper(layoutTree);

    qSort(layoutTreeLeaves.begin(),layoutTreeLeaves.end(),layoutTreeNodeLessThan);

    qSort(feedList.begin(),feedList.end(),feedLessThan);

    if(feedList.size()!=layoutTreeLeaves.size())
        qDebug()<<"Error: feed size is not equal to layout panel size!!";

    else
        for(int i=0;i<feedList.size();++i)
        {
            layoutTreeLeaves[i]->feed=feedList[i];
    }
}


float Page::easeTension(LayoutTreeNode *node)
{
    if(node->seperator!=LayoutTreeNode::NONE)
    {
        float tension1=easeTension(node->n1);
        float tension2=easeTension(node->n2);

        qDebug()<<"tension 1"<<tension1<<"tension 2"<<tension2<<"seperator"<<node->seperatorPosition;

        node->seperatorPosition=0.5*(node->seperatorPosition+tension1/(tension1+tension2));

        qDebug()<<"adjusted position:"<<node->seperatorPosition;

        return tension1+tension2;
    }
    else
    {

        float width=node->boundingBox.right-node->boundingBox.left;
        float height=node->boundingBox.bottom-node->boundingBox.top;

        float area=(width)*(height);

        float tension=node->feed->getContentSize()/area;

        if(width<100 || height<100)
            tension+=1.0;

     //   qDebug()<<"tension:"<<tension;

        return tension;
    }
}

void Page::recomputePanelSize(LayoutTreeNode *node)
{
    if(node->seperator==LayoutTreeNode::HORIZONTAL)
    {
        node->n1->boundingBox=LayoutPanelRect(node->boundingBox.top,node->boundingBox.bottom,node->boundingBox.left,node->boundingBox.left+(node->boundingBox.right-node->boundingBox.left)*node->seperatorPosition);
        node->n2->boundingBox=LayoutPanelRect(node->boundingBox.top,node->boundingBox.bottom,node->n1->boundingBox.right,node->boundingBox.right);

        recomputePanelSize(node->n1);
        recomputePanelSize(node->n2);
    }
    else if(node->seperator==LayoutTreeNode::VERTICAL)
    {
        node->n1->boundingBox=LayoutPanelRect(node->boundingBox.top,node->boundingBox.top+(node->boundingBox.bottom-node->boundingBox.top)*node->seperatorPosition,node->boundingBox.left,node->boundingBox.right );
        node->n2->boundingBox=LayoutPanelRect(node->n1->boundingBox.bottom,node->boundingBox.bottom,node->boundingBox.left,node->boundingBox.right);

        recomputePanelSize(node->n1);
        recomputePanelSize(node->n2);
    }
}

void Page::cleanLayoutTree(LayoutTreeNode *node)
{
    if(node->seperator!=LayoutTreeNode::NONE)
    {
        cleanLayoutTree(node->n1);
        cleanLayoutTree(node->n2);
    }

    delete node;
}
void Page::paint(QPainter *p,const QStyleOptionGraphicsItem*,QWidget*)
{
    /*for(int i=0;i<debuglist.size();++i)
    {
        p->setBrush(Qt::NoBrush);

        int ff=rand()%6;

        switch(ff)
        {
            case 0:  p->setPen(Qt::black);break;
        case 1:  p->setPen(Qt::red);break;
        case 2:  p->setPen(Qt::green);break;
        case 3:  p->setPen(Qt::blue);break;
        case 4:  p->setPen(Qt::white);break;
        case 5:  p->setPen(Qt::yellow);break;
        }


        p->drawRect(debuglist[i]);
    }*/
}

void Page::gatherPanelSizes(LayoutTreeNode *node)
{
    if(node->seperator==LayoutTreeNode::NONE)
    {
        //debuglist.push_back( QRect(node->boundingBox.left,node->boundingBox.top,node->boundingBox.right-node->boundingBox.left,node->boundingBox.bottom-node->boundingBox.top));
    }
    else
    {
        gatherPanelSizes(node->n1);
        gatherPanelSizes(node->n2);
    }
}

void Page::generateTiles(QGraphicsScene *scene)
{
    assignFeedToLayoutPanels();

   /* for(int i=0;i<8;++i){
    qDebug()<<"Tension 1"<<easeTension(layoutTree);
    recomputePanelSize(layoutTree);
 }*/

  //  gatherPanelSizes(layoutTree);


    for(int i=0;i<layoutTreeLeaves.size();++i)
    {
        Tile* tile=new Tile();
        tile->setFeed(*(layoutTreeLeaves[i]->feed));

        QRect r(0,0,layoutTreeLeaves[i]->boundingBox.right-layoutTreeLeaves[i]->boundingBox.left,layoutTreeLeaves[i]->boundingBox.bottom-layoutTreeLeaves[i]->boundingBox.top);
        tile->setRect(r);

        tile->initialize();

        tile->setPos(layoutTreeLeaves[i]->boundingBox.left,layoutTreeLeaves[i]->boundingBox.top);
        scene->addItem(tile);

    }

}
