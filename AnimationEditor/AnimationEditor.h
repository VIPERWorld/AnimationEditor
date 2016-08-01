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
    void reExportDocument();
    void importFromXML();
    void importFromJSON();
    void exportToXML();
    void exportToJSON();
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
    Animation* m_actualAnimation{nullptr};
    QListWidget *animationsView{nullptr};
    QLabel *animationsLabel  {nullptr};
    QPushButton *newAnimationButton{nullptr};
    QPushButton *deleteAnimationButton{nullptr};
    //Frames
    QListWidget *animationFramesView  {nullptr};
    //Frame variables
    QSpinBox  *XSpin        {nullptr};
    QSpinBox  *YSpin        {nullptr};
    QSpinBox  *WidthSpin    {nullptr};
    QSpinBox  *HeightSpin   {nullptr};
};

#endif // ANIMATIONEDITOR_H
