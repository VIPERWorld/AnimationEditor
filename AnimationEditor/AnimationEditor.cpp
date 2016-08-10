#include "AnimationEditor.h"
#include <QApplication>
#include <QMenuBar>
#include <QFileDialog>
#include <QtWidgets/QInputDialog>
#include "DocumentServices/XMLDocumentService.h"
#include "DocumentServices/JSONDocumentService.h"
#include "DocumentServices/AEPDocumentService.h"
#include <iostream>
#include <QtWidgets/QMessageBox>
#include <AnimationEditor/Panels/AnimationEditorPanel.h>

AnimationEditor::AnimationEditor(QWidget *parent) : QMainWindow(parent)
{
    QMainWindow::setWindowTitle("AnimationEditor");
    //Menu bar
    //File section
    fileMenu = menuBar()->addMenu("File");
    //New document
    newProjectAction = new QAction(fileMenu);
    newProjectAction->setText("New");
    connect(newProjectAction, &QAction::triggered, this, &AnimationEditor::createNewProject);
    //Open document
    openProjectAction = new QAction(fileMenu);
    openProjectAction->setText("Open");
    connect(openProjectAction, &QAction::triggered, this, &AnimationEditor::openProject);
    //Save document
    saveProjectAction = new QAction(fileMenu);
    saveProjectAction->setText("Save");
    connect(saveProjectAction, &QAction::triggered, this, &AnimationEditor::saveProject);
    saveProjectAction->setEnabled(false);
    //Re-Export project to last used format
    reExportAction = new QAction(fileMenu);
    reExportAction->setText("Re-Export");
    connect(reExportAction, &QAction::triggered, this, [this]() { m_documentWriter->writeToFile(m_exportedPath, m_animations); });
    //Enable after opening/creating
    reExportAction->setEnabled(false);
    //Importing
    //Import from .XML
    importFromXMLAction = new QAction(fileMenu);
    importFromXMLAction->setText("Import from .XML");
    connect(importFromXMLAction, &QAction::triggered, this, [this] ()
    {
        auto importPath = QFileDialog::getOpenFileName(this,
                                                            tr("Choose .xml file which you want to import"), "", tr("XML document (*.xml)"));
        if(!importPath.isEmpty())
        {
            XMLDocumentReader xmlReader;
            m_animations = xmlReader.readFromFile(importPath);
            animationsPanel->setAnimations(m_animations);
        }
    });
    importFromXMLAction->setEnabled(false);
    //Import from .JSON
    importFromJSONAction = new QAction(fileMenu);
    importFromJSONAction->setText("Import from .JSON");
    connect(importFromJSONAction, &QAction::triggered, this, [this] ()
    {
        auto importPath = QFileDialog::getOpenFileName(this,
                                                       tr("Choose .json file which you want to import"), "", tr("JSON document (*.json)"));
        if(!importPath.isEmpty())
        {
            JSONDocumentReader jsonReader;
            m_animations = jsonReader.readFromFile(importPath);
            animationsPanel->setAnimations(m_animations);
        }
    });
    importFromJSONAction->setEnabled(false);
    //Exporting
    //Export to .XML
    exportToXMLAction = new QAction(fileMenu);
    exportToXMLAction->setText("Export To .XML");
    connect(exportToXMLAction, &QAction::triggered, this, [this]()
    {
        auto newExportPath = QFileDialog::getSaveFileName(this,
                                                            tr("Where to save document"), "", tr("XML document (*.xml)")).section(".",0,0);
        if(!newExportPath.isEmpty())
        {
            m_exportedPath = newExportPath + ".xml";
            setDocumentWriter(new XMLDocumentWriter);
            reExportAction->setEnabled(true);
            m_documentWriter->writeToFile(m_exportedPath, m_animations);
        }
    });
    exportToXMLAction->setEnabled(false);
    //Export to .JSON
    exportToJSONAction = new QAction(fileMenu);
    exportToJSONAction->setText("Export To .JSON");
    connect(exportToJSONAction, &QAction::triggered, this, [this]()
    {
        auto newExportPath = QFileDialog::getSaveFileName(this,
                                                          tr("Where to save document"), "", tr("JSON document (*.json)")).section(".",0,0);
        if(!newExportPath.isEmpty())
        {
            m_exportedPath = newExportPath + ".json";
            setDocumentWriter(new JSONDocumentWriter);
            reExportAction->setEnabled(true);
            m_documentWriter->writeToFile(m_exportedPath, m_animations);
        }
    });
    exportToJSONAction->setEnabled(false);
    //Exit app
    exitAction = new QAction(fileMenu);
    exitAction->setText("Exit");
    connect(exitAction, &QAction::triggered, this, &AnimationEditor::close);
    //Pack actions into QList
    QList<QAction*> fileActions;
    fileActions.append(newProjectAction);
    fileActions.append(openProjectAction);
    fileActions.append(saveProjectAction);
    fileActions.append(fileMenu->addSeparator());
    fileActions.append(reExportAction);
    fileActions.append(importFromXMLAction);
    fileActions.append(importFromJSONAction);
    fileActions.append(exportToXMLAction);
    fileActions.append(exportToJSONAction);
    fileActions.append(fileMenu->addSeparator());
    fileActions.append(exitAction);
    //Add actions to fileMenu
    fileMenu->addActions(fileActions);

    //Animations panel
    animationsPanel = new AnimationsPanel(this);
    //Frames panel
    framesPanel = new AnimationFramesPanel(this, animationsPanel);
    //Input boxes panel
    inputBoxesPanel = new InputBoxesPanel(this, framesPanel);
    //Animation editor panel
    animationEditorPanel = new AnimationEditorPanel(this, animationsPanel, inputBoxesPanel);
    //Layout
    editorLayout = new QGridLayout(this);
    editorLayout->addWidget(animationsPanel, 0, 0, 1, 1);
    editorLayout->addWidget(framesPanel, 1, 0, 1, 2);
    editorLayout->addWidget(inputBoxesPanel, 1, 2, 1, 1);
    editorLayout->addWidget(animationEditorPanel, 0, 1, 1, 2);

    QWidget *editor = new QWidget(this);
    editor->setLayout(editorLayout);
    QMainWindow::setCentralWidget(editor);
    //Disable widgets
    enableWidgets(false);
    m_animations.clear();
}

