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
            {
                binWriter << animation.getAnimationName();
                int numOfFrames = animation.getFrames().size();
                binWriter << numOfFrames;
                for(auto const &frame : animation.getFrames())
                {
                    binWriter << frame.m_frameName;
                    binWriter << frame.m_framePosition.x();
                    binWriter << frame.m_framePosition.y();
                    binWriter << frame.m_frameSize.x();
                    binWriter << frame.m_frameSize.y();
                }
            }
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
            int numOfAnimations = 0;
            binReader >> numOfAnimations;
            for (auto i = 0; i <= numOfAnimations; ++i)
            {
                Animation animation;
                QString animationName;
                binReader >> animationName;
                animation.setAnimationName(animationName);
                int numOfFrames = 0;
                binReader >> numOfFrames;
                for(auto i = 0; i <= numOfFrames; ++i)
                {
                    AnimationFrame frame;
                    binReader >> frame.m_frameName;

                    float posX = 0;
                    float posY = 0;
                    float width = 0;
                    float height = 0;
                    binReader >> posX;
                    binReader >> posY;
                    binReader >> width;
                    binReader >> height;

                    frame.m_framePosition = {posX, posY};
                    frame.m_frameSize = {width, height};
                    animation.addFrame(frame);
                }
                animations.append(animation);
            }
        }
        return animations;
    }
};

#endif // AEPDOCUMENTSERVICE_H
