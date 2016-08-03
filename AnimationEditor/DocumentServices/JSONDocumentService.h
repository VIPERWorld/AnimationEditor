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
            for(auto animation : animations)
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
                else
                {
                    QJsonObject animationObject
                    {
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
        QFile file(documentPath);
        if(file.open(QIODevice::ReadOnly))
        {
            QString data = file.readAll();
            QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());
            QJsonObject animationsObject = document.object();
            QJsonArray animationsArray = animationsObject.value("Animations").toArray();
            for(auto i = 0; i <= animationsArray.size() ; ++i)
            {
                auto animationObject = animationsArray.at(i).toObject();
                animations.append(Animation());
                auto animation = animations.back();
                animation.setAnimationName(animationObject.value("name").toString());
                if(animationObject.contains("frames"))
                {
                    QJsonArray framesArray = animationObject.value("frames").toArray();
                    for (auto j = 0; j <= framesArray.size(); ++j)
                    {
                        AnimationFrame frame;
                        auto frameObject = framesArray.at(j).toObject();
                        frame.m_frameName = frameObject.value("name").toString();
                        frame.m_framePosition = {(float) frameObject.value("x").toDouble(),
                                                 (float) frameObject.value("y").toDouble()};
                        frame.m_frameSize = {(float) frameObject.value("width").toDouble(),
                                             (float) frameObject.value("height").toDouble()};
                        animation.addFrame(frame);
                    }
                }
            }
        }
        return animations;
    }
};

#endif // JSONDOCUMENTWRITER_H
