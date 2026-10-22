#ifndef SPATIALINDEX_H
#define SPATIALINDEX_H

#include <QRectF>
#include <QVector>
#include <memory>


struct LayoutObject {
    int id;
    QRectF bounds;
    bool isIntersected = false;
};

class SpatialQuadtree {
private:
    static constexpr int MAX_OBJECTS = 8;
    static constexpr int MAX_LEVELS = 6;

    int level;
    QRectF bounds;
    QVector<LayoutObject*> objects;
    std::unique_ptr<SpatialQuadtree> nodes[4];

    void split() {
        qreal subWidth = bounds.width() / 2.0;
        qreal subHeight = bounds.height() / 2.0;
        qreal x = bounds.x();
        qreal y = bounds.y();

        nodes[0] = std::make_unique<SpatialQuadtree>(level + 1, QRectF(x + subWidth, y, subWidth, subHeight));
        nodes[1] = std::make_unique<SpatialQuadtree>(level + 1, QRectF(x, y, subWidth, subHeight));
        nodes[2] = std::make_unique<SpatialQuadtree>(level + 1, QRectF(x, y + subHeight, subWidth, subHeight));
        nodes[3] = std::make_unique<SpatialQuadtree>(level + 1, QRectF(x + subWidth, y + subHeight, subWidth, subHeight));
    }

    int getIndex(const QRectF& rect) const {
        int index = -1;
        qreal verticalMidpoint = bounds.x() + (bounds.width() / 2.0);
        qreal horizontalMidpoint = bounds.y() + (bounds.height() / 2.0);

        bool topQuadrant = (rect.y() < horizontalMidpoint && rect.y() + rect.height() < horizontalMidpoint);
        bool bottomQuadrant = (rect.y() > horizontalMidpoint);

        if (rect.x() < verticalMidpoint && rect.x() + rect.width() < verticalMidpoint) {
            if (topQuadrant) index = 1;
            else if (bottomQuadrant) index = 2;
        } else if (rect.x() > verticalMidpoint) {
            if (topQuadrant) index = 0;
            else if (bottomQuadrant) index = 3;
        }

        return index;
    }

public:
    SpatialQuadtree(int pLevel, const QRectF& pBounds) : level(pLevel), bounds(pBounds) {}

    void clear() {
        objects.clear();
        for (auto& node : nodes) {
            if (node) {
                node->clear();
                node.reset();
            }
        }
    }

    void insert(LayoutObject* obj) {
        if (nodes[0]) {
            int index = getIndex(obj->bounds);
            if (index != -1) {
                nodes[index]->insert(obj);
                return;
            }
        }

        objects.append(obj);

        if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
            if (!nodes[0]) split();

            int i = 0;
            while (i < objects.size()) {
                int index = getIndex(objects[i]->bounds);
                if (index != -1) {
                    nodes[index]->insert(objects.takeAt(i));
                } else {
                    i++;
                }
            }
        }
    }

    void queryWindow(const QRectF& queryArea, QVector<LayoutObject*>& result) const {
        if (!bounds.intersects(queryArea)) return;

        for (LayoutObject* obj : objects) {
            if (queryArea.intersects(obj->bounds)) {
                result.append(obj);
            }
        }

        if (!nodes[0]) return;

        for (int i = 0; i < 4; ++i) {
            if (nodes[i]) {
                nodes[i]->queryWindow(queryArea, result);
            }
        }
    }
};

#endif // SPATIALINDEX_H