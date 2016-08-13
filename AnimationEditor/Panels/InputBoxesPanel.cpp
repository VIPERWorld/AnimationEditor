#include <QtWidgets/QGridLayout>
#include "InputBoxesPanel.h"

InputBoxesPanel::InputBoxesPanel(QMainWindow *parent, AnimationFramesPanel *framesPanel)
    :
        QWidget(parent)
{
    connect(framesPanel, &AnimationFramesPanel::currentFrameChanged, this, [this](AnimationFrame *frame)
    {
        if(frame != nullptr)
        {
            m_currentFrame = frame;
            setEnabled(true);
            updateValues();
        }
        else
        {
            setEnabled(false);
            updateValues();
        }
    });

    XLabel = new QLabel("X", this);
    XSpin = new QSpinBox(this);
    XSpin->setMaximum(99999);
    XSpin->setMinimum(-99999);
    connect(XSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val)
    {
        if(m_currentFrame)
            m_currentFrame->m_framePosition.setX(val);
        updateValues();
    });
    XSpin->setEnabled(false);

    YLabel = new QLabel("Y", this);
    YSpin = new QSpinBox(this);
    YSpin->setMaximum(99999);
    YSpin->setMinimum(-99999);
    connect(YSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val){
        if(m_currentFrame)
            m_currentFrame->m_framePosition.setY(val);
        updateValues();
    });
    YSpin->setEnabled(false);

    WidthLabel = new QLabel("Width", this);
    WidthSpin = new QSpinBox(this);
    WidthSpin->setMaximum(99999);
    WidthSpin->setMinimum(-99999);
    connect(WidthSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val){
        if(m_currentFrame)
            m_currentFrame->m_frameSize.setX(val);
        updateValues();
    });
    WidthSpin->setEnabled(false);

    HeightLabel = new QLabel("Height", this);
    HeightSpin = new QSpinBox(this);
    HeightSpin->setMaximum(99999);
    HeightSpin->setMinimum(-99999);
    connect(HeightSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val)
    {
        if (m_currentFrame)
            m_currentFrame->m_frameSize.setY(val);
        updateValues();
    });
    HeightSpin->setEnabled(false);

    auto layout = new QGridLayout(this);
    layout->addWidget(XLabel, 0, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(XSpin, 1, 0, 1, 1);
    layout->addWidget(YLabel, 0, 1, 1 ,1, Qt::AlignCenter);
    layout->addWidget(YSpin, 1, 1, 1, 1);
    layout->addWidget(WidthLabel, 2, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(WidthSpin, 3, 0, 1, 1);
    layout->addWidget(HeightLabel, 2, 1, 1, 1, Qt::AlignCenter);
    layout->addWidget(HeightSpin, 3, 1, 1, 1);
    setLayout(layout);
}

void InputBoxesPanel::updateValues()
{
    if(m_currentFrame)
    {
        XSpin->setValue((int)m_currentFrame->m_framePosition.x());
        YSpin->setValue((int)m_currentFrame->m_framePosition.y());
        WidthSpin->setValue((int)m_currentFrame->m_frameSize.x());
        HeightSpin->setValue((int)m_currentFrame->m_frameSize.y());
        emit valueChanged(m_currentFrame->m_framePosition, m_currentFrame->m_frameSize);
    }
    else
    {
        XSpin->clear();
        YSpin->clear();
        WidthSpin->clear();
        HeightSpin->clear();
        emit valueChanged(QVector2D(0, 0), QVector2D(0, 0));
    }
}

void InputBoxesPanel::setEnabled(bool boolean)
{
    XSpin->setEnabled(boolean);
    YSpin->setEnabled(boolean);
    WidthSpin->setEnabled(boolean);
    HeightSpin->setEnabled(boolean);
}

void InputBoxesPanel::updatePosition(QVector2D pos)
{
    if(m_currentFrame)
        m_currentFrame->m_framePosition = pos;
    updateValues();
}

