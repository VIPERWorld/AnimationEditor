#ifndef XMLDOCUMENTSERVICE_H
#define XMLDOCUMENTSERVICE_H
#include "DocumentService.h"

class XMLDocumentWriter : virtual public DocumentWriter
{
public:
    XMLDocumentWriter() { }
    virtual ~XMLDocumentWriter() { }
    virtual void writeToFile(QString documentPath, std::vector<Animation> &animations) override;
};

class XMLDocumentReader : virtual public DocumentReader
{
public:
    XMLDocumentReader() { }
    virtual ~XMLDocumentReader() { }
    virtual std::vector<Animation> readFromFile(QString documentPath) override;
};

#endif // XMLDOCUMENTSERVICE_H
