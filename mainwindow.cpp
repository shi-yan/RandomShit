#include "mainwindow.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>
#include <QDebug>
#include "json/parser.h"
#include "googleoauthdialog.h"
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "googlereaderapi.h"
#include "tile.h"
#include "page.h"
#include <QScrollBar>
#include "databasemanager.h"
#include "fetcher.h"
#include "graphicsview.h"
#include "navigationbar.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent)
{
    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("MainWindow"));
    resize(1280, 768);

  //  setWindowFlags(windowFlags()| Qt::FramelessWindowHint);

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    graphicsView = new GraphicsView(centralWidget);
    graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
    graphicsView->setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);

    verticalLayout->addWidget(graphicsView);

    setCentralWidget(centralWidget);


    setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));

    QMetaObject::connectSlotsByName(this);
    scene=new QGraphicsScene(this);

    QBrush bgBrush(QColor::fromRgb(40,38,38));
    bgBrush.setTextureImage(QImage("/Users/billconan/Desktop/bg4.png"));
    scene->setBackgroundBrush(bgBrush);
    graphicsView->setScene(scene);

   // resize(1280,768);

    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    GoogleOauthDialog::getSingleton().setModal(true);

    connect(graphicsView,SIGNAL(resized(int,int)),this,SLOT(onGraphicsViewResized(int,int)));



   // scene->addItem(sTab=new SubscriptionTab());

   // sTab->setText("Qt Subscription");
   // sTab->setRect(QRect(0,0,200,80));


  //  sTab->setPos(1270,140);

    scene->addItem(&TabBar::getSingleton());

    TabBar::getSingleton().setTabBarWidth(200);

    TabBar::getSingleton().setHeight(720);


    scene->addItem(&NavigationBar::getSingleton());

    NavigationBar::getSingleton().setPos(-16,0);


    NavigationBar::getSingleton().setScreenSize(1280,720);


}


void MainWindow::onGraphicsViewResized(int width,int height)
{
    TabBar::getSingleton().setHeight(height);

    NavigationBar::getSingleton().setScreenSize(width,height);
}

void MainWindow::onLoad()
{
    GoogleReaderAPI::getSingleton().getReadList();
}

void MainWindow::onGo()
{

}

void MainWindow::showEvent ( QShowEvent * event )
{
    QMainWindow::showEvent(event);

    if(!GoogleOauthDialog::getSingleton().loadIdentity())
    {
        bool result=GoogleOauthDialog::getSingleton().exec();
        if(result)
        {
             //  Fetcher::getSingleton().getSubscriptionListFromServer();
            Fetcher::getSingleton().getSubscriptionListFromDatabase();
       //     Fetcher::getSingleton().startFetcher(5);
        }
    }
    else
    {
       Fetcher::getSingleton().getSubscriptionListFromDatabase();
      //    Fetcher::getSingleton().getSubscriptionListFromServer();
      //  Fetcher::getSingleton().startFetcher(5);
    }

//
    for(int i=0;i<Fetcher::getSingleton().getSubscriptionList().count();++i)
    {
        TabBar::getSingleton().addNewTab(Fetcher::getSingleton().getSubscriptionList()[i].getTitle());
    }

    //sTab->setPosition(50,50);

   // fillAPage();
}

void MainWindow::fillAPage()
{
    feedList=DatabaseManager::getSingleton().fetchFeedList(0,100);

    Page *p=new Page();


    p->updateSize(graphicsView->rect().top(),graphicsView->rect().bottom(),graphicsView->rect().left(),graphicsView->rect().right());

    qDebug()<<"page size: left:"<<graphicsView->rect().top()<<";"<<graphicsView->rect().bottom()<<";"<<graphicsView->rect().left()<<";"<<graphicsView->rect().right();

    int i=0;

    qDebug()<<"feed list size:"<<feedList.size();

    while(p->addFeed(&feedList[i]))
    {
        ++i;

        if(i>=feedList.size())
            break;
    }

    p->generateTiles(scene);

    scene->addItem(p);
}

void MainWindow:: onFeedListUpdated(QVector<Feed> &feedlist)
{
}

 //   p->generateTiles(scene);


    //scene->addItem(p);

  //  qDebug()<<"finish generating tiles";
//}

MainWindow::~MainWindow()
{
}
