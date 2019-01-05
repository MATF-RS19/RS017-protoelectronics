#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	// Window title and size
    setWindowTitle("ProtoElectronics");
    resize(1000, 800);

	// Creating gui
    createListWidget();
    createSceneAndView();
    createLayout();
}

void MainWindow::createListWidget() {
	// Creating on left side list of components for dragging
    itemListWidget = new QListWidget;

    QStringList itemList;
    itemList << "Wire" << "Resistor" << "Ground" <<
                "DC Voltage" << "Switch" << "AND" << "OR" <<
                "XOR" << "NAND" << "NOR" <<
                "NXOR" << "NOT";
    itemListWidget->addItems(itemList);
    itemListWidget->setFixedWidth(120);

    itemListWidget->setDragEnabled(true);
}

void MainWindow::createSceneAndView() {
	// Creating middle part
    scene = new GridZone(this);
    scene->setSceneRect(0,0,600,400);
    view = new QGraphicsView();
    view->setScene(scene);
}

void MainWindow::createLayout(){
	// Creating main layout: list of widgets and view
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
    frameLayout->addWidget(itemListWidget);
    frameLayout->addWidget(view);

	// Adding label for printing properties
	propertiesMessage = new QLabel();
	propertiesMessage->setAlignment(Qt::AlignBottom);
	propertiesMessage->setFixedWidth(130);
	frameLayout->addWidget(propertiesMessage);

    setCentralWidget(frame);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    // On pressed escape key window is closed
	if(event->key() == Qt::Key_Escape){
			scene->clear();
			qApp->quit();
	}
	QMainWindow::keyPressEvent(event);
}
