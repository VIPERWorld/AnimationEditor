#include <QtWidgets/QGridLayout>
#include "AnimationEditorPanel.h"
#include "InputBoxesPanel.h"
#include <QGraphicsPixmapItem>
#include <QMouseEvent>

AnimationEditorPanel::AnimationEditorPanel(QMainWindow *parent, AnimationsPanel *animationsPanel, InputBoxesPanel* inputBoxesPanel)
{
    animationEditorLabel = new QLabel("Animation Editor", this);
    animationEditorScene = new QGraphicsScene(this);
    animationEditorView = new AnimationEditorView(this);
    animationEditorView->setScene(animationEditorScene);
    editorCords = new QLabel("X: Y: ", this);
    connect(animationsPanel, &AnimationsPanel::currentAnimationChanged, this, &AnimationEditorPanel::updateSpriteSheet);
    connect(inputBoxesPanel, &InputBoxesPanel::valueChanged, this, &AnimationEditorPanel::updateFrameRectangle);
    connect(animationEditorView, &AnimationEditorView::mouseMoved, this, &AnimationEditorPanel::updateCords);
    auto layout = new QGridLayout(this);
    layout->addWidget(animationEditorLabel, 0, 0, 1, 2, Qt::AlignCenter);
    layout->addWidget(editorCords, 0, 2, 1, 1, Qt::AlignRight);
    layout->addWidget(animationEditorView, 1, 0, 1, 3);
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
                animationEditorScene->removeItem(m_actualSpriteSheet);
            m_actualSpriteSheet = nullptr;
            animationEditorView->setTrackedItem(nullptr);
            animationEditorScene->clear();
        }
    }

    animationEditorView->repaint();
}

void AnimationEditorPanel::updateFrameRectangle(QVector2D pos, QVector2D size)
{
    if(pos.x() != 0 || pos.y() != 0 && (size.x() != 0 && size.y() != 0))
    {
        if (m_actualFrameRectangle)
        {
            animationEditorScene->removeItem(m_actualFrameRectangle);
            m_actualFrameRectangle = animationEditorScene->addRect(
                    QRectF(QPointF(pos.x(), pos.y()), QPointF(size.x(), size.y())), QPen(QColor(255, 0, 0, 200)));
        } else
            m_actualFrameRectangle = animationEditorScene->addRect(
                    QRectF(QPointF(pos.x(), pos.y()), QPointF(size.x(), size.y())), QPen(QColor(255, 0, 0, 200)));

        m_actualFrameRectangle->setFlag(QGraphicsItem::ItemIsMovable);
    }
    else
    {
        if(m_actualFrameRectangle)
            animationEditorScene->removeItem(m_actualFrameRectangle);
        m_actualFrameRectangle = nullptr;
    }

    animationEditorView->setTrackedItem(m_actualFrameRectangle);
    animationEditorView->repaint();
}

void AnimationEditorPanel::updateCords(QVector2D pos)
{
    editorCords->setText("X: " + QString::number(pos.x()) + " Y: " + QString::number(pos.y()));
}