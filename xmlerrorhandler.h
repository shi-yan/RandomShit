#ifndef XMLERRORHANDLER_H
#define XMLERRORHANDLER_H
#include <QXmlErrorHandler>
#include <QDebug>

class XmlErrorHandler:public QXmlErrorHandler
{
public:
    XmlErrorHandler();


    ~XmlErrorHandler(){}

    virtual bool warning(const QXmlParseException&e)
    {
        qDebug()<<"Warning:"<<e.message()<<" at line:"<<e.lineNumber();

        return true;
    }

    virtual bool error(const QXmlParseException&e)
    {
        qDebug()<<"Error:"<<e.message()<<" at line:"<<e.lineNumber();
        return true;
    }

    virtual bool fatalError(const QXmlParseException&e)
    {
        qDebug()<<"Fatal Error:"<<e.message()<<" at line:"<<e.lineNumber();
        qDebug()<<"System id:"<<e.systemId()<<" Public id:"<<e.publicId();
        return true;
    }

    virtual QString errorString() const
    {

    }
};

#endif // XMLERRORHANDLER_H
