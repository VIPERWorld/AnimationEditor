#include "AnimationEditorView.h"
#include <QDebug>
#include <iostream>

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
    if(isDragging && trackedItem)
    {
        auto newMousePosition = event->pos();
        auto diff = newMousePosition - lastMousePosition;
        emit itemPositionChanged(QVector2D(trackedItem->scenePos().x() + diff.x(), trackedItem->scenePos().y() + diff.y()));
    }
    emit mouseMoved(QVector2D(event->x(), event->y()));
    QGraphicsView::mouseMoveEvent(event);
}


void AnimationEditorView::mousePressEvent(QMouseEvent *event)
{
    if(trackedItem)
    {
        if(trackedItem->boundingRect().contains(event->pos()))
        {
            isDragging = true;
            lastMousePosition = event->pos();
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void AnimationEditorView::mouseReleaseEvent(QMouseEvent *event)
{
    isDragging = false;
    QGraphicsView::mouseReleaseEvent(event);
}