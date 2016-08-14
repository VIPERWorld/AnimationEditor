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
    void setTrackedItem(QGraphicsRectItem*);
signals:
    void mouseMoved(QVector2D);
    void itemPositionChanged(QVector2D);
protected:
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);

    QGraphicsRectItem *trackedItem{nullptr};
    QPointF lastMousePosition;
    bool isDragging{false};
};


#endif //ANIMATIONEDITOR_ANIMATIONEDITORVIEW_H
