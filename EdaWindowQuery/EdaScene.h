#ifndef EDASCENE_H
#define EDASCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "SpatialIndex.h"

class EdaScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit EdaScene(QObject *parent = nullptr);
    void generateLayoutObjects(int count);
    void setUseSpatialIndex(bool use) { m_useSpatialIndex = use; }
    
    int getSelectedCount() const { return m_selectedCount; }
    double getLastQueryTimeMs() const { return m_lastQueryTimeMs; }

signals:
    void queryCompleted();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

private:
    void updateQueryWindow();
    void executeQuery();

    QVector<LayoutObject> m_layoutData;
    QVector<QGraphicsRectItem*> m_graphicsItems;

    QGraphicsRectItem* m_queryWindowItem = nullptr;
    SpatialQuadtree m_spatialIndex;

    bool m_isDragging = false;
    QPointF m_dragStart;
    bool m_useSpatialIndex = true;

    int m_selectedCount = 0;
    double m_lastQueryTimeMs = 0.0;
};

#endif