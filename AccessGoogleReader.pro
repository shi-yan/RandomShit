#-------------------------------------------------
#
# Project created by QtCreator 2011-05-19T11:02:39
#
#-------------------------------------------------

QT       += core gui webkit network xml sql

TARGET = AccessGoogleReader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    json/serializerrunnable.cpp \
    json/serializer.cpp \
    json/qobjecthelper.cpp \
    json/parserrunnable.cpp \
    json/parser.cpp \
    json/json_scanner.cpp \
    json/json_parser.cc \
    googleoauthdialog.cpp \
    graphicsscene.cpp \
    googlereaderapi.cpp \
    feed.cpp \
    item.cpp \
    tile.cpp \
    page.cpp \
    fetcher.cpp \
    databasemanager.cpp \
    subscription.cpp \
    xmlerrorhandler.cpp \
    subscriptiontab.cpp \
    fakepage.cpp \
    graphicsview.cpp \
    tabbar.cpp \
    tabbarhandle.cpp \
    navigationbarbutton.cpp \
    navigationbar.cpp

HEADERS  += mainwindow.h \
    json/stack.hh \
    json/serializerrunnable.h \
    json/serializer.h \
    json/qobjecthelper.h \
    json/qjson_export.h \
    json/qjson_debug.h \
    json/position.hh \
    json/parserrunnable.h \
    json/parser_p.h \
    json/parser.h \
    json/location.hh \
    json/json_scanner.h \
    json/json_parser.hh \
    googleoauthdialog.h \
    graphicsscene.h \
    googlereaderapi.h \
    feed.h \
    item.h \
    tile.h \
    page.h \
    fetcher.h \
    databasemanager.h \
    subscription.h \
    xmlerrorhandler.h \
    subscriptiontab.h \
    fakepage.h \
    graphicsview.h \
    tabbar.h \
    tabbarhandle.h \
    navigationbarbutton.h \
    navigationbar.h

FORMS    +=

OTHER_FILES += \
    json/json_parser.yy
