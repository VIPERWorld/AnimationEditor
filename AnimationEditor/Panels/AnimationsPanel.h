#ifndef ANIMATIONEDITOR_ANIMATIONSPANEL_H
#define ANIMATIONEDITOR_ANIMATIONSPANEL_H
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QPixmap>

class AnimationEditor;
class Animation;

class AnimationsPanel : public QWidget
{
    Q_OBJECT;
public:
    explicit AnimationsPanel(QWidget* parent);
    void setAnimations(QVector<Animation> &animations);
    Animation* getCurrentAnimation();
    void setEnabled(bool);
signals:
    void currentAnimationChanged(Animation*);
public slots:
    void newAnimation();
    void deleteAnimation();
    void moveAnimationUp();
    void moveAnimationDown();
    void setSpriteSheet();
private:
    void updateAnimationsView();

    QVector<Animation> *m_animations{nullptr};
    Animation *m_currentAnimation{nullptr};
    int m_animationIndex{-1};
    int m_animationCounter{0};
    QVector<QPixmap> m_spriteSheetCache;

    QListWidget *animationsView{nullptr};
    QLabel *animationsLabel{nullptr};
    QPushButton *newAnimationButton{nullptr};
    QPushButton *deleteAnimationButton{nullptr};
    QPushButton *upAnimationButton{nullptr};
    QPushButton *downAnimationButton{nullptr};
    QLabel *spriteSheetLabel{nullptr};
    QPushButton *setSpriteSheetButton{nullptr};
};


#endif //ANIMATIONEDITOR_ANIMATIONSPANEL_H
