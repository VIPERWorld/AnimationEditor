#ifndef ANIMATIONEDITOR_ANIMATIONEDITORPANEL_H
#define ANIMATIONEDITOR_ANIMATIONEDITORPANEL_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include "AnimationFramesPanel.h"
#include "InputBoxesPanel.h"

class AnimationEditorPanel : public QWidget
{
    Q_OBJECT;
public:
    AnimationEditorPanel(QMainWindow *parent, AnimationsPanel*, InputBoxesPanel*);
public slots:
    void updateSpriteSheet(Animation*);
    void updateFrameRectangle(QVector2D, QVector2D);
private:
    QGraphicsPixmapItem *m_actualSpriteSheet{nullptr};
    QGraphicsRectItem *m_actualFrameRectange{nullptr};

    QLabel *animationEditorLabel{nullptr};
    QGraphicsView *animationEditorView{nullptr};
    QGraphicsScene *animationEditorScene{nullptr};
};


#endif //ANIMATIONEDITOR_ANIMATIONEDITORPANEL_H
