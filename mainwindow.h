#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "feed.h"
#include <QGraphicsScene>
#include <QVBoxLayout>
#include "subscriptiontab.h"
#include "fakepage.h"
#include "tabbar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
QGraphicsScene *scene;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void showEvent ( QShowEvent * event );

    void fillAPage();

    QVector<Feed> feedList;

 //   SubscriptionTab *sTab;
FakePage *fakepage;
public slots:
   void onGo();
   void onLoad();
   void onFeedListUpdated(QVector<Feed>&);
   void onGraphicsViewResized(int width,int height);

};

#endif // MAINWINDOW_H
