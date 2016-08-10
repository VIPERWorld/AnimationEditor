#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>
#include <QtCore/QDir>
#include <QtWidgets/QGridLayout>
#include "AnimationFramesPanel.h"

AnimationFramesPanel::AnimationFramesPanel(QMainWindow *parent = 0, AnimationsPanel *animationsPanel = 0) : QWidget(parent)
{
    framesLabel = new QLabel("Frames", this);
    animationFramesView = new QListWidget(this);
    connect(animationFramesView, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item)
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Set new frame name"),
                                             tr("New frame name"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
        if (ok && !text.isEmpty() && m_currentFrame)
        {
            bool okName = true;
            for(auto &frame : *m_animationFrames)
            {
                if(frame.m_frameName == text)
                {
                    okName = false;
                    break;
                }
            }

            if(okName)
                m_currentFrame->m_frameName = text;
            else
            {
                QMessageBox warning;
                warning.setText("There's already frame with this name");
                warning.setStandardButtons(QMessageBox::Ok);
                warning.exec();
            }
            updateFramesView();
        }
    });
    //Update current animation frames
    connect(animationsPanel, &AnimationsPanel::currentAnimationChanged, this, [this](Animation* animation)
    {
        if(animation)
        {
            m_animationFrames = &animation->getFrames();
            m_currentFrame = nullptr;
            m_frameIndex = -1;
            setEnabled(false);
            animationFramesView->setEnabled(true);
            newFrameButton->setEnabled(true);
        }
        else
        {
            setEnabled(false);
            m_animationFrames = nullptr;
            m_currentFrame = nullptr;
            m_frameIndex = -1;
        }
        updateFramesView();
    });
    //Update frames index
    connect(animationFramesView, &QListWidget::currentRowChanged, this, [this](int index)
    {
        m_frameIndex = index;
        if(m_frameIndex != -1 && m_animationFrames)
        {
            m_currentFrame = &(*m_animationFrames)[m_frameIndex];
            setEnabled(true);
            if(m_frameIndex == 0)
                upFrameButton->setEnabled(false);
            if(m_frameIndex == m_animationFrames->size() - 1)
                downFrameButton->setEnabled(false);
        }
        else
        {
            m_currentFrame = nullptr;
            setEnabled(false);
            animationFramesView->setEnabled(true);
            newFrameButton->setEnabled(true);
        }
        emit currentFrameChanged(m_currentFrame);
    });
    animationFramesView->setEnabled(false);
    //New frame button
    newFrameButton = new QPushButton("New", this);
    connect(newFrameButton, &QPushButton::released, this, &AnimationFramesPanel::newFrame);
    //Delete frame button
    deleteFrameButton = new QPushButton("Delete", this);
    connect(deleteFrameButton, &QPushButton::released, this, &AnimationFramesPanel::deleteFrame);
    //Up frame button
    upFrameButton = new QPushButton("Up", this);
    connect(upFrameButton, &QPushButton::released, this, &AnimationFramesPanel::moveFrameUp);
    //Down frame button
    downFrameButton = new QPushButton("Down", this);
    connect(downFrameButton, &QPushButton::released, this, &AnimationFramesPanel::moveFrameDown);

    auto *layout = new QGridLayout(this);
    layout->addWidget(framesLabel, 0, 0, 1, 6, Qt::AlignCenter);
    layout->addWidget(animationFramesView, 1, 0, 3, 6);
    layout->addWidget(newFrameButton, 4, 0, 1, 2);
    layout->addWidget(upFrameButton, 4, 2, 1, 1);
    layout->addWidget(downFrameButton, 4, 3, 1, 1);
    layout->addWidget(deleteFrameButton, 4, 4, 1, 2);
    setLayout(layout);

    setEnabled(false);
}

void AnimationFramesPanel::setEnabled(bool boolean)
{
    animationFramesView->setEnabled(boolean);
    newFrameButton->setEnabled(boolean);
    deleteFrameButton->setEnabled(boolean);
    upFrameButton->setEnabled(boolean);
    downFrameButton->setEnabled(boolean);
}

AnimationFrame* AnimationFramesPanel::getCurrentFrame()
{
    return m_currentFrame;
}

void AnimationFramesPanel::updateFramesView()
{
    animationFramesView->clear();
    if(m_animationFrames)
    {
        for(auto &frame : *m_animationFrames)
            animationFramesView->addItem(frame.m_frameName);
        animationFramesView->setCurrentRow(m_frameIndex);
    }
}

void AnimationFramesPanel::newFrame()
{
     if(m_animationFrames)
     {
         AnimationFrame newFrame;
         newFrame.m_frameName = "Frame" + QString::number(m_frameCounter++);
         m_animationFrames->append(newFrame);
         updateFramesView();
     }
}

void AnimationFramesPanel::deleteFrame()
{
    if(m_animationFrames && m_currentFrame)
    {
        m_animationFrames->erase(m_currentFrame);
        m_currentFrame = nullptr;
    }
    updateFramesView();
}

void AnimationFramesPanel::moveFrameUp()
{

}

void AnimationFramesPanel::moveFrameDown()
{

}