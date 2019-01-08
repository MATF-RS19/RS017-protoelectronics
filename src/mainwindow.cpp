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
    this->scene->clear();
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "/Users",
                "Text File (*.json)"
                );

    qDebug() << filename;
    QString val;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject data = document.object();

    QJsonArray and_components = data["and"].toArray();
    std::for_each(and_components.begin(),and_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        ANDGate *gate = new ANDGate();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray nand_components = data["nand"].toArray();
    std::for_each(nand_components.begin(),nand_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        NANDGate *gate = new NANDGate();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });

    QJsonArray or_components = data["or"].toArray();
    std::for_each(or_components.begin(),or_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        ORGate *gate = new ORGate();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray nor_components = data["nor"].toArray();
    std::for_each(nor_components.begin(),nor_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        NORGate *gate = new NORGate();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray xor_components = data["xor"].toArray();
    std::for_each(xor_components.begin(),xor_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        XORGate *gate = new XORGate();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray nxor_components = data["nxor"].toArray();
    std::for_each(nxor_components.begin(),nxor_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        NXORGate *gate = new NXORGate();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray not_components = data["not"].toArray();
    std::for_each(not_components.begin(),not_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        NOTGate *gate = new NOTGate();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray lcd_components = data["lcd"].toArray();
    std::for_each(lcd_components.begin(),lcd_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        LCDDisplay *gate = new LCDDisplay();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray decoder_components = data["decoder"].toArray();
    std::for_each(decoder_components.begin(),decoder_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        Decoder *gate = new Decoder();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray flipflop_components = data["flipflop"].toArray();
    std::for_each(flipflop_components.begin(),flipflop_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        JKFlipFlop *gate = new JKFlipFlop();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray switch_components = data["switch"].toArray();
    std::for_each(switch_components.begin(),switch_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        Switch *gate = new Switch();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        if (!points[3].toBool()){
            gate->close();
        }
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray ground_components = data["ground"].toArray();
    std::for_each(ground_components.begin(),ground_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        Ground *gate = new Ground();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray clock_components = data["clock"].toArray();
    std::for_each(clock_components.begin(),clock_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        Clock *gate = new Clock(5,points[3].toInt());
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->Component::connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray wire_components= data["wire"].toArray();
    std::for_each(wire_components.begin(),wire_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        Wire *gate = new Wire();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        this->rotateComponent(gate,points[2].toInt());
        gate->setBoundingRect(points[3].toInt());
        gate->disconnect();
        gate->connect(gate->connectionPoints());
    });
    QJsonArray voltage_components= data["voltage"].toArray();
    std::for_each(voltage_components.begin(),voltage_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        DCVoltage *gate = new DCVoltage();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        gate->setVoltage(points[3].toInt());
        this->rotateComponent(gate,points[2].toInt());
    });
    QJsonArray resistor_components= data["resistor"].toArray();
    std::for_each(resistor_components.begin(),resistor_components.end(),[this](QJsonValueRef value){
        QJsonArray points = value.toArray();
        Resistor *gate = new Resistor();
        gate->setPos(QPointF(points[0].toInt(),points[1].toInt()));
        gate->connect(gate->connectionPoints());
        this->scene->addItem(gate);
        gate->setResistance(points[3].toInt());
        this->rotateComponent(gate,points[2].toInt());
    });

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
    QJsonArray flipflop_components;
    QJsonArray lcd_components;
    QJsonArray decoder_components;
    QList<QGraphicsItem*> allItems = scene->items();
    for (int i = 0; i < allItems.size(); ++i) {
        QGraphicsItem *component = allItems[i];
        if(component->parentItem() == nullptr){
            // Save coordinates
            qreal x = component->x();
            qreal y = component->y();
            QJsonArray points;
            //Add to array in every case
            //Position
            points.push_back(x);
            points.push_back(y);
            Component *rItem = qgraphicsitem_cast<Component*> (component);
            //Angle of rotation
            points.push_back(rItem->rotationAngle());
            // Find type of component
            std::string type = rItem->componentType();
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
                // Additional information about voltage
                points.push_back(rItem->voltage());
                dcvoltage_components.push_back(points);
            }
            if(type == "resistor"){
                //Additional information about resistor
                Resistor *resistant = static_cast<Resistor*>(rItem);
                points.push_back(resistant->resistance());
                resistor_components.push_back(points);
            }
            if(type == "clock"){
                //Additional infomration about clock
                Clock* clock = static_cast<Clock*>(rItem);
                points.push_back(clock->timeInterval());
                clock_components.push_back(points);
            }
            if (type == "ground"){
                ground_components.push_back(points);
            }
            if (type == "wire"){
                Wire *wire = static_cast<Wire*>(rItem);
                //Information about wire width
                points.push_back(wire->boundingRect().width());
                wire_components.push_back(points);
            }
            if (type == "switch"){
                //Additional information about switcher
                Switch *switcher = static_cast<Switch*>(rItem);
                points.push_back(switcher->isOpened());
                switch_components.push_back(points);
            }
            if (type == "flipflop"){
                flipflop_components.push_back(points);
            }
            if (type == "lcd"){
                lcd_components.push_back(points);
            }
            if (type == "decoder"){
                decoder_components.push_back(points);
            }

        }
    }
    QJsonObject topQuery;
    //Save in Json format (map)
    // { type : [ information ]}
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
    topQuery["switch"] = switch_components;
    topQuery["decoder"] = decoder_components;
    topQuery["lcd"] = lcd_components;
    topQuery["flipflop"] = flipflop_components;
    QJsonDocument document(topQuery);

    //Set the name of the new file
    //Increase counter
    std::string fileName ="sema" + std::to_string(counterOfFiles++)+".json";
    currentFile = QString(fileName.c_str());
    saveFile(document);

}


void MainWindow::saveFile(QJsonDocument document) {

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
