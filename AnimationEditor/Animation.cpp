#include "Animation.h"
AnimationFrame Animation::m_nullAnimationFrame;

Animation::~Animation()
{
    if(m_spriteSheet)
        delete m_spriteSheet;
}

Animation::Animation(const Animation &other)
{
    m_animationFrames = other.m_animationFrames;
    m_animationName = other.m_animationName;
    if(!other.m_spriteSheetPath.isEmpty())
    {
        m_spriteSheetPath = other.m_spriteSheetPath;
        m_spriteSheet = new QPixmap();
        m_spriteSheet->load(m_spriteSheetPath);
    }
    else
        m_spriteSheet = nullptr;
}

Animation::Animation(Animation &&other)
{
    m_animationFrames = other.m_animationFrames;
    m_animationName = other.m_animationName;
    if(!other.m_spriteSheetPath.isEmpty())
    {
        m_spriteSheetPath = other.m_spriteSheetPath;
        m_spriteSheet = new QPixmap();
        m_spriteSheet->load(m_spriteSheetPath);
    }
    else
        m_spriteSheet = nullptr;
}

Animation& Animation::operator=(const Animation &other)
{
    {
        m_animationFrames = other.m_animationFrames;
        m_animationName = other.m_animationName;
        if(!other.m_spriteSheetPath.isEmpty())
        {
            m_spriteSheetPath = other.m_spriteSheetPath;
            m_spriteSheet = new QPixmap();
            m_spriteSheet->load(m_spriteSheetPath);
        }
        else
            m_spriteSheet = nullptr;
        return *this;
    }
}

AnimationFrame& Animation::getFrame(QString frameName)
{
    for(auto &frame : m_animationFrames)
    {
        if(frame.m_frameName == frameName)
            return frame;
    }
    return m_nullAnimationFrame;
}

bool Animation::loadSpriteSheetFromFile(QString path)
{
    m_spriteSheet = new QPixmap();
    m_spriteSheetPath = path;
    return m_spriteSheet->load(path);
}