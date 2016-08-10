#ifndef ANIMATIONEDITOR_INPUTBOXESPANEL_H
#define ANIMATIONEDITOR_INPUTBOXESPANEL_H


#include <QtWidgets/QSpinBox>
#include "AnimationFramesPanel.h"

class InputBoxesPanel : public QWidget
{
    Q_OBJECT;
public:
    InputBoxesPanel(QMainWindow *parent, AnimationFramesPanel *framesPanel);
    void setEnabled(bool);
signals:
    void valueChanged(QVector2D, QVector2D);
private:
    void updateValues();

    AnimationFrame *m_currentFrame{nullptr};
    QLabel *XLabel{nullptr};
    QSpinBox  *XSpin        {nullptr};
    QLabel *YLabel{nullptr};
    QSpinBox  *YSpin        {nullptr};
    QLabel *WidthLabel{nullptr};
    QSpinBox  *WidthSpin    {nullptr};
    QLabel *HeightLabel{nullptr};
    QSpinBox  *HeightSpin   {nullptr};
};


#endif //ANIMATIONEDITOR_INPUTBOXESPANEL_H
