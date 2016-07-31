#include "AnimationEditor.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QFileDialog>

AnimationEditor::AnimationEditor(QWidget *parent) : QMainWindow(parent)
{
    QMainWindow::setWindowTitle("AnimationEditor");

    //Menu bar
    //File section
    fileMenu = menuBar()->addMenu("File");

    newFileAction = new QAction(fileMenu);
    newFileAction->setText("New");
    connect(newFileAction, SIGNAL(triggered(bool)), this, SLOT(createNewDocument()));

    openFileAction = new QAction(fileMenu);
    openFileAction->setText("Open");
    connect(openFileAction, SIGNAL(triggered(bool)), this, SLOT(openDocument()));

    exitAction = new QAction(fileMenu);
    exitAction->setText("Exit");
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    QList<QAction*> fileActions;
    fileActions.push_back(newFileAction);
    fileActions.append(openFileAction);
    fileActions.append(exitAction);
    fileMenu->addActions(fileActions);

    //Layout
    editorLayout = new QGridLayout(this);
    animationsLabel = new QLabel("Animations", this);
    animationsLabel->setAlignment(Qt::AlignCenter);

    newAnimationButton = new QPushButton("New", this);
    connect(newAnimationButton, SIGNAL(released()), this, SLOT(newAnimation()));
    deleteAnimationButton = new QPushButton("Delete", this);
    connect(deleteAnimationButton, SIGNAL(released()), this, SLOT(deleteAnimation()));

    animationsView = new QListWidget(this);
    editorLayout->addWidget(animationsLabel, 0, 0, 1, 2);
    editorLayout->addWidget(animationsView, 1, 0, 1, 2);
    editorLayout->addWidget(newAnimationButton, 2, 0);
    editorLayout->addWidget(deleteAnimationButton, 2, 1);

    QWidget *editor = new QWidget(this);
    editor->setLayout(editorLayout);

    QMainWindow::setCentralWidget(editor);
}

void AnimationEditor::enableWidgets(bool boolean)
{

}

void AnimationEditor::createNewDocument()
{
    m_actualDocumentName = QFileDialog::getSaveFileName(this,
        tr("Create Document"), "", tr("XML documents (*.xml)"));

    if(!m_actualDocumentName.isEmpty())
    {


    }
}

void AnimationEditor::openDocument()
{

}

void AnimationEditor::newAnimation()
{

}

void AnimationEditor::deleteAnimation()
{

}
