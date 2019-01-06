#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

MainWindow::~MainWindow() {}

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
				"DC Voltage" << "Clock" << "Switch" << "AND" << "OR" <<
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

	// Buttons OPEN & SAVE
	buttonBox = new QDialogButtonBox(Qt::Vertical);

	openFileButton = new QPushButton(tr("&Open"));
	openFileButton->setDefault(true);

	saveFileButton = new QPushButton(tr("&Save"));
	saveFileButton->setDefault(true);

    buttonBox->addButton(openFileButton, QDialogButtonBox::ApplyRole);
    buttonBox->addButton(saveFileButton, QDialogButtonBox::ApplyRole);

	// Setting fixed widt and stylesheet
	buttonBox->setFixedWidth(130);
	buttonBox->setStyleSheet("background-color: rgb(8, 246, 242); border-radius: 10px; padding: 6px; ");

	// Connecting buttons with slots
	connect(this->openFileButton, SIGNAL(clicked(bool)), this, SLOT(onOpenFile()));
	connect(this->saveFileButton, SIGNAL(clicked(bool)), this, SLOT(onSaveFile()));

	// Label for printing properties
	propertiesMessage = new QLabel();
	propertiesMessage->setAlignment(Qt::AlignBottom);
	propertiesMessage->setFixedWidth(130);
	frameLayout->addWidget(propertiesMessage);

	// Layouts
	QVBoxLayout *rightLayout = new QVBoxLayout;
	rightLayout->addWidget(buttonBox);
	rightLayout->addWidget(propertiesMessage);
	rightLayout->setSpacing(8);
	frameLayout->addLayout(rightLayout);

    setCentralWidget(frame);
}

void MainWindow::onOpenFile() {
	// Open already existing scheme

	// TODO
	QString filename = QFileDialog::getOpenFileName(
				this,
				tr("Open File"),
				"/Users",
				"All files (*.*);;Text File (*.txt)"
				);

	qDebug() << filename;

}

void MainWindow::onSaveFile() {
	// Save the scheme
         QJsonArray or_components;
         QJsonArray xor_components;
         QJsonArray and_components;
         QJsonArray nand_components;
         QJsonArray nor_components;
         QJsonArray nxor_components;
         QJsonArray not_components;
         QJsonArray wire_components;
         QJsonArray dcvoltage_components;
         QJsonArray clock_components;
         QJsonArray resistor_components;
         QJsonArray switch_components;
         QJsonArray ground_components;
         QList<QGraphicsItem*> allItems = scene->items();
         for (int i = 0; i < allItems.size(); ++i) {
              QGraphicsItem *component = allItems[i];
              if(component->parentItem() == nullptr){
                       qreal x = component->x();
                       qreal y = component->y();
                       //std::cout << "x: " << x  << " y: " << y << std::endl;
                       QJsonArray points;
                       points.push_back(x);
                       points.push_back(y);
                       Component *rItem = qgraphicsitem_cast<Component*> (component);
                       std::string type = rItem->componentType();
                       //std::cout << "Type: " << rItem->componentType() << std::endl;
                       if (type == "or"){
                          or_components.push_back(points);
                       }
                       if (type == "nor"){
                          nor_components.push_back(points);
                       }
                       if (type == "and"){
                          and_components.push_back(points);
                       }
                       if (type == "nand"){
                          nand_components.push_back(points);
                       }
                       if (type == "xor"){
                          xor_components.push_back(points);
                       }
                       if (type == "nxor"){
                          nxor_components.push_back(points);
                       }
                       if( type == "not"){
                          not_components.push_back(points);
                       }
                       if(type == "voltage"){
                          //std::cout << rItem->voltage() << std::endl;
                          points.push_back(rItem->voltage());
                          dcvoltage_components.push_back(points);
                       }
                       if(type == "resistor"){
                          Resistor* rItem = static_cast<Resistor*>(rItem);
                          //std::cout << rItem->resistance() << std::endl;
                          points.push_back(rItem->resistance());
                          resistor_components.push_back(points);
                       }
                       if(type == "clock"){
                          //std::cout << "Voltage: " << rItem->voltage() << std::endl;
                          Clock* rItem = static_cast<Clock*>(rItem);
                          //std::cout << "Interval: " << rItem->timeInterval() <<std::endl;
                          int interval = rItem->timeInterval();
                          double voltage = rItem->oldVoltage();

                          points.push_back(voltage);
                          points.push_back(interval);
                          clock_components.push_back(points);
                       }
                       if (type == "ground"){
                          ground_components.push_back(points);
                       }
                       if (type == "wire"){
                          wire_components.push_back(points);
                       }

              }
            }
            QJsonObject topQuery;
            topQuery["or"] = or_components;
            topQuery["nor"] = nor_components;
            topQuery["and"] = and_components;
            topQuery["nand"] = nand_components;
            topQuery["xor"] = xor_components;
            topQuery["nxor"] = nxor_components;
            topQuery["not"] = not_components;
            topQuery["clock"] = clock_components;
            topQuery["wire"] = wire_components;
            topQuery["ground"] = ground_components;
            topQuery["voltage"] = dcvoltage_components;
            topQuery["resistor"] = resistor_components;
            QJsonDocument document(topQuery);
            //qDebug() << document.toJson();

	// TODO
    //QString fileName = QFileDialog::getSaveFileName(
      //          new MainWindow(),
        //        tr("Save as"),
          //      "*.json"
            //    );
    //qDebug() << "A";
    //if(!fileName.isEmpty()) {
        currentFile = QString("sema2.json");
        saveFile(document);
    //}

    //qDebug() << fileName;

}


void MainWindow::saveFile(QJsonDocument document) {
	// TODO
    //QString text = "save me";


	QFile file(currentFile);

    if(file.open(QFile::WriteOnly)) {
        file.write(document.toJson());
	}
	else {
		QMessageBox::warning(
					this,
					"TextEditor",
					tr("Cannot write file %1.\nError: %2")
					.arg(currentFile)
					.arg(file.errorString())
					);
	}
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    // On pressed escape key window is closed
	if(event->key() == Qt::Key_Escape){
			scene->clear();
			qApp->quit();
	}
	QMainWindow::keyPressEvent(event);
}
