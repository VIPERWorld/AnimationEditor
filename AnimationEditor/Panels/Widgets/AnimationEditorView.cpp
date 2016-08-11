#include "AnimationEditorView.h"

AnimationEditorView::AnimationEditorView(QWidget *parent)
:
    QGraphicsView(parent)
{
    setMouseTracking(true);
}

void AnimationEditorView::setTrackedItem(QGraphicsItem *item)
{
    trackedItem = item;
}

void AnimationEditorView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(QVector2D(event->x(), event->y()));
}

void AnimationEditorView::dragEnterEvent(QDragEnterEvent *event)
{
    if(trackedItem)
    {
        if(trackedItem->boundingRect().contains(event->pos()))
            isDragging = true;
    }
}

void AnimationEditorView::dragMoveEvent(QDragMoveEvent *)
{
    if(isDragging)
        emit itemMoved(trackedItem);
}

void AnimationEditorView::dragLeaveEvent(QDragLeaveEvent *event)
{
    isDragging = false;
}