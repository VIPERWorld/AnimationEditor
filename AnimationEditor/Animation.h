#ifndef ANIMATION_H
#define ANIMATION_H
#include <QString>
#include <QVector>
#include <QVector2D>
#include <algorithm>
#include <QPixmap>

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
    Animation() = default;
    ~Animation();

    Animation(const Animation& other);
    Animation(Animation&& other);
    Animation& operator=(const Animation& other);
    //Animation name
    void setAnimationName(QString animationName) { m_animationName = animationName; }
    const QString &getAnimationName() const { return m_animationName; }
    //Frames
    QVector<AnimationFrame> &getFrames() { return m_animationFrames; }
    void addFrame(AnimationFrame newFrame) { m_animationFrames.push_back(newFrame); }
    AnimationFrame &getFrame(QString frameName);
    //Spritesheet
    QPixmap *getSpritesheet() { return m_spriteSheet; }
    bool loadSpriteSheetFromFile(QString path);
private:
    QString m_animationName{"Animation"};
    QVector<AnimationFrame> m_animationFrames;
    //Null animation returned when frame not found
    static AnimationFrame m_nullAnimationFrame;
    QString   m_spriteSheetPath{""};
    QPixmap  *m_spriteSheet{nullptr};
};


#endif // ANIMATION_H
