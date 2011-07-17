#ifndef LAYOUTTREENODE_H
#define LAYOUTTREENODE_H

#include "layoutpanelrect.h"
#include "feed.h"

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

#endif // LAYOUTTREENODE_H
