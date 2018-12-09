#include "component.h"

Component::Component() {
    setFlags(QGraphicsItem::ItemIsSelectable |
            QGraphicsItem::ItemIsMovable |
            QGraphicsItem::ItemSendsGeometryChanges);

    penForLines = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap);
    penForDots = QPen(Qt::white, 6, Qt::SolidLine, Qt::RoundCap);
}

QVariant Component::itemChange(GraphicsItemChange change,
const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        QPointF newPos = value.toPointF();
        if(QApplication::mouseButtons() == Qt::LeftButton &&
                qobject_cast<GridZone*> (scene())){

            //penForLines.setColor(QColor(8, 246, 242));
            GridZone* customScene = qobject_cast<GridZone*> (scene());
            int gridSize = customScene->getGridSize();
            qreal xV = round(newPos.x()/gridSize)*gridSize;
            qreal yV = round(newPos.y()/gridSize)*gridSize;
            return QPointF(xV, yV);
        }
        else {
            return newPos;
        }
    }
    else {
        return QGraphicsItem::itemChange(change, value);
    }
}

QRectF Component::boundingRect() const {
    return QRectF(0,0,100,100);
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //painter->drawRect(boundingRect());
}

Ground::Ground() {

}

void Ground::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Component::paint(painter, option, widget);
    painter->setPen(penForLines);

    QLineF lineV(50, 60, 50, 0);
    QLineF lineH1(20, 60, 80, 60);
    QLineF lineH2(30, 70, 70, 70);
    QLineF lineH3(40, 80, 60, 80);

    painter->drawLine(lineV);
    painter->drawLine(lineH1);
    painter->drawLine(lineH2);
    painter->drawLine(lineH3);
}

Wire::Wire() {

}

void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Component::paint(painter, option, widget);
    painter->setPen(penForLines);
    QLineF line(0, 50, 100, 50);
    painter->drawLine(line);

    painter->setPen(penForDots);
    QPointF p1(0, 50);
    QPointF p2(100, 50);
    painter->drawPoint(p1);
    painter->drawPoint(p2);
}

Resistor::Resistor() {

}

void Resistor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Component::paint(painter, option, widget);

    painter->setPen(penForLines);
    QLineF lineStart(0, 50, 6, 50);
    QLineF lineZig1(6, 50, 17, 25);
    QLineF lineZig2(17, 25, 28, 75);
    QLineF lineZig3(28, 75, 39, 25);
    QLineF lineZig4(39, 25, 50, 75);
    QLineF lineZig5(50, 75, 61, 25);
    QLineF lineZig6(61, 25, 72, 75);
    QLineF lineZig7(72, 75, 83, 25);
    QLineF lineZig8(83, 25, 94, 50);
    QLineF lineEnd(94, 50, 100, 50);

    painter->drawLine(lineStart);
    painter->drawLine(lineZig1);
    painter->drawLine(lineZig2);
    painter->drawLine(lineZig3);
    painter->drawLine(lineZig4);
    painter->drawLine(lineZig5);
    painter->drawLine(lineZig6);
    painter->drawLine(lineZig7);
    painter->drawLine(lineZig8);
    painter->drawLine(lineEnd);

    painter->setFont(QFont("Times", 12, QFont::Bold));
    painter->drawText(rec, Qt::AlignHCenter, "1k");

    painter->setPen(penForDots);
    QPointF p1(0, 50);
    QPointF p2(100, 50);
    painter->drawPoint(p1);
    painter->drawPoint(p2);
}

DCVoltage::DCVoltage() {

}

void DCVoltage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Component::paint(painter, option, widget);

    painter->setPen(penForLines);

    QLineF line(50, 0, 50, 40);
    painter->drawLine(line);

    painter->setPen(penForDots);

    QPointF p(50, 0);
    painter->drawPoint(p);

    painter->setPen(QPen(Qt::black));
    painter->setFont(QFont("Times", 12, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter, "+5V");
}

VoltageSource::VoltageSource() {

};

void VoltageSource::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Component::paint(painter, option, widget);

    painter->setPen(penForLines);

    QLineF lineUpVertical(50,0, 50, 45);
    QLineF lineUpHorizontal(25, 45, 75, 45);
    QLineF lineDownHorizontal(35, 55, 65, 55);
    QLineF lineDownVertical(50, 55, 50, 100);

    painter->drawLine(lineUpVertical);
    painter->drawLine(lineUpHorizontal);
    painter->drawLine(lineDownHorizontal);
    painter->drawLine(lineDownVertical);

    painter->setPen(penForDots);

    QPointF pUp(50,0);
    QPointF pDown(50,100);
    painter->drawPoint(pUp);
    painter->drawPoint(pDown);
}
