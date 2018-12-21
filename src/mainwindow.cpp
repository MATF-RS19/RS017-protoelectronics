#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("ProtoElectronics");
    resize(1000, 800);

    createListWidget();
    createSceneAndView();
    createGraphicsItems();
    createLayout();
}

void MainWindow::createListWidget() {
    itemListWidget = new QListWidget;

    QStringList itemList;
    itemList << "Wire" << "Resistor" << "Ground" << "DC Voltage" << "AND gate";
    itemListWidget->addItems(itemList);
    itemListWidget->setFixedWidth(120);

    itemListWidget->setDragEnabled(true);
}

void MainWindow::createSceneAndView() {
    scene = new GridZone(this);
    scene->setSceneRect(0,0,600,400);
    view = new QGraphicsView();
    view->setScene(scene);
}

void MainWindow::createGraphicsItems(){
    Ground *ground = new Ground();
    scene->addItem(ground);

    Wire* wire = new Wire;
    wire->setPos(ground->pos()+QPointF(120,0));
    scene->addItem(wire);

    Resistor* resistor = new Resistor(1000);
    resistor->setPos(wire->pos()+QPointF(120, 0));
    scene->addItem(resistor);

    DCVoltage* dcVoltage = new DCVoltage(10);
    dcVoltage->setPos(resistor->pos()+QPointF(120, 0));
    scene->addItem(dcVoltage);
/*
    VoltageSource* voltageSource = new VoltageSource;
    voltageSource->setPos(dcVoltage->pos()+QPointF(120, 0));
    scene->addItem(voltageSource);
*/
    ANDGate* andComponent = new ANDGate();
    andComponent->setPos(ground->pos()+QPointF(0, 140));
    scene->addItem(andComponent);

    NANDGate* nandComponent = new NANDGate();
    nandComponent->setPos(andComponent->pos()+QPointF(160, 0));
    scene->addItem(nandComponent);

    ORGate* orComponent = new ORGate();
    orComponent->setPos(nandComponent->pos()+QPointF(160, 0));
    scene->addItem(orComponent);

    NORGate* norComponent = new NORGate();
    norComponent->setPos(andComponent->pos()+QPointF(0, 150));
    scene->addItem(norComponent);

    XORGate* xorComponent = new XORGate();
    xorComponent->setPos(norComponent->pos()+QPointF(160, 0));
    scene->addItem(xorComponent);

    NXORGate* nxorComponent = new NXORGate();
    nxorComponent->setPos(xorComponent->pos()+QPointF(160, 0));
    scene->addItem(nxorComponent);

    NOTGate* notComponent = new NOTGate();
    notComponent->setPos(norComponent->pos()+QPointF(0, 160));
    scene->addItem(notComponent);
}

void MainWindow::createLayout(){
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
    frameLayout->addWidget(itemListWidget);
    frameLayout->addWidget(view);
    setCentralWidget(frame);
}
