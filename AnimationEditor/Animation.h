#ifndef ANIMATION_H
#define ANIMATION_H
#include <QString>
#include <QVector>
#include <QVector2D>
#include <algorithm>

struct AnimationFrame
{
    AnimationFrame() { }
    AnimationFrame(QString frameName, QVector2D framePosition, QVector2D frameSize) :
        m_frameName(frameName),
        m_framePosition(framePosition),
        m_frameSize(frameSize)
    {

    }
    AnimationFrame(const AnimationFrame &) = default;

    QString   m_frameName{""};
    QVector2D m_framePosition{QVector2D(0, 0)};
    QVector2D m_frameSize{QVector2D(0, 0)};
};

class Animation
{
public:
    void setAnimationName(QString animationName) { m_animationName = animationName; }
    const QString &getAnimationName() const { return m_animationName; }

    void addFrame(AnimationFrame newFrame) { m_animationFrames.push_back(newFrame); }

    const AnimationFrame &getFrame(QString frameName)
    {
        for(auto &frame : m_animationFrames)
        {
            if(frame.m_frameName == frameName)
                return frame;
        }
        return m_nullAnimation;
    }

    void deleteFrame(QString frameName)
    {
        auto foundFrame = std::find_if(m_animationFrames.begin(), m_animationFrames.end(), [frameName](const AnimationFrame &animationFrame)
        {
            return animationFrame.m_frameName == frameName;
        });

        if(foundFrame != m_animationFrames.end())
            m_animationFrames.erase(foundFrame);
    }
private:
    QString m_animationName{"Animation"};
    QVector<AnimationFrame> m_animationFrames;
    AnimationFrame m_nullAnimation;
};


#endif // ANIMATION_H
