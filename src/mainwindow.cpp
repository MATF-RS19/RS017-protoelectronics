#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

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
				"NXOR" << "NOT" <<
				"JK Flip Flop" << "Decoder" << "LCD Display";

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

	// TODO
	QString fileName = QFileDialog::getSaveFileName(
				this,
				tr("Save as"),
				"*.json"
				);

	if(!fileName.isEmpty()) {
		currentFile = fileName;
		saveFile();
	}

	qDebug() << fileName;

}

void MainWindow::saveFile() {
	// TODO
	QString text = "save me";


	QFile file(currentFile);

	if(file.open(QFile::WriteOnly)) {
		file.write(text.toUtf8());
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
