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
#include "DocumentService.h"
#include <memory>

class AnimationEditor : public QMainWindow
{
    Q_OBJECT
public:
    explicit AnimationEditor(QWidget *parent = 0);    
public slots:
    void createNewProject();
    void openProject();
    void saveProject();
    void newAnimation();
    void deleteAnimation();
    void moveAnimationUp();
    void moveAnimationDown();
    void newFrame();
    void deleteFrame();
    void moveFrameUp();
    void moveFrameDown();
private:
    void enableWidgets(bool);
    //Document writer strategy
    std::unique_ptr<DocumentWriter> m_documentWriter{nullptr};
    void setDocumentWriter(DocumentWriter *writer);

    //Document
    QString m_actualDocumentPath;
    QString m_exportedPath;

    //File menu section
    QMenu   *fileMenu               {nullptr};
    QAction *newProjectAction       {nullptr};
    QAction *openProjectAction      {nullptr};
    QAction *saveProjectAction      {nullptr};
    QAction *reExportAction         {nullptr};
    QAction *importFromXMLAction    {nullptr};
    QAction *importFromJSONAction   {nullptr};
    QAction *exportToXMLAction      {nullptr};
    QAction *exportToJSONAction     {nullptr};
    QAction *exitAction             {nullptr};

    QGridLayout *editorLayout{nullptr};

    //Animations
    QVector<Animation> m_animations;
    QVector<Animation>::iterator m_currentAnimation{ m_animations.end() };
    std::unique_ptr<QVector<AnimationFrame>::iterator> m_currentFrame;
    QListWidget *animationsView{nullptr};
    void updateAnimationsView();
    QLabel *animationsLabel{nullptr};
    QPushButton *newAnimationButton{nullptr};
    QPushButton *deleteAnimationButton{nullptr};
    QPushButton *upAnimationButton{nullptr};
    QPushButton *downAnimationButton{nullptr};
    //Frames
    QLabel *framesLabel{nullptr};
    QListWidget *animationFramesView{nullptr};
    void updateFramesView();
    QPushButton *newFrameButton{nullptr};
    QPushButton *deleteFrameButton{nullptr};
    QPushButton *upFrameButton{nullptr};
    QPushButton *downFrameButton{nullptr};
    //Frame variables
    QLabel *XLabel{nullptr};
    QSpinBox  *XSpin        {nullptr};
    QLabel *YLabel{nullptr};
    QSpinBox  *YSpin        {nullptr};
    QLabel *WidthLabel{nullptr};
    QSpinBox  *WidthSpin    {nullptr};
    QLabel *HeightLabel{nullptr};
    QSpinBox  *HeightSpin   {nullptr};

    int animationCounter{0};
    int frameCounter{0};
};

#endif // ANIMATIONEDITOR_H
