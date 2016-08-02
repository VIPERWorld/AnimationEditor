#ifndef JSONDOCUMENTWRITER_H
#define JSONDOCUMENTWRITER_H
#include "../DocumentService.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector>
#include <QString>
#include "../Animation.h"

class JSONDocumentWriter : virtual public DocumentWriter
{
public:
    JSONDocumentWriter() { }
    virtual ~JSONDocumentWriter() { }
    virtual void writeToFile(QString documentPath, QVector<Animation> &animations) override
    {
        QFile file(documentPath);
        if(file.open(QIODevice::WriteOnly))
        {
            QJsonArray animationsArray;
            for(auto const &animation : animations)
            {
                if(animation.getFrames().size() > 0)
                {
                    QJsonArray animationFrames;
                    for (auto const &frame : animation.getFrames())
                    {
                        QJsonValue name = frame.m_frameName;
                        QJsonValue posX = frame.m_framePosition.x();
                        QJsonValue posY = frame.m_framePosition.y();
                        QJsonValue width = frame.m_frameSize.x();
                        QJsonValue height = frame.m_frameSize.y();
                        QJsonObject frameObject
                        {
                                {"name",   name},
                                {"x",      posX},
                                {"y",      posY},
                                {"width",  width},
                                {"height", height}
                        };
                        animationFrames.append(frameObject);
                    }
                    QJsonObject animationObject
                    {
                            {"frames", animationFrames},
                            {"name",   animation.getAnimationName()}
                    };
                    animationsArray.append(animationObject);
                }
            }
            QJsonObject animationsObject
            {
                    {"Animations", animationsArray}
            };
            QJsonDocument document = QJsonDocument::fromVariant(animationsObject.toVariantMap());
            file.write(document.toJson(QJsonDocument::Indented));
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
