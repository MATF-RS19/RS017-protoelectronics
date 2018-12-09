#include "scene.h"

GridZone::GridZone(QObject* parent) :
    QGraphicsScene(parent), gridSize(10)
{
    Q_ASSERT(gridSize > 0);
}

void GridZone::drawBackground(QPainter *painter, const QRectF &rect)
{
    QPen pen;
    painter->setPen(pen);

    //Background color
    painter->setBrush(Qt::darkGray);
    painter->fillRect(rect,  painter->brush());

    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);
    QVector<QPointF> points;
    for (qreal x = left; x < rect.right(); x += gridSize){
        for (qreal y = top; y < rect.bottom(); y += gridSize){
            points.append(QPointF(x,y));
        }
    }
    painter->drawPoints(points.data(), points.size());

}
