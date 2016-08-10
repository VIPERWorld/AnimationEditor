#include <QtWidgets/QGridLayout>
#include "AnimationEditorPanel.h"
#include "InputBoxesPanel.h"
#include <QGraphicsPixmapItem>

AnimationEditorPanel::AnimationEditorPanel(QMainWindow *parent, AnimationsPanel *animationsPanel, InputBoxesPanel* inputBoxesPanel)
{
    animationEditorLabel = new QLabel("Animation Editor", this);
    animationEditorScene = new QGraphicsScene(this);
    animationEditorView = new QGraphicsView(this);
    animationEditorView->setScene(animationEditorScene);
    connect(animationsPanel, &AnimationsPanel::currentAnimationChanged, this, &AnimationEditorPanel::updateSpriteSheet);
    connect(inputBoxesPanel, &InputBoxesPanel::valueChanged, this, &AnimationEditorPanel::updateFrameRectangle);
    auto layout = new QGridLayout(this);
    layout->addWidget(animationEditorLabel, 0, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(animationEditorView, 1, 0, 1, 1);
    setLayout(layout);
}

void AnimationEditorPanel::updateSpriteSheet(Animation *animation)
{
    if(animation)
    {
        auto &&spriteSheet = animation->getSpritesheet();
        if (spriteSheet)
        {
            if (m_actualSpriteSheet)
            {
                animationEditorScene->removeItem(m_actualSpriteSheet);
                m_actualSpriteSheet = animationEditorScene->addPixmap(*spriteSheet);
            }
            else
            {
                animationEditorScene->clear();
                m_actualSpriteSheet = animationEditorScene->addPixmap(*spriteSheet);
            }
        }
        else
        {
            if (m_actualSpriteSheet)
            {
                animationEditorScene->clear();
                delete m_actualSpriteSheet;
            }
            else
                animationEditorScene->clear();
        }
    }
}

void AnimationEditorPanel::updateFrameRectangle(QVector2D pos, QVector2D size)
{
    if(pos.x() != 0 || pos.y() != 0 || size.x() != 0 || size.y() != 0)
    {
        if (m_actualFrameRectange)
        {
            animationEditorScene->removeItem(m_actualFrameRectange);
            m_actualFrameRectange = animationEditorScene->addRect(
                    QRectF(QPointF(pos.x(), pos.y()), QPointF(size.x(), size.y())), QPen(QColor(255, 0, 0, 200)));
        } else
            m_actualFrameRectange = animationEditorScene->addRect(
                    QRectF(QPointF(pos.x(), pos.y()), QPointF(size.x(), size.y())), QPen(QColor(255, 0, 0, 200)));
    }
}