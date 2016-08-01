#ifndef XMLDOCUMENTSERVICE_H
#define XMLDOCUMENTSERVICE_H

#include <QtCore/QXmlStreamWriter>
#include "../DocumentService.h"
#include <QString>

class XMLDocumentWriter : virtual public DocumentWriter
{
public:
    XMLDocumentWriter() { }
    virtual ~XMLDocumentWriter() { }
    virtual void writeToFile(QString documentPath, QVector<Animation> &animations) override
    {
        QFile file(documentPath);
        if(file.open(QIODevice::WriteOnly))
        {
            QXmlStreamWriter writer(&file);
            writer.setAutoFormatting(true);
            writer.writeStartDocument();
            //<Animations>
            writer.writeStartElement("Animations");
            for(auto const &animation : animations)
            {
                //<Animation>
                writer.writeStartElement("Animation");
                //Animation name
                writer.writeAttribute("name", animation.getAnimationName());
                for(auto const & frame : animation.getFrames())
                {
                    //<Frame>
                    writer.writeStartElement("Frame");
                    writer.writeAttribute("name", frame.m_frameName);
                    writer.writeAttribute("x", QString::number(frame.m_framePosition.x()));
                    writer.writeAttribute("y", QString::number(frame.m_framePosition.y()));
                    writer.writeAttribute("width", QString::number(frame.m_frameSize.x()));
                    writer.writeAttribute("height", QString::number(frame.m_frameSize.y()));
                    writer.writeEndElement();
                }
                writer.writeEndElement();
            }
            writer.writeEndElement();
            writer.writeEndDocument();
        }
    }
};

class XMLDocumentReader : virtual public DocumentReader
{
public:
    XMLDocumentReader() { }
    virtual ~XMLDocumentReader() { }
    virtual QVector<Animation> readFromFile(QString documentPath) override
    {
        QVector<Animation> animations;
        QFile file(documentPath);
        if(file.open(QIODevice::ReadOnly))
        {
            QXmlStreamReader reader;
            reader.setDevice(&file);
            while(reader.readNextStartElement())
            {
                if(reader.name() == "Animations")
                {
                    while(reader.readNextStartElement())
                    {
                        if(reader.name() == "Animation")
                        {
                            animations.append(Animation());
                            animations.back().setAnimationName(reader.attributes().value("name").toString());
                            while(reader.readNextStartElement())
                            {
                                if(reader.name() == "Frame")
                                {
                                    AnimationFrame frame;
                                    frame.m_frameName = reader.attributes().value("name").toString();
                                    frame.m_framePosition = { reader.attributes().value("x").toFloat(), reader.attributes().value("y").toFloat() };
                                    frame.m_frameSize = { reader.attributes().value("width").toFloat(), reader.attributes().value("height").toFloat() };
                                    animations.back().addFrame(frame);
                                }
                                else
                                    reader.skipCurrentElement();
                            }
                        }
                        else
                            reader.skipCurrentElement();
                    }
                }
            }
        }
        return animations;
    }
};

#endif // XMLDOCUMENTSERVICE_H
