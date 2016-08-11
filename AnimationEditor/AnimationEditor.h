#ifndef ANIMATIONEDITOR_H
#define ANIMATIONEDITOR_H

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QPushButton>
#include <QListWidget>
#include "Animation.h"
#include "AnimationEditor/DocumentServices/DocumentService.h"
#include "AnimationEditor/Panels/AnimationsPanel.h"
#include "AnimationEditor/Panels/AnimationFramesPanel.h"
#include "AnimationEditor/Panels/InputBoxesPanel.h"
#include <memory>
#include <QtWidgets/QCheckBox>
#include <AnimationEditor/Panels/AnimationEditorPanel.h>

class AnimationEditor : public QMainWindow
{
    Q_OBJECT
public:
    explicit AnimationEditor(QWidget *parent = 0);
public slots:
    void createNewProject();
    void openProject();
    void saveProject();
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
    //Animations panel
    AnimationsPanel *animationsPanel{nullptr};
    //Frames panel
    AnimationFramesPanel *framesPanel{nullptr};
    //Input Boxes panel
    InputBoxesPanel *inputBoxesPanel{nullptr};
    //Animation editor panel
    AnimationEditorPanel *animationEditorPanel{nullptr};
};

#endif // ANIMATIONEDITOR_H
