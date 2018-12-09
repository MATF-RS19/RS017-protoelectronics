#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include <QApplication>

class GridZone : public QGraphicsScene
{
    Q_OBJECT
public:
    GridZone(QObject* parent = nullptr);
    int getGridSize() const {return this->gridSize;}

protected:
    void drawBackground(QPainter* painter, const QRectF &rect);
private:
    int gridSize;
};

#endif // SCENE_H
