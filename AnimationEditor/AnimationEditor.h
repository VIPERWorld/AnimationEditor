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
    int m_animationIndex{-1};
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
    int m_frameIndex{-1};
    //Frame variables
    QSpinBox  *XSpin        {nullptr};
    QSpinBox  *YSpin        {nullptr};
    QSpinBox  *WidthSpin    {nullptr};
    QSpinBox  *HeightSpin   {nullptr};
};

#endif // ANIMATIONEDITOR_H
