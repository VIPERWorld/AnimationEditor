#ifndef AEPDOCUMENTSERVICE_H
#define AEPDOCUMENTSERVICE_H
#include "../DocumentService.h"

class AEPDocumentWriter : virtual public DocumentWriter
{
public:
    AEPDocumentWriter() { }
    virtual ~AEPDocumentWriter() { }
    virtual void writeToFile(QString documentPath, QVector<Animation> &animations) override;
};

class AEPDocumentReader : virtual public DocumentReader
{
public:
    AEPDocumentReader() { }
    virtual ~AEPDocumentReader() { }
    virtual QVector<Animation> readFromFile(QString documentPath) override;
};

#endif // AEPDOCUMENTSERVICE_H
