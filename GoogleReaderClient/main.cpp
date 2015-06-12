#include <QtGui/QApplication>
#include "mainwindow.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    DatabaseManager::getSingleton().open();

    //DatabaseManager::getSingleton().dropEveryThing();
    DatabaseManager::getSingleton().needFirstTimeInitialization();

  // exit(0);
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();

    return a.exec();
}
