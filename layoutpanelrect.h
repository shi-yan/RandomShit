#ifndef LAYOUTPANELRECT_H
#define LAYOUTPANELRECT_H

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
#endif // LAYOUTPANELRECT_H
