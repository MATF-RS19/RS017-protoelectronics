#include "logcomponent.h"

LOGComponent::LOGComponent() {
    setFlags(QGraphicsItem::ItemIsSelectable |
            QGraphicsItem::ItemIsMovable |
            QGraphicsItem::ItemSendsGeometryChanges);

    penForLines = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap);
    penForDots = QPen(Qt::white, 6, Qt::SolidLine, Qt::RoundCap);
}

QVariant LOGComponent::itemChange(GraphicsItemChange change,
const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        QPointF newPos = value.toPointF();
        if(QApplication::mouseButtons() == Qt::LeftButton &&
                qobject_cast<GridZone *> (scene())){
            GridZone* customScene = qobject_cast<GridZone *> (scene());
            int gridSize = customScene->getGridSize();
            qreal xV = round(newPos.x()/gridSize)*gridSize;
            qreal yV = round(newPos.y()/gridSize)*gridSize;
            return QPointF(xV, yV);
        }
        else
            return newPos;

    }
    else
        return QGraphicsItem::itemChange(change, value);

}

QRectF LOGComponent::boundingRect() const {
    return QRectF(0, 0, 180, 120);
}

void LOGComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawRect(boundingRect());
}

ANDComponent::ANDComponent() {

}

void ANDComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //LOGComponent::paint(painter, option, widget);

    painter->setPen(penForLines);

    //first input lead
    //TODO if input voltage > 0 - color lead in green
    //if voltage < 0 - color in dark red
    painter->drawLine(0,30,50,30);
    //second input
    painter->drawLine(0,90,50,90);
    //component body
    painter->drawLine(50,10,50,110);
    painter->drawLine(50,10,100,10);
    painter->drawLine(50,110,100,110);
    painter->drawArc(QRect(62,10,75,100), -90*16, 180*16);
    //output lead
    painter->drawLine(137,60,180,60);

    //connection points
    painter->setPen(penForDots);
    QPointF in1(0.5, 30);
    QPointF in2(0.5, 90);
    QPointF out(179.5, 60);
    painter->drawPoint(in1);
    painter->drawPoint(in2);
    painter->drawPoint(out);
}

NANDComponent::NANDComponent() {

}

void NANDComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //LOGComponent::paint(painter, option, widget);

    painter->setPen(penForLines);

    //first input lead
    //TODO if input voltage > 0 - color lead in green
    //if voltage < 0 - color in dark red
    painter->drawLine(0,30,50,30);
    //second input
    painter->drawLine(0,90,50,90);
    //component body
    painter->drawLine(50,10,50,110);
    painter->drawLine(50,10,100,10);
    painter->drawLine(50,110,100,110);
    painter->drawArc(QRect(62,10, 75,100), -90*16, 180*16);
    //output lead
    painter->drawLine(140,60,180,60);

    //connection points
    painter->setPen(penForDots);
    QPointF in1(0.5, 30);
    QPointF in2(0.5, 90);
    QPointF out(179.5, 60);
    painter->drawPoint(in1);
    painter->drawPoint(in2);
    painter->drawPoint(out);

    QPainterPath path;
    path.addEllipse(QPointF(139, 60), 1, 1);
    painter->drawPath(path);
}

ORComponent::ORComponent() {

}

void ORComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //LOGComponent::paint(painter, option, widget);

    painter->setPen(penForLines);

    //first input lead
    //TODO if input voltage > 0 - color lead in green
    //if voltage < 0 - color in dark red
    painter->drawLine(0,30,50,30);
    //second input
    painter->drawLine(0,90,50,90);
    //component body
    painter->drawLine(50,10,50,110);
    painter->drawLine(50,10,100,10);
    painter->drawLine(50,110,100,110);
    //output lead
    painter->drawLine(137,60,180,60);

    //connection points
    painter->setPen(penForDots);
    QPointF in1(0.5, 30);
    QPointF in2(0.5, 90);
    QPointF out(179.5, 60);
    painter->drawPoint(in1);
    painter->drawPoint(in2);
    painter->drawPoint(out);
}