void AnimationEditor::enableWidgets(bool boolean)
{
    saveProjectAction->setEnabled(boolean);
    importFromJSONAction->setEnabled(boolean);
    importFromXMLAction->setEnabled(boolean);
    exportToJSONAction->setEnabled(boolean);
    exportToXMLAction->setEnabled(boolean);
    animationEditorPanel->setEnabled(boolean);
}

void AnimationEditor::setDocumentWriter(DocumentWriter *writer)
{
    m_documentWriter.reset(writer);
    reExportAction->setEnabled(true);
}

void AnimationEditor::createNewProject()
{
   auto newDocumentPath = QFileDialog::getSaveFileName(this,
    tr("Create Project"), "", tr("Animation Editor Project (*.aep)")).section(".",0,0);

    if(!newDocumentPath.isEmpty())
    {
        m_actualDocumentPath = newDocumentPath + ".aep";
        QFile projectFile(m_actualDocumentPath);
        if(projectFile.open(QIODevice::WriteOnly))
        {
            enableWidgets(true);
            reExportAction->setEnabled(false);
            m_animations.clear();
            animationsPanel->setAnimations(m_animations);
            m_documentWriter.reset(); 
        }
    }
}

void AnimationEditor::openProject()
{
    auto newDocumentPath = QFileDialog::getOpenFileName(this,
     tr("Open Project"), "", tr("Animation Editor Project (*.aep)"));

     if(!newDocumentPath.isEmpty())
     {
         m_actualDocumentPath = newDocumentPath;
         auto aepReader = AEPDocumentReader();
         enableWidgets(true);
         m_animations = aepReader.readFromFile(m_actualDocumentPath);
         animationsPanel->setAnimations(m_animations);
         m_documentWriter.reset();
         reExportAction->setEnabled(false);
     }
}

void AnimationEditor::saveProject()
{
    auto aepWriter = AEPDocumentWriter();
    aepWriter.writeToFile(m_actualDocumentPath, m_animations);
}