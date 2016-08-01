#ifndef DOCUMENTWRITER_H
#define DOCUMENTWRITER_H
#include <QString>
#include <vector>
#include "Animation.h"

class DocumentWriter
{
public:
    DocumentWriter() { }
    virtual ~DocumentWriter() { }
    virtual void writeToFile(QString documentPath, QVector<Animation> &animations) = 0;
};

class DocumentReader
{
public:
    DocumentReader() { }
    virtual ~DocumentReader() { }
    virtual QVector<Animation> readFromFile(QString documentPath) = 0;
};

#endif // DOCUMENTWRITER_H
