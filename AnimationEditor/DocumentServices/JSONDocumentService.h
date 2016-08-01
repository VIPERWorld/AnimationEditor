#ifndef JSONDOCUMENTWRITER_H
#define JSONDOCUMENTWRITER_H
#include "../DocumentService.h"

class JSONDocumentWriter : virtual public DocumentWriter
{
public:
    JSONDocumentWriter() { }
    virtual ~JSONDocumentWriter() { }
    virtual void writeToFile(QString documentPath, QVector<Animation> &animations) override
    {
        QFile doc(documentPath);
        if(doc.open(QIODevice::WriteOnly))
        {

        }
    }
};

class JSONDocumentReader : virtual public DocumentReader
{
public:
    JSONDocumentReader() { }
    virtual ~JSONDocumentReader() { }
    virtual QVector<Animation> readFromFile(QString documentPath) override
    {
        QVector<Animation> animations;
        QFile doc(documentPath);
        if(doc.open(QIODevice::ReadOnly))
        {

        }
        
        return animations;
    }
};

#endif // JSONDOCUMENTWRITER_H
