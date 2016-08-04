#include "AnimationEditor.h"
#include <QApplication>
#include <QMenuBar>
#include <QFileDialog>
#include <QtWidgets/QInputDialog>
#include "DocumentServices/XMLDocumentService.h"
#include "DocumentServices/JSONDocumentService.h"
#include "DocumentServices/AEPDocumentService.h"
#include <QtAlgorithms>
#include <iostream>
#include <QtWidgets/QMessageBox>

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
    //Layout
    editorLayout = new QGridLayout(this);
    //////////////////////////////////////////////Animations
    animationsView = new QListWidget(this);
    connect(animationsView, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item)
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Set new animation name"),
                                             tr("New animation name"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
        if (ok && !text.isEmpty())
        {
            bool okName = true;
            for(auto &animation : m_animations)
            {
                if(animation.getAnimationName() == text)
                {
                    okName = false;
                    break;
                }
            }
            if(okName)
                m_currentAnimation->setAnimationName(text);
            else
            {
                QMessageBox warning;
                warning.setText("There's already animation with this name");
                warning.setStandardButtons(QMessageBox::Ok);
                warning.exec();
            }
            updateAnimationsView();
        }
    });
    connect(animationsView, &QListWidget::currentRowChanged, this, [this](int index)
    {
        if(index != -1)
            m_currentAnimation = &m_animations[index];
        else
            m_currentAnimation = nullptr;
        updateFramesView();
    });
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
    //////////////////////////////////////////////Frames
    framesLabel = new QLabel("Frames", this);
    animationFramesView = new QListWidget(this);
    connect(animationFramesView, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item)
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Set new frame name"),
                                             tr("New frame name"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
        if (ok && !text.isEmpty() && m_currentFrame)
        {
            bool okName = true;
            for(auto &frame : m_currentAnimation->getFrames())
            {
                if(frame.m_frameName == text)
                {
                    okName = false;
                    break;
                }
            }
            
            if(okName)
                (*m_currentFrame)->m_frameName = text;
            else
            {
                QMessageBox warning;
                warning.setText("There's already frame with this name");
                warning.setStandardButtons(QMessageBox::Ok);
                warning.exec();
            }
            updateFramesView();
        }
    });
    //Update frames index
    connect(animationFramesView, &QListWidget::currentRowChanged, this, [this](int index)
    {
        if(index != -1 && m_currentAnimation != m_animations.end())
        {
            m_currentFrame.reset(new QVector<AnimationFrame>::iterator);
            *m_currentFrame = m_currentAnimation->getFrames().begin() + index;
        }
        else
            m_currentFrame.reset();

        if(m_currentAnimation != m_animations.end() && m_currentFrame)
        {
            XSpin->setEnabled(true);
            YSpin->setEnabled(true);
            WidthSpin->setEnabled(true);
            HeightSpin->setEnabled(true);
            XSpin->setValue((int)(*m_currentFrame)->m_framePosition.x());
            YSpin->setValue((int)(*m_currentFrame)->m_framePosition.y());
            WidthSpin->setValue((int)(*m_currentFrame)->m_frameSize.x());
            HeightSpin->setValue((int)(*m_currentFrame)->m_frameSize.y());
        }
        else
        {
            XSpin->setEnabled(false);
            YSpin->setEnabled(false);
            WidthSpin->setEnabled(false);
            HeightSpin->setEnabled(false);
            XSpin->clear();
            YSpin->clear();
            WidthSpin->clear();
            HeightSpin->clear();
        }

    });
    animationFramesView->setEnabled(false);
    //New frame button
    newFrameButton = new QPushButton("New", this);
    connect(newFrameButton, &QPushButton::released, this, &AnimationEditor::newFrame);
    newFrameButton->setEnabled(false);
    //Delete frame button
    deleteFrameButton = new QPushButton("Delete", this);
    connect(deleteFrameButton, &QPushButton::released, this, &AnimationEditor::deleteFrame);
    deleteFrameButton->setEnabled(false);
    //Up frame button
    upFrameButton = new QPushButton("Up", this);
    connect(upFrameButton, &QPushButton::released, this, &AnimationEditor::moveFrameUp);
    upFrameButton->setEnabled(false);
    //Down frame button
    downFrameButton = new QPushButton("Down", this);
    connect(downFrameButton, &QPushButton::released, this, &AnimationEditor::moveFrameDown);
    downFrameButton->setEnabled(false);
    //////////////////////////////////////////////Frame input boxes
    XLabel = new QLabel("X", this);
    XSpin = new QSpinBox(this);
    connect(XSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val)
    {
       if(m_currentAnimation && m_currentFrame)
           (*m_currentFrame)->m_framePosition.setX(val);
    });
    XSpin->setEnabled(false);
    YLabel = new QLabel("Y", this);
    YSpin = new QSpinBox(this);
    connect(YSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val){
        if(m_currentAnimation && m_currentFrame)
            (*m_currentFrame)->m_framePosition.setY(val);
    });
    YSpin->setEnabled(false);
    WidthLabel = new QLabel("Width", this);
    WidthSpin = new QSpinBox(this);
    connect(WidthSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val){
        if(m_currentAnimation && m_currentFrame)
            (*m_currentFrame)->m_frameSize.setX(val);
    });
    WidthSpin->setEnabled(false);
    HeightLabel = new QLabel("Height", this);
    HeightSpin = new QSpinBox(this);
    connect(HeightSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val){
        if(m_currentAnimation && m_currentFrame)
            (*m_currentFrame)->m_frameSize.setY(val);
    });
    HeightSpin->setEnabled(false);
    //////////////////////////////////////////////Layout
    editorLayout->addWidget(animationsLabel, 0, 0, 1, 6);
    editorLayout->addWidget(animationsView, 1, 0, 1, 6);
    editorLayout->addWidget(newAnimationButton, 2, 0, 1, 2);
    editorLayout->addWidget(upAnimationButton, 2, 2, 1, 1);
    editorLayout->addWidget(downAnimationButton, 2, 3, 1, 1);
    editorLayout->addWidget(deleteAnimationButton, 2, 4, 1, 2);
    editorLayout->addWidget(framesLabel, 3, 0, 1, 12, Qt::AlignCenter);
    editorLayout->addWidget(animationFramesView, 4, 0, 4, 12);
    editorLayout->addWidget(newFrameButton, 8, 0, 1, 5);
    editorLayout->addWidget(upFrameButton, 8, 5, 1, 1);
    editorLayout->addWidget(downFrameButton, 8, 6, 1, 1);
    editorLayout->addWidget(deleteFrameButton, 8, 7, 1, 5);
    editorLayout->addWidget(XLabel, 4, 12, 1, 2, Qt::AlignCenter);
    editorLayout->addWidget(XSpin, 5, 12, 1, 2);
    editorLayout->addWidget(YLabel, 4, 14, 1, 2, Qt::AlignCenter);
    editorLayout->addWidget(YSpin, 5, 14, 1 , 2);
    editorLayout->addWidget(WidthLabel, 6, 12, 1, 2, Qt::AlignCenter);
    editorLayout->addWidget(WidthSpin, 7, 12, 1, 2);
    editorLayout->addWidget(HeightLabel, 6, 14, 1, 2, Qt::AlignCenter);
    editorLayout->addWidget(HeightSpin, 7, 14, 1, 2);
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
    animationFramesView->setEnabled(boolean);
    newFrameButton->setEnabled(boolean);
    deleteFrameButton->setEnabled(boolean);
    upFrameButton->setEnabled(boolean);
    downFrameButton->setEnabled(boolean);
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
            m_currentAnimation = nullptr;
            m_currentFrame = nullptr;
            animationsView->clear();
            animationFramesView->clear();
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
         m_currentAnimation = nullptr;
         m_currentFrame = nullptr;
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

