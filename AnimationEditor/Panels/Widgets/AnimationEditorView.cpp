#include "AnimationEditorView.h"
#include <QDebug>
#include <iostream>

AnimationEditorView::AnimationEditorView(QWidget *parent)
:
    QGraphicsView(parent)
{
    setMouseTracking(true);
}

void AnimationEditorView::setTrackedItem(QGraphicsRectItem *item)
{
    trackedItem = item;
}

void AnimationEditorView::mouseMoveEvent(QMouseEvent *event)
{
    if(isDragging && trackedItem)
    {
        QPointF newMousePosition = mapToScene(event->pos());
        QPointF diff = newMousePosition - lastMousePosition;
        emit itemPositionChanged(QVector2D((float)diff.x(), (float)diff.y()));
        lastMousePosition = newMousePosition;
    }
    emit mouseMoved(QVector2D(mapToScene(event->x(), event->y())));
    QGraphicsView::mouseMoveEvent(event);
}


void AnimationEditorView::mousePressEvent(QMouseEvent *event)
{
    if(trackedItem)
    {
        if(trackedItem->boundingRect().contains(mapToScene(event->pos())))
        {
            isDragging = true;
            lastMousePosition = mapToScene(event->pos());
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void AnimationEditorView::mouseReleaseEvent(QMouseEvent *event)
{
    isDragging = false;
    QGraphicsView::mouseReleaseEvent(event);
}