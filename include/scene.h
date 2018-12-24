#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include <QApplication>
#include <QKeyEvent>
#include <QMimeData>

class GridZone : public QGraphicsScene
{
    Q_OBJECT
public:
    GridZone(QObject* parent = nullptr);
    int getGridSize() const {return this->gridSize;}

protected:
    void drawBackground(QPainter* painter, const QRectF &rect) override;

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
private:
    int gridSize;
};

#endif // SCENE_H