void AnimationEditor::updateAnimationsView()
{
    animationsView->clear();
    for(auto const &animation : m_animations)
        animationsView->addItem(animation.getAnimationName());
}

void AnimationEditor::updateFramesView()
{
    animationFramesView->clear();
    if(m_currentAnimation && m_currentAnimation->getFrames().size() > 0)
    {
        for (auto const &frame : m_currentAnimation->getFrames())
            animationFramesView->addItem(frame.m_frameName);
    }
}

//Animation creation methods
void AnimationEditor::newAnimation()
{
    m_animations.append(Animation());
    m_animations.back().setAnimationName("Animation" + QString::number(animationCounter++));
    updateAnimationsView();
}

void AnimationEditor::deleteAnimation()
{

    if (m_currentAnimation)
    {
        m_animations.erase(m_currentAnimation);
        m_currentAnimation = nullptr;
    }
    updateAnimationsView();

}

void AnimationEditor::moveAnimationUp()
{
    int distance = -1;
    if(m_currentAnimation)
    {
        distance = (int) std::distance(m_animations.begin(), m_currentAnimation);
        if (distance > 0)
        {
            m_animations.move(distance, distance - 1);
            --distance;
        }
    }
    updateAnimationsView();
    if(distance != -1)
        animationsView->setCurrentRow(distance, QItemSelectionModel::Select);
}

void AnimationEditor::moveAnimationDown()
{
    int distance = -1;
    if(m_currentAnimation)
    {
        distance = (int) std::distance(m_animations.begin(), m_currentAnimation);
        if (distance < m_animations.size() - 1)
        {
            m_animations.move(distance, distance + 1);
            ++distance;
        }
        else
            distance = m_animations.size() - 1;
    }
    updateAnimationsView();
    if(distance != -1)
        animationsView->setCurrentRow(distance, QItemSelectionModel::Select);
}
//Frame creation methods
void AnimationEditor::newFrame()
{
    if(m_currentAnimation)
    {
        AnimationFrame newFrame;
        newFrame.m_frameName = "Frame" + QString::number(frameCounter++);
        m_currentAnimation->addFrame(newFrame);
    }
    updateFramesView();
}

void AnimationEditor::deleteFrame()
{
    if(m_currentFrame)
    {
        m_currentAnimation->getFrames().erase(*m_currentFrame);
        m_currentFrame = nullptr;
    }
    updateFramesView();
}

void AnimationEditor::moveFrameUp()
{
    int distance = -1;
    if (m_currentAnimation && m_currentFrame)
    {
        distance = (int) std::distance(m_currentAnimation->getFrames().begin(), *m_currentFrame);
        if (distance > 0)
        {
            m_currentAnimation->getFrames().move(distance, distance - 1);
            --distance;
        }
    }
    updateFramesView();
    if (distance != -1)
        animationFramesView->setCurrentRow(distance, QItemSelectionModel::Select);
}

void AnimationEditor::moveFrameDown()
{
    int distance = -1;
    if(m_currentAnimation && m_currentFrame)
    {
        distance = (int)std::distance(m_currentAnimation->getFrames().begin(), *m_currentFrame);
        if(distance < m_currentAnimation->getFrames().size() - 1)
        {
            m_currentAnimation->getFrames().move(distance, distance + 1);
            ++distance;
        }
        else
            distance = m_currentAnimation->getFrames().size() - 1;
    }
    updateFramesView();
    if(distance != -1)
        animationFramesView->setCurrentRow(distance, QItemSelectionModel::Select);
}