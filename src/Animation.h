#ifndef ANIMATION_H
#define ANIMATION_H
#include <QString>
#include <vector>
#include <QVector2D>

struct AnimationFrame
{
    AnimationFrame(QString frameName, QVector2D framePosition, QVector2D frameSize) :
        m_frameName(frameName),
        m_framePosition(framePosition),
        m_frameSize(frameSize)
    {

    }

    QString   m_frameName;
    QVector2D m_framePosition;
    QVector2D m_frameSize;
};

class Animation
{
public:
    Animation(QString animationName) : m_animationName(animationName), m_nullAnimation(QString(""), QVector2D(0, 0), QVector2D(0, 0)) {  }

    void setAnimationName(QString animationName) { m_animationName = animationName; }
    const QString &getAnimationName() { return m_animationName; }

    void addFrame(AnimationFrame newFrame) { m_animationFrames.push_back(newFrame); }


    const AnimationFrame &getFrame(QString frameName)
    {
        auto foundFrame = std::find_if(m_animationFrames.begin(), m_animationFrames.end(), [frameName](const AnimationFrame &animationFrame)
        {
            return animationFrame.m_frameName == frameName;
        });

        if(foundFrame != m_animationFrames.end())
            return *foundFrame;
        else
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
    QString m_animationName;
    std::vector<AnimationFrame> m_animationFrames;
    AnimationFrame m_nullAnimation;
};




#endif // ANIMATION_H
