#ifndef ANIMATIONEDITOR_ANIMATIONFRAMESPANEL_H
#define ANIMATIONEDITOR_ANIMATIONFRAMESPANEL_H

#include <QVector>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include "AnimationsPanel.h"
#include "AnimationEditor/Animation.h"

class AnimationFramesPanel : public QWidget
{
    Q_OBJECT;
public:
    AnimationFramesPanel(QMainWindow* parent, AnimationsPanel *animationsPanel);
    AnimationFrame *getCurrentFrame();
    void setEnabled(bool);
public slots:
    void newFrame();
    void deleteFrame();
    void moveFrameUp();
    void moveFrameDown();
signals:
    void currentFrameChanged(AnimationFrame*);
private:
    void updateFramesView();

    QVector<AnimationFrame> *m_animationFrames{nullptr};
    AnimationFrame* m_currentFrame{nullptr};
    int m_frameIndex{-1};
    int m_frameCounter{0};

    QLabel      *framesLabel{nullptr};
    QListWidget *animationFramesView{nullptr};
    QPushButton *newFrameButton{nullptr};
    QPushButton *deleteFrameButton{nullptr};
    QPushButton *upFrameButton{nullptr};
    QPushButton *downFrameButton{nullptr};


};


#endif //ANIMATIONEDITOR_ANIMATIONFRAMESPANEL_H
