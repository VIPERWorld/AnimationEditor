#include <QtCore/QItemSelectionModel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QGridLayout>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>
#include "AnimationsPanel.h"
#include "AnimationEditor/Animation.h"

AnimationsPanel::AnimationsPanel(QWidget* parent = 0) :
    QWidget(parent)
{
    animationsView = new QListWidget(this);
    connect(animationsView, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item)
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Set new animation name"),
                                             tr("New animation name"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
        if (ok && !text.isEmpty())
        {
            bool okName = true;
            for(auto &animation : *m_animations)
            {
                if(animation.getAnimationName() == text)
                {
                    okName = false;
                    break;
                }
            }

            if(okName)
                m_currentAnimation->setAnimationName(text);
            else
            {
                QMessageBox warning;
                warning.setText("There's already animation with this name");
                warning.setStandardButtons(QMessageBox::Ok);
                warning.exec();
            }
            updateAnimationsView();
        }
    });

    connect(animationsView, &QListWidget::currentRowChanged, this, [this](int index)
    {
        m_animationIndex = index;
        if(m_animationIndex != -1)
        {
            m_currentAnimation = &(*m_animations)[m_animationIndex];
            setEnabled(true);
            if(m_animationIndex == 0)
                upAnimationButton->setEnabled(false);
            if(m_animationIndex == m_animations->size() - 1)
                downAnimationButton->setEnabled(false);
        }
        else
        {
            m_currentAnimation = nullptr;
            setEnabled(false);
            animationsView->setEnabled(true);
            newAnimationButton->setEnabled(true);
        }
        emit currentAnimationChanged(m_currentAnimation);
    });

    //'Animations' label
    animationsLabel = new QLabel("Animations", this);
    animationsLabel->setAlignment(Qt::AlignCenter);
    //New Animation button
    newAnimationButton = new QPushButton("New", this);
    connect(newAnimationButton, &QPushButton::released, this, &AnimationsPanel::newAnimation);
    //Delete Animation button
    deleteAnimationButton = new QPushButton("Delete", this);
    connect(deleteAnimationButton, &QPushButton::released, this, &AnimationsPanel::deleteAnimation);
    //Up animation button
    upAnimationButton = new QPushButton("Up", this);
    connect(upAnimationButton, &QPushButton::released, this, &AnimationsPanel::moveAnimationUp);
    //Down animation button
    downAnimationButton = new QPushButton("Down", this);
    connect(downAnimationButton, &QPushButton::released, this, &AnimationsPanel::moveAnimationDown);
    //Spritesheet label
    spriteSheetLabel = new QLabel("No spritesheet");
    //Set spritesheet button
    setSpriteSheetButton = new QPushButton("Set SpriteSheet", this);
    connect(setSpriteSheetButton, &QPushButton::released, this, &AnimationsPanel::setSpriteSheet);
    setSpriteSheetButton->setEnabled(false);

    auto *layout = new QGridLayout(this);
    layout->addWidget(animationsLabel, 0, 0, 1, 6, Qt::AlignCenter);
    layout->addWidget(animationsView, 1, 0, 3, 6);
    layout->addWidget(newAnimationButton, 4, 0, 1, 2);
    layout->addWidget(upAnimationButton, 4, 2, 1, 1);
    layout->addWidget(downAnimationButton, 4, 3, 1, 1);
    layout->addWidget(deleteAnimationButton, 4, 4, 1, 2);
    layout->addWidget(spriteSheetLabel, 5, 0, 1, 3);
    layout->addWidget(setSpriteSheetButton, 5, 3, 1, 3);
    setLayout(layout);

    setEnabled(false);
}

void AnimationsPanel::setEnabled(bool boolean)
{
    animationsView->setEnabled(boolean);
    newAnimationButton->setEnabled(boolean);
    deleteAnimationButton->setEnabled(boolean);
    upAnimationButton->setEnabled(boolean);
    downAnimationButton->setEnabled(boolean);
    setSpriteSheetButton->setEnabled(boolean);
}

Animation* AnimationsPanel::getCurrentAnimation()
{
    return m_currentAnimation;
}

void AnimationsPanel::setAnimations(QVector<Animation> &animations)
{
    m_animations = &animations;
    updateAnimationsView();
    setEnabled(false);
    animationsView->setEnabled(true);
    newAnimationButton->setEnabled(true);
}

void AnimationsPanel::updateAnimationsView()
{
    animationsView->clear();
    for(auto const &animation : *m_animations)
        animationsView->addItem(animation.getAnimationName());
}

void AnimationsPanel::newAnimation()
{
    m_animations->append(Animation());
    m_animations->back().setAnimationName("Animation" + QString::number(m_animationCounter++));
    updateAnimationsView();
}

void AnimationsPanel::deleteAnimation()
{
    if (m_animations && m_currentAnimation)
    {
        m_animations->erase(m_animations->begin() + m_animationIndex);
        m_currentAnimation = nullptr;
        updateAnimationsView();
        emit currentAnimationChanged(m_currentAnimation);
    }
}

void AnimationsPanel::moveAnimationUp()
{
    if(m_animationIndex > 0 && m_currentAnimation && m_animations)
    {
        m_animations->move(m_animationIndex, m_animationIndex - 1);
        m_currentAnimation = &(*m_animations)[--m_animationIndex];
        const int index = m_animationIndex;
        updateAnimationsView();
        emit currentAnimationChanged(m_currentAnimation);
        animationsView->setCurrentRow(index);
    }
}

void AnimationsPanel::moveAnimationDown()
{
    if(m_animations && m_currentAnimation && m_animationIndex < m_animations->size() - 1)
    {
        m_animations->move(m_animationIndex, m_animationIndex + 1);
        m_currentAnimation = &(*m_animations)[++m_animationIndex];
        const int index = m_animationIndex;
        updateAnimationsView();
        emit currentAnimationChanged(m_currentAnimation);
        animationsView->setCurrentRow(index);
    }
}

void AnimationsPanel::setSpriteSheet()
{
    if(m_currentAnimation)
    {
        auto imagePath = QFileDialog::getOpenFileName(this,
                                                            tr("Choose spritesheet"), "", tr("Image file (*.png)"));
        if(!imagePath.isEmpty())
        {
            m_currentAnimation->loadSpriteSheetFromFile(imagePath);
            emit currentAnimationChanged(m_currentAnimation);
            spriteSheetLabel->setText(QFileInfo(imagePath).fileName());
        }
    }
}