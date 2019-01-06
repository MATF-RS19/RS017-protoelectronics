#include "scene.h"
#include "log_component.hpp"

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

void GridZone::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    Q_UNUSED(event);
}

void GridZone::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
    Q_UNUSED(event);
}

void GridZone::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
    Q_UNUSED(event);
}

void GridZone::dropEvent(QGraphicsSceneDragDropEvent *event) {
	// Taking from the side component which we drop on our grid
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
            QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
            QDataStream stream(&itemData, QIODevice::ReadOnly);

            int row, col;
            QMap<int, QVariant> valueMap;

            stream >> row >> col >> valueMap;

            if(!valueMap.isEmpty()) {
                QString componentType = valueMap.value(0).toString();
                qreal xV = round(event->scenePos().x()/gridSize)*gridSize;
                qreal yV = round(event->scenePos().y()/gridSize)*gridSize;
				QPointF newPos(xV, yV);

                if(componentType == "Wire") {
                    Wire* wire = new Wire();
                    //Set position
                    wire->setPos(newPos);

					//Establish connection
					wire->connect(wire->connectionPoints());

					// Add item
                    this->addItem(wire);
                }
				else if(componentType == "Resistor") {
					Resistor* resistor = new Resistor();
					resistor->setPos(newPos);
					resistor->connect(resistor->connectionPoints());
					this->addItem(resistor);
                }
				else if(componentType == "Ground") {
					Ground* gnd = new Ground();
					gnd->setPos(newPos);
					gnd->connect(gnd->connectionPoints());
					this->addItem(gnd);
                }
                else if (componentType == "DC Voltage") {
                    DCVoltage* dcv = new DCVoltage();
                    dcv->setPos(newPos);
					dcv->Component::connect(dcv->connectionPoints());
                    this->addItem(dcv);
                }
				else if (componentType == "Clock") {
					Clock* cl = new Clock();
					cl->setPos(newPos);
					cl->Component::connect(cl->connectionPoints());
					this->addItem(cl);
				}
                else if(componentType == "Switch") {
                    Switch* sw = new Switch();
                    sw->setPos(newPos);
					sw->Component::connect(sw->connectionPoints());
                    this->addItem(sw);
                }
                else if (componentType == "AND") {
                    ANDGate* andGate = new ANDGate();
                    andGate->setPos(newPos);
					andGate->Component::connect(andGate->connectionPoints());
                    this->addItem(andGate);
                }
                else if (componentType == "OR") {
                    ORGate* orGate = new ORGate();
                    orGate->setPos(newPos);
					orGate->Component::connect(orGate->connectionPoints());
                    this->addItem(orGate);
                }
                else if (componentType == "XOR") {
                    XORGate* xorGate = new XORGate();
                    xorGate->setPos(newPos);
					xorGate->Component::connect(xorGate->connectionPoints());
                    this->addItem(xorGate);
                }
                else if (componentType == "NAND") {
                    NANDGate* nandGate = new NANDGate();
                    nandGate->setPos(newPos);
					nandGate->Component::connect(nandGate->connectionPoints());
                    this->addItem(nandGate);
                }
                else if (componentType == "NOR") {
                    NORGate* norGate = new NORGate();
                    norGate->setPos(newPos);
					norGate->Component::connect(norGate->connectionPoints());
                    this->addItem(norGate);
                }
                else if (componentType == "NXOR") {
                    NXORGate* nxorGate = new NXORGate();
                    nxorGate->setPos(newPos);
					nxorGate->Component::connect(nxorGate->connectionPoints());
                    this->addItem(nxorGate);
                }
                else if (componentType == "NOT") {
                    NOTGate* notGate = new NOTGate();
                    notGate->setPos(newPos);
					notGate->Component::connect(notGate->connectionPoints());
                    this->addItem(notGate);
                }
				else if (componentType == "JK Flip Flop") {
					JKFlipFlop* jkFlipFlop = new JKFlipFlop();
					jkFlipFlop->setPos(newPos);
					jkFlipFlop->Component::connect(jkFlipFlop->connectionPoints());
					this->addItem(jkFlipFlop);
				}
				else if (componentType == "Decoder") {
					Decoder* decoder = new Decoder();
					decoder->setPos(newPos);
					//decoder->Component::connect(decoder->connectionPoints());
					this->addItem(decoder);
				}
				else if (componentType == "LCD Display") {
					LCDDisplay* lcdDisplay = new LCDDisplay();
					lcdDisplay->setPos(newPos);
					//lcdDisplay->Component::connect(lcdDisplay->connectionPoints());
					this->addItem(lcdDisplay);
				}
			}
            event->accept();
    }
    else event->ignore();
}

void GridZone::keyPressEvent(QKeyEvent *event) {
    // On pressed delete key removing item from scene
    if(event->key() == Qt::Key_Delete) {
        foreach(QGraphicsItem *item, this->selectedItems())
            if(Component *rItem = qgraphicsitem_cast<Component*> (item))
                delete item;
        this->clearSelection();
    }
}
