#include "EdaScene.h"
#include <QBrush>
#include <QPen>
#include <QRandomGenerator>
#include <QElapsedTimer>

EdaScene::EdaScene(QObject *parent)
    : QGraphicsScene(parent), m_spatialIndex(0, QRectF(0, 0, 800, 600)) {
    
    setSceneRect(0, 0, 800, 600);
    setBackgroundBrush(Qt::white);


    m_queryWindowItem = addRect(180, 130, 140, 140, 
        QPen(QColor(0, 102, 204), 2, Qt::DashLine), 
        QBrush(QColor(0, 102, 204, 40)));
    m_queryWindowItem->setZValue(10); 
}

void EdaScene::generateLayoutObjects(int count) {
    clear();
    m_layoutData.clear();
    m_graphicsItems.clear();
    m_spatialIndex.clear();

    m_queryWindowItem = addRect(180, 130, 140, 140, 
        QPen(QColor(0, 102, 204), 2, Qt::DashLine), 
        QBrush(QColor(0, 102, 204, 40)));
    m_queryWindowItem->setZValue(10);

    QRandomGenerator *rng = QRandomGenerator::global();

    for (int i = 0; i < count; ++i) {
        LayoutObject obj;
        obj.id = i;
        qreal x = rng->bounded(20, 720);
        qreal y = rng->bounded(20, 520);
        qreal w = rng->bounded(15, 60);
        qreal h = rng->bounded(15, 60);

        obj.bounds = QRectF(x, y, w, h);
        m_layoutData.append(obj);


        QGraphicsRectItem* item = addRect(obj.bounds, 
            QPen(QColor(180, 180, 180), 1), 
            QBrush(QColor(240, 240, 240)));
        m_graphicsItems.append(item);
    }


    for (int i = 0; i < m_layoutData.size(); ++i) {
        m_spatialIndex.insert(&m_layoutData[i]);
    }

    executeQuery();
}

void EdaScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (m_queryWindowItem->rect().contains(event->scenePos())) {
        m_isDragging = true;
        m_dragStart = event->scenePos();
    }
    QGraphicsScene::mousePressEvent(event);
}

void EdaScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_isDragging) {
        QPointF delta = event->scenePos() - m_dragStart;
        m_dragStart = event->scenePos();

        QRectF r = m_queryWindowItem->rect();
        r.translate(delta);
        m_queryWindowItem->setRect(r);

        executeQuery(); 
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void EdaScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    m_isDragging = false;
    QGraphicsScene::mouseReleaseEvent(event);
}

void EdaScene::wheelEvent(QGraphicsSceneWheelEvent *event) {
    QRectF r = m_queryWindowItem->rect();
    
    qreal scaleFactor = (event->delta() > 0) ? 1.1 : 0.9;

    QPointF center = r.center();
    qreal newWidth = qMax(30.0, r.width() * scaleFactor);   
    qreal newHeight = qMax(30.0, r.height() * scaleFactor); 

    r.setWidth(newWidth);
    r.setHeight(newHeight);
    r.moveCenter(center);

    m_queryWindowItem->setRect(r);
    executeQuery();

    event->accept();
}

void EdaScene::executeQuery() {
    QRectF queryRect = m_queryWindowItem->rect();
    
    for (auto* item : m_graphicsItems) {
        item->setPen(QPen(QColor(180, 180, 180), 1));
        item->setBrush(QBrush(QColor(240, 240, 240)));
    }

    QElapsedTimer timer;
    timer.start();

    QVector<LayoutObject*> hitObjects;

    if (m_useSpatialIndex) {
        m_spatialIndex.queryWindow(queryRect, hitObjects);
    } else {
        for (int i = 0; i < m_layoutData.size(); ++i) {
            if (queryRect.intersects(m_layoutData[i].bounds)) {
                hitObjects.append(&m_layoutData[i]);
            }
        }
    }

    m_lastQueryTimeMs = timer.nsecsElapsed() / 1000000.0;
    m_selectedCount = hitObjects.size();

    for (LayoutObject* obj : hitObjects) {
        m_graphicsItems[obj->id]->setPen(QPen(QColor(0, 153, 76), 2));
        m_graphicsItems[obj->id]->setBrush(QBrush(QColor(153, 255, 153)));
    }

    emit queryCompleted();
}