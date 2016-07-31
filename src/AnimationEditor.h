#ifndef ANIMATIONEDITOR_H
#define ANIMATIONEDITOR_H

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QPushButton>
#include <QListWidget>
#include "Animation.h"

class AnimationEditor : public QMainWindow
{
    Q_OBJECT
public:
    explicit AnimationEditor(QWidget *parent = 0);    
public slots:
    void createNewDocument();
    void openDocument();
    void newAnimation();
    void deleteAnimation();
private:
    void enableWidgets(bool);

    QMenu *fileMenu         {nullptr};
    QAction *newFileAction  {nullptr};
    QAction *openFileAction {nullptr};
    QAction *exitAction     {nullptr};

    QGridLayout *editorLayout{nullptr};

    QString m_actualDocumentName;

    QVector<Animation> m_animations;
    QListWidget *animationsView{nullptr};
    QLabel *animationsLabel  {nullptr};
    QPushButton *newAnimationButton{nullptr};
    QPushButton *deleteAnimationButton{nullptr};

    QListView *animationFramesView  {nullptr};
    QGraphicsScene *animationPreview{nullptr};

    QSpinBox  *XSpin        {nullptr};
    QSpinBox  *YSpin        {nullptr};
    QSpinBox  *WidthSpin    {nullptr};
    QSpinBox  *HeightSpin   {nullptr};
};

#endif // ANIMATIONEDITOR_H
