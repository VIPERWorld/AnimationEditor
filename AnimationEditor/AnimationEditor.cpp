#include "AnimationEditor.h"
#include <QApplication>
#include <QMenuBar>
#include <QFileDialog>
#include <QtWidgets/QInputDialog>
#include "DocumentServices/XMLDocumentService.h"
#include "DocumentServices/JSONDocumentService.h"
#include "DocumentServices/AEPDocumentService.h"


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
            updateAnimationsView();
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
            updateAnimationsView();
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
                                                            tr("Where to save document"), "", tr("XML document (*.xml)"));
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
                                                          tr("Where to save document"), "", tr("JSON document (*.json)"));
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

    //Layout
    editorLayout = new QGridLayout(this);
    //Animations list
    animationsView = new QListWidget(this);
    connect(animationsView, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item)
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Set new animation name"),
                                             tr("New animation name"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
        if (ok && !text.isEmpty())
        {
            m_animations[m_animationIndex].setAnimationName(text);
            updateAnimationsView();
        }
    });
    connect(animationsView, &QListWidget::currentRowChanged, this, [this](int index){ m_animationIndex = index; });
    //'Animations' label
    animationsLabel = new QLabel("Animations", this);
    animationsLabel->setAlignment(Qt::AlignCenter);
    //New Animation button
    newAnimationButton = new QPushButton("New", this);
    connect(newAnimationButton, &QPushButton::released, this, &AnimationEditor::newAnimation);
    //Delete Animation button
    deleteAnimationButton = new QPushButton("Delete", this);
    connect(deleteAnimationButton, &QPushButton::released, this, &AnimationEditor::deleteAnimation);
    //Up animation button
    upAnimationButton = new QPushButton("Up", this);
    connect(upAnimationButton, &QPushButton::released, this, &AnimationEditor::moveAnimationUp);
    upAnimationButton->setEnabled(false);
    //Down animation button
    downAnimationButton = new QPushButton(QString("Down"), this);
    connect(downAnimationButton, &QPushButton::released, this, &AnimationEditor::moveAnimationDown);
    downAnimationButton->setEnabled(false);

    //Place widgets into layout
    editorLayout->addWidget(animationsLabel, 0, 0, 1, 6);
    editorLayout->addWidget(animationsView, 1, 0, 1, 6);
    editorLayout->addWidget(newAnimationButton, 2, 0, 1, 2);
    editorLayout->addWidget(upAnimationButton, 2, 2, 1, 1);
    editorLayout->addWidget(downAnimationButton, 2, 3, 1, 1);
    editorLayout->addWidget(deleteAnimationButton, 2, 4, 1, 2);

    //Create widget -> apply layout -> set as central widget of main window
    QWidget *editor = new QWidget(this);
    editor->setLayout(editorLayout);
    QMainWindow::setCentralWidget(editor);

    //Disable widgets
    enableWidgets(false);
    m_animations.clear();
}

void AnimationEditor::enableWidgets(bool boolean)
{
    animationsView->setEnabled(boolean);
    newAnimationButton->setEnabled(boolean);
    deleteAnimationButton->setEnabled(boolean);
    saveProjectAction->setEnabled(boolean);
    upAnimationButton->setEnabled(boolean);
    downAnimationButton->setEnabled(boolean);
    importFromJSONAction->setEnabled(boolean);
    importFromXMLAction->setEnabled(boolean);
    exportToJSONAction->setEnabled(boolean);
    exportToXMLAction->setEnabled(boolean);
}

void AnimationEditor::setDocumentWriter(DocumentWriter *writer)
{
    m_documentWriter.reset(writer);
    reExportAction->setEnabled(true);
}

void AnimationEditor::createNewProject()
{
   auto newDocumentPath = QFileDialog::getSaveFileName(this,
    tr("Create Project"), "", tr("Animation Editor Project (*.aep)"));

    if(!newDocumentPath.isEmpty())
    {
        m_actualDocumentPath = newDocumentPath + ".aep";
        QFile projectFile(m_actualDocumentPath);
        if(projectFile.open(QIODevice::WriteOnly))
        {
            enableWidgets(true);
            reExportAction->setEnabled(false);
            m_animationIndex = -1;
            animationsView->clear();
            m_animations.clear();
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
         //Create aep reader
         auto aepReader = AEPDocumentReader();
         //Read animations from file
         m_animations = aepReader.readFromFile(m_actualDocumentPath);
         //Enable widgets
         enableWidgets(true);
         reExportAction->setEnabled(false);
         m_animationIndex = -1;
         //Set writer to nullptr
         m_documentWriter.reset();
         updateAnimationsView();
     }
}

void AnimationEditor::saveProject()
{
    auto aepWriter = AEPDocumentWriter();
    aepWriter.writeToFile(m_actualDocumentPath, m_animations);
}


void AnimationEditor::newAnimation()
{
    m_animations.append(Animation());
    m_animations.back().setAnimationName("Animation");
    updateAnimationsView();
}

void AnimationEditor::deleteAnimation()
{
   if(m_animationIndex != -1)
       m_animations.erase(m_animations.begin() + m_animationIndex);
    updateAnimationsView();
    m_animationIndex = -1;
}

void AnimationEditor::moveAnimationUp()
{
    if(m_animationIndex > 0)
        m_animations.move(m_animationIndex--, m_animationIndex - 1);

    const int backupIndex = m_animationIndex;
    updateAnimationsView();
    animationsView->setCurrentRow(backupIndex, QItemSelectionModel::Select);
}

void AnimationEditor::moveAnimationDown()
{
    if(m_animationIndex != -1 && m_animationIndex < m_animations.size() - 1)
        m_animations.move(m_animationIndex++, m_animationIndex + 1);

    const int backupIndex = m_animationIndex;
    updateAnimationsView();
    animationsView->setCurrentRow(backupIndex, QItemSelectionModel::Select);
}

void AnimationEditor::updateAnimationsView()
{
    animationsView->clear();
    for(auto const &animation : m_animations)
        animationsView->addItem(animation.getAnimationName());
}
