#ifndef AEPDOCUMENTSERVICE_H
#define AEPDOCUMENTSERVICE_H
#include "../DocumentService.h"
#include <QFile>

class AEPDocumentWriter : virtual public DocumentWriter
{
public:
    AEPDocumentWriter() { }
    virtual ~AEPDocumentWriter() { }
    virtual void writeToFile(QString documentPath, QVector<Animation> &animations) override
    {
        QFile file(documentPath);
        if(file.open(QIODevice::WriteOnly))
        {
            QDataStream binWriter(&file);
            int numOfAnimations = animations.size();
            binWriter << numOfAnimations;
            for(auto const &animation : animations)
                binWriter << animation.getAnimationName();
        }
    }
};

class AEPDocumentReader : virtual public DocumentReader
{
public:
    AEPDocumentReader() { }
    virtual ~AEPDocumentReader() { }
    virtual QVector<Animation> readFromFile(QString documentPath) override
    {
        QVector<Animation> animations;
        QFile file(documentPath);
        if(file.open(QIODevice::ReadOnly))
        {
            QDataStream binReader(&file);
            int numOfAnimations;
            binReader >> numOfAnimations;
            for (auto i = 0; i <= numOfAnimations; i++)
            {
                animations.append(Animation());
                QString animationName;
                binReader >> animationName;
                animations.back().setAnimationName(animationName);
            }
        }
        return animations;
    }
};

#endif // AEPDOCUMENTSERVICE_H
