#include "AnimationEditor.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QFileDialog>
#include "XMLDocumentService.h"
#include "JSONDocumentService.h"

AnimationEditor::AnimationEditor(QWidget *parent) : QMainWindow(parent)
{
    QMainWindow::setWindowTitle("AnimationEditor");

    //Menu bar
    //File section
    fileMenu = menuBar()->addMenu("File");
    //New document
    newProjectAction = new QAction(fileMenu);
    newProjectAction->setText("New");
    connect(newProjectAction, SIGNAL(triggered(bool)), this, SLOT(createNewProject()));

    //Open document
    openProjectAction = new QAction(fileMenu);
    openProjectAction->setText("Open");
    connect(openProjectAction, SIGNAL(triggered(bool)), this, SLOT(openProject()));

    //Save document
    saveProjectAction = new QAction(fileMenu);
    saveProjectAction->setText("Save");
    connect(saveProjectAction, SIGNAL(triggered(bool)), this, SLOT(saveProject()));

    //Re-Export project to last used format
    reExportAction = new QAction(fileMenu);
    reExportAction->setText("Re-Export");
    connect(reExportAction, SIGNAL(triggered(bool)), this, SLOT(reExportDocument()));
    //Enable after opening/creating
    reExportAction->setEnabled(false);

    //Importing
    //Import from .XML
    importFromXMLAction = new QAction(fileMenu);
    importFromXMLAction->setText("Import from .XML");
    connect(importFromXMLAction, SIGNAL(triggered(bool)), this, SLOT(importFromXML()));
    importFromXMLAction->setEnabled(false);

    //Import from .JSON
    importFromJSONAction = new QAction(fileMenu);
    importFromJSONAction->setText("Import from .JSON");
    connect(importFromJSONAction, SIGNAL(triggered(bool)), this, SLOT(importFromJSON()));
    importFromJSONAction->setEnabled(false);

    //Exporting
    //Export to .XML
    exportToXMLAction = new QAction(fileMenu);
    exportToXMLAction->setText("Export To .XML");
    connect(exportToXMLAction, SIGNAL(triggered(bool)), this, SLOT(exportToXML()));
    exportToXMLAction->setEnabled(false);

    //Export to .JSON
    exportToJSONAction = new QAction(fileMenu);
    exportToJSONAction->setText("Export To .JSON");
    connect(exportToJSONAction, SIGNAL(triggered(bool)), this, SLOT(exportToJSON()));
    exportToJSONAction->setEnabled(false);

    //Exit app
    exitAction = new QAction(fileMenu);
    exitAction->setText("Exit");
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

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

    //Layout
    editorLayout = new QGridLayout(this);
    //Animations list
    animationsView = new QListWidget(this);
    //'Animations' label
    animationsLabel = new QLabel("Animations", this);
    animationsLabel->setAlignment(Qt::AlignCenter);
    //New Animation button
    newAnimationButton = new QPushButton("New", this);
    connect(newAnimationButton, SIGNAL(released()), this, SLOT(newAnimation()));
    //Delete Animation button
    deleteAnimationButton = new QPushButton("Delete", this);
    connect(deleteAnimationButton, SIGNAL(released()), this, SLOT(deleteAnimation()));

    //Place widgets into layout
    editorLayout->addWidget(animationsLabel, 0, 0, 1, 2);
    editorLayout->addWidget(animationsView, 1, 0, 1, 2);
    editorLayout->addWidget(newAnimationButton, 2, 0);
    editorLayout->addWidget(deleteAnimationButton, 2, 1);

    //Create widget -> apply layout -> set as central widget of main window
    QWidget *editor = new QWidget(this);
    editor->setLayout(editorLayout);
    QMainWindow::setCentralWidget(editor);

    //Disable widgets
    enableWidgets(false);
}

void AnimationEditor::enableWidgets(bool boolean)
{
    animationsView->setEnabled(boolean);
    newAnimationButton->setEnabled(boolean);
    deleteAnimationButton->setEnabled(boolean);
}

void AnimationEditor::setDocumentWriter(DocumentWriter *writer)
{
    m_documentWriter.reset(writer);
    reExportAction->setEnabled(true);
}

void AnimationEditor::createNewProject()
{
    m_actualDocumentPath = QFileDialog::getSaveFileName(this,
    tr("Create Project"), "", tr("Animation Editor Project (*.aep)"));

    if(!m_actualDocumentPath.isEmpty())
    {
        QFile projectFile(m_actualDocumentPath + ".aep");
        if(projectFile.open(QIODevice::WriteOnly))
        {
            enableWidgets(true);
            m_animations.clear();
            m_documentWriter.reset();
        }
    }
}

void AnimationEditor::openProject()
{

}

void AnimationEditor::saveProject()
{

}

void AnimationEditor::newAnimation()
{

}

void AnimationEditor::deleteAnimation()
{

}

void AnimationEditor::reExportDocument()
{
    //TODO EXCEPTIONS
    m_documentWriter->writeToFile(m_exportedPath, m_animations);
}

void AnimationEditor::importFromXML()
{

}

void AnimationEditor::importFromJSON()
{

}

void AnimationEditor::exportToXML()
{
    setDocumentWriter(new XMLDocumentWriter());
    reExportDocument();
}

void AnimationEditor::exportToJSON()
{
    setDocumentWriter(new JSONDocumentWriter());
    reExportDocument();
}
