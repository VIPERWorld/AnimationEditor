#ifndef ANIMATIONEDITOR_ANIMATIONEDITORVIEW_H
#define ANIMATIONEDITOR_ANIMATIONEDITORVIEW_H


#include <QtWidgets/QGraphicsView>
#include <QtGui/QDragEnterEvent>
#include <QGraphicsItem>

class AnimationEditorView : public QGraphicsView
{
    Q_OBJECT;
public:
    explicit AnimationEditorView(QWidget*);
    void setTrackedItem(QGraphicsItem*);
signals:
    void mouseMoved(QVector2D);
    void itemMoved(QGraphicsItem*);
protected:
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void dragEnterEvent(QDragEnterEvent*);
    virtual void dragMoveEvent(QDragMoveEvent*);
    virtual void dragLeaveEvent(QDragLeaveEvent*);

    QGraphicsItem *trackedItem{nullptr};
    bool isDragging{false};
};


#endif //ANIMATIONEDITOR_ANIMATIONEDITORVIEW_H
