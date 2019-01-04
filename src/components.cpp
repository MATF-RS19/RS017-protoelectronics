#include "components.hpp"
#include <iostream>
#include <stdexcept>

#ifdef QTPAINT
#include "scene.h" // for itemChange
#include "mainwindow.h" // for propertiesMessage
#include "dialog.h"
#endif

template<typename T>
int Counter<T>::_counter(0);
int Wire::_nodeVoltageChanged(-1);

std::set<std::shared_ptr<Node>, Node::lex_node_cmp> Node::_allNodes;

std::string Component::toString() const {
	std::string str;
	str = name() + "\n";

	str += "U = " + std::to_string(voltage()) + " V\n";
	str += "I = " + std::to_string(current()) + " A\n";
	str += "P = " + std::to_string(power()) + " W\n";
	return str;
}

std::ostream& operator<<(std::ostream& out, const Component& c) {
    out << "Nodes: [ ";
    for (const auto& n : c.nodes()) {
        out << n->id() << " ";
    }

    out << "]\n";
	out << c.toString();
    return out;
}

std::ostream& operator<<(std::ostream& out, const Node& n) {
	out << "*  Node " << n.id() << ":" << std::endl
		<< "(" << n.x() << ", " << n.y() << ")" << std::endl
		<< "V = " << n._v << " V" << std::endl
		<< "Connected components: " << std::endl;

	for (const auto& c : n.components()) {
		out << c->name() << "\t";
	}

	return out;
}


//Node
Node::Node(int x, int y, Component* const component = nullptr)
:_x(x), _y(y)
{
    _components.clear();
    _components.reserve(2);
	if (component != nullptr) _components.push_back(component);
}

int Node::x() const { return _x; }
int Node::y() const { return _y; }

void Node::addComponent(Component* const e){
    //Node don't need multiple connection to the same component
    if (e != nullptr && !isConnectedTo(e))
        _components.push_back(e);
}

std::vector<Component*> Node::directComponents() const{
	return _components;
}

template<typename T>
void append(T& a, T& b) {
    a.reserve(a.size() + b.size());
    std::move(b.begin(), b.end(), std::inserter(a, a.end()));
    b.clear();
}

std::vector<Component*> Node::components() const{
	std::vector<Component*> allComponents;
    allComponents.reserve(_components.size());

    for (const auto& c : _components) {
        if ( c->componentType() != "wire" ) {
            allComponents.push_back(c);
        } else {
            //If it's wire, we need to recursively take components from another side of wire
            auto otherNode = ((Wire*)c)->otherNode(this->id());
            //To prevent infinite recursion,
            //disconnect node from component through which we came to that node
            //but after recursion establish removed connection
            otherNode->disconnectFromComponent(c);
            auto otherComponnets = otherNode->components();
            append(allComponents, otherComponnets);
            otherNode->addComponent(c);
        }
    }

    return allComponents;
}

std::vector<Component*> Node::directComponents(const std::string& componentType) const {
	std::vector<Component*> filterd;

	for (auto const& component : _components) {
		if (component->componentType() == componentType) {
			filterd.push_back(component);
		}
	}

	return filterd;
}

std::vector<Component*> Node::components(const std::string& componentType) const {
	std::vector<Component*> filterd;

	for (auto const& component : components()) {
		if (component->componentType() == componentType) {
			filterd.push_back(component);
		}
	}

	return filterd;
}

std::vector<Component*>::iterator Node::find(Component* const e) {
    if (e == nullptr) return _components.end();

    return find_if(_components.begin(), _components.end(),
            [e](Component* const curr){return e == curr;});
}

std::vector<Component*> Node::findDirectlyConnected(const std::string& componentType, int x, int y) {
	auto i = Node::find(x, y);
	if (i != _allNodes.end()) {
		return (*i)->directComponents(componentType);
	} else {
		return std::vector<Component*>();
	}
}

std::vector<Component*> Node::find(const std::string& componentType, int x, int y) {
	auto i = Node::find(x, y);
	if (i != _allNodes.end()) {
		return (*i)->components(componentType);
	} else {
		return std::vector<Component*>();
	}
}

bool Node::isConnectedTo(Component* const e) {
    return find(e) != _components.end() ? true : false;
}

std::set<std::shared_ptr<Node>, Node::lex_node_cmp>::iterator Node::find(int x, int y) {
    return _allNodes.find(std::make_shared<Node>(x, y));
}

void Node::disconnectFromComponent(Component* const e) {
    if (e == nullptr) return;

    auto it = find(e);
    if (it != _components.end()){
        _components.erase(it);
    }
}

//Component
Component::Component(const std::string &name)
	:_name(name)
{
    _nodes.clear();
    _nodes.reserve(3);

#ifdef QTPAINT
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable |
            QGraphicsItem::ItemIsMovable |
            QGraphicsItem::ItemSendsGeometryChanges);

    penForLines = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap);
    penForLinesWhite = QPen(Qt::white, 3, Qt::SolidLine, Qt::RoundCap);
    penForDots = QPen(Qt::white, 6, Qt::SolidLine, Qt::RoundCap);
    penForLeadsGreen = QPen(Qt::green, 3, Qt::SolidLine, Qt::RoundCap);
    penForLeadsRed = QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap);
#endif
}

#ifdef QTPAINT
QVariant Component::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();

        if(QApplication::mouseButtons() == Qt::LeftButton &&
                qobject_cast<GridZone*> (scene())){

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

void Component::mousePressEvent(QGraphicsSceneMouseEvent* event) {	
	if(event->button() == Qt::RightButton) {
        QPointF center = boundingRect().center();
        QTransform rotation = QTransform().translate(center.x(), center.y()).rotate(90).translate(-center.x(), -center.y());
        setTransform(rotation, true);

        disconnect();
        connect(connectionPoints());
    }
    QGraphicsItem::mousePressEvent(event);
}

void Component::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        disconnect();
        connect(connectionPoints());
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void Component::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    auto dots = this->connectionPoints();
//    qDebug() << QString::fromStdString(this->componentType());
//    for(auto e: dots) {
//        qDebug() << e.first << " " << e.second;
//    }
    //qDebug() << QString::fromStdString(this->toString());
    QGraphicsItem::mouseMoveEvent(event);
}

void Component::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    setSelected(true);
    penForLines.setColor(QColor(8, 246, 242));
    penForLinesWhite.setColor(QColor(8, 246, 242));

	// DONT MAKE GLOBAL, it will crash!
	MainWindow * mw = MainWindow::getMainWindow();
	mw->propertiesMessage->setText(QString::fromStdString(this->toString()));
	update();

	QGraphicsItem::hoverEnterEvent(event);
}

void Component::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    setSelected(false);
    penForLines.setColor(QColor(Qt::black));
    penForLinesWhite.setColor(Qt::white);

	// DONT MAKE GLOBAL, it will crash!
	MainWindow * mw = MainWindow::getMainWindow();
	mw->propertiesMessage->setText("");
	update();

	QGraphicsItem::hoverLeaveEvent(event);
}

QRectF Component::boundingRect() const {
    return QRectF(0,0,100,100);
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawRect(boundingRect());
}
#endif

Component::~Component() {
    for (auto it = _nodes.begin(); it != _nodes.end(); ++it) {
        //disconnect node->component
        (*it)->disconnectFromComponent(this);

        //remove from all nodes
        if ((*it).use_count() == 2) {
            Node::_allNodes.erase(*it);
        }

        //"disconnect" component->node
        *it = nullptr;
    }
}

std::string Component::name() const {
	return _name;
}

std::vector<std::shared_ptr<Node>> Component::nodes() const {
	return _nodes;
}

std::vector<std::shared_ptr<Node>>::iterator Component::find(int x, int y)  {
    return std::find_if(_nodes.begin(), _nodes.end(),
            [x, y](const std::shared_ptr<Node>& node_ptr){
                    return node_ptr->x() == x && node_ptr->y() == y; });
}

bool Component::isConnectedTo(int x, int y) {
    auto const it = find(x, y);
    if (it == _nodes.end()) return false;
    return true;
}

void Component::addNode(int x, int y) {

    auto new_node = std::make_shared<Node>(x, y, this);
	_nodes.push_back(new_node);

	auto res = Node::_allNodes.insert(new_node);
    //node already exists
	if (res.second == false) {
		_nodes.back() = *res.first;
		_nodes.back()->addComponent(this);
	}
}

void Component::connect(const std::vector<std::pair<int, int>> &connPts) {
    for(auto const& connPoint : connPts) {
        this->addNode(connPoint.first, connPoint.second);
    }
}

void Component::disconnect(int x, int y) {
    if (Node::find(x, y) == Node::_allNodes.end()) return;

    for (auto it = _nodes.begin(); it != _nodes.end(); ) {
        if ((*it)->x() == x && (*it)->y() == y) {
            //disconnect from _allNodes
            if ((*it).use_count() == 2)
                Node::_allNodes.erase(*it);

            //node doesn't point to component anymore
            (*it)->disconnectFromComponent(this);

            //and component doesn't point to node
            it = _nodes.erase(it);
        } else {
            ++it;
        }
    }
}

void Component::disconnect() {
    for (auto it = _nodes.begin(); it != _nodes.end(); ) {
        //disconnect from _allNodes
        if ((*it).use_count() == 2)
            Node::_allNodes.erase(*it);

        //node doesn't point to component anymore
        (*it)->disconnectFromComponent(this);

        //and component doesn't point to node
        it = _nodes.erase(it);
    }
}

template <typename Iter>
void Component::disconnectAndPreserveEmptyPlace(Iter &it) {
        assert(it != _nodes.end());

        //disconnect from _allNodes
        if ((*it).use_count() == 2)
            Node::_allNodes.erase(*it);

        //node doesn't point to component anymore
        (*it)->disconnectFromComponent(this);

        //and component doesn't point to node
        *it = nullptr;
}

template <typename Iter>
void Component::addNodeAt(Iter &pos, int x, int y) {
    assert(pos != _nodes.end());
    assert(*pos == nullptr);

    *pos = std::make_shared<Node>(x, y, this);

    auto res = Node::_allNodes.insert((*pos));
    //Node already exists
    if (res.second == false) {
        (*pos) = *res.first;
        (*pos)->addComponent(this);
	}
}

void Component::reconnect(int xFrom, int yFrom, int xTo, int yTo) {
    //If start and end node is the same node we don't need to reconnect
    if(xFrom == xTo && yFrom == yTo) return;

    //If node from doesn't exist, just connect to destination node
    auto nodeFrom = Node::find(xFrom, yFrom);
    if (nodeFrom == Node::_allNodes.end()) {
        addNode(xTo, yTo);
        return;
    }

    //If exists, disconnect component from that node
    //and connect to destination node, but on the same position as original
    for (auto it = _nodes.begin(); it != _nodes.end(); it++) {
        if ((*it)->x() == xFrom && (*it)->y() == yFrom ) {
            disconnectAndPreserveEmptyPlace(it);
            addNodeAt(it, xTo, yTo);
        }
	}
}

double Component::power() const {
	return voltage() * current();
}

void Component::updateVoltages(const std::shared_ptr<Node>& node) const {
    for (const auto& component : node->components()) {
        component->voltage();
        component->update();
    }
}


//Ground
Ground::Ground()
	:Component("GND" + std::to_string(_counter+1))
{}

#ifdef QTPAINT
void Ground::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //Component::paint(painter, option, widget);

    // Setting color for drawing lines
    painter->setPen(penForLines);

    // Vertical line
    painter->drawLine(50, 60, 50, 0);

    // Horizontal lines, one beneath other
    painter->drawLine(20, 60, 80, 60);
    painter->drawLine(30, 70, 70, 70);
    painter->drawLine(40, 80, 60, 80);

    // Connection points
    painter->setPen(penForDots);
	QPointF up(50, 1);
    painter->drawPoint(up);
}

std::vector<std::pair<int, int>> Ground::connectionPoints(void) const {
    std::vector<std::pair<int, int>> dots;

    // Find local coordinates of connection point
    QPointF localPoint(boundingRect().x()+boundingRect().width()/2,
                       boundingRect().y());

    // And then map to scene coordinates
    auto scenePoint = mapToScene(localPoint);
    dots.push_back(std::pair<int, int>(scenePoint.x(), scenePoint.y()));
    return dots;
}

#endif

double Ground::voltage() const {
	return 0;
}

//TODO
double Ground::current() const {
	return 0;
}

void Ground::addNode(int x, int y) {
    if (_nodes.size() >= 1) {
        throw std::runtime_error("Ground already connected!");
    }

    Component::addNode(x, y);
    _nodes.back()->_v = 0;
}



//Wire
Wire::Wire()
	:Component("W" + std::to_string(_counter+1))
{
#ifdef QTPAINT
	// Default values for wire
	changingBoundingRec = QRectF(0,0,100,100);
	startWire = QPointF(0,50);
	endWire = QPointF(100,50);
#endif
}

std::string Wire::toString() const {
	std::string str;
	str = name() + "\n";

	str += "U = " + std::to_string(voltage()) + " V\n";
	return str;
}


#ifdef QTPAINT
QRectF Wire::boundingRect() const {
	return changingBoundingRec;
}

void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
	// Component::paint(painter, option, widget);

	// Setting color for drawing lines
	if(_nodes[0]->_v > 0 || _nodes[1]->_v > 0)
		painter->setPen(penForLeadsGreen);
	else if(_nodes[0]->_v < 0 || _nodes[1]->_v < 0)
		painter->setPen(penForLeadsRed);
	else
		painter->setPen(penForLines);

	line.setPoints(startWire, endWire);
	painter->drawLine(line);

	// Connection points
	painter->setPen(penForDots);
	QPointF p1(0.8, 50);
	QPointF p2(changingBoundingRec.width()-0.8, 50);
	painter->drawPoint(p1);
	painter->drawPoint(p2);
}

std::vector<std::pair<int, int>> Wire::connectionPoints(void) const {
    std::vector<std::pair<int, int>> dots;
    dots.reserve(2);

    // Find local coordinates of connection point
    QPointF localPoint1(boundingRect().x(),
                       boundingRect().y()+boundingRect().height()/2);

    QPointF localPoint2(boundingRect().x()+boundingRect().width(),
                       boundingRect().y()+boundingRect().height()/2);

    // And then map to scene coordinates
    auto scenePoint1 = mapToScene(localPoint1);
    auto scenePoint2 = mapToScene(localPoint2);
    dots.push_back(std::pair<int, int>(scenePoint1.x(), scenePoint1.y()));
    dots.push_back(std::pair<int, int>(scenePoint2.x(), scenePoint2.y()));

    return dots;
}

void Wire::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
	if(event->button() == Qt::LeftButton) {
		double width = changingBoundingRec.width();
        double scaleNumber = 10;
        changingBoundingRec.setWidth(scaleNumber + width);
        endWire.setX(scaleNumber + width);
		update();
	}
	QGraphicsItem::mouseDoubleClickEvent(event);
}

#endif

std::shared_ptr<Node> Wire::otherNode(int id) const {
    return _nodes[0]->id() == id ? _nodes[1] : _nodes[0];
}


//TODO
double Wire::voltage() const {
    if (_nodes.size() != 2) return 0;

    //voltage connected to wire
    if (_nodes[1]->directComponents("voltage").size() != 0) {
        _nodeVoltageChanged = 0;
        _nodes[0]->_v = _nodes[1]->_v;
    }
    else if(_nodes[0]->directComponents("voltage").size() != 0) {
        _nodeVoltageChanged = 1;
        _nodes[1]->_v = _nodes[0]->_v;
    }

    //other voltage sources, from logic gates...
    if (_nodeVoltageChanged != 0 && _nodes[0]->_v != 0.0) {
        _nodes[1]->_v = _nodes[0]->_v;
        _nodeVoltageChanged = 1;
    }
    else if (_nodeVoltageChanged != 1 && _nodes[1]->_v != 0.0) {
        _nodes[0]->_v = _nodes[1]->_v;
        _nodeVoltageChanged = 0;
    }
    else {
        _nodes[0]->_v = 0;
        _nodes[1]->_v = 0;
        return 0;
    }


    return _nodes[0]->_v;
}

//TODO
double Wire::current() const {
	return 0;
}

void Wire::addNode(int x, int y) {
    if (_nodes.size() >= 2) {
        throw std::runtime_error("Wire already connected!");
    }

    Component::addNode(x, y);

    voltage();
}

void Wire::disconnect(int x, int y) {
    auto it = Node::find(x, y);
    if (it == Node::_allNodes.end()) return;

    if ((*it) == _nodes[0] && _nodeVoltageChanged == 0) {
        (*it)->_v = 0;
    }
    if ((*it) == _nodes[1] && _nodeVoltageChanged == 1) {
        (*it)->_v = 0;
    }

    Component::disconnect(x, y);
}

void Wire::disconnect() {
    for (int i = 0; i < _nodes.size() ; ++i) {
        if (_nodeVoltageChanged == i) {
            _nodes[i]->_v = 0;
        }
    }

    Component::disconnect();
}

void Wire::reconnect(int xFrom, int yFrom, int xTo, int yTo) {
    auto start = Node::find(xFrom, yFrom);
    if (start != Node::_allNodes.end()) {
        voltage();
    }
    Component::reconnect(xFrom, yFrom, xTo, yTo);
}



//Resistor
Resistor::Resistor(double resistance)
	:Component("R" + std::to_string(_counter+1)),
	_resistance(resistance)
{
    if (_resistance <= 0) {
        throw std::invalid_argument("Resistance must be positive");
    }
}

#ifdef QTPAINT
void Resistor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //Component::paint(painter, option, widget);

    // Setting color for drawing lines
    painter->setPen(penForLines);

    // Input line
    painter->drawLine(0, 50, 18, 50);

    // Zigzag lines
    painter->drawLine(18, 50, 25, 30);
    painter->drawLine(25, 30, 35, 65);
    painter->drawLine(35, 65, 45, 30);
    painter->drawLine(45, 30, 55, 65);
    painter->drawLine(55, 65, 65, 30);
    painter->drawLine(65, 30, 75, 65);
    painter->drawLine(75, 65, 82, 50);

    // Output line
    painter->drawLine(82, 50, 100, 50);

    painter->setFont(QFont("Times", 12, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignHCenter, QString::number(_resistance) + " Ohm");

    // Connection points
    painter->setPen(penForDots);
	QPointF p1(1, 50);
	QPointF p2(99, 50);
    painter->drawPoint(p1);
    painter->drawPoint(p2);
}

std::vector<std::pair<int, int>> Resistor::connectionPoints(void) const {
    std::vector<std::pair<int, int>> dots;
    dots.reserve(2);

    // Find local coordinates of connection point
    QPointF localPoint1(boundingRect().x(),
                       boundingRect().y()+boundingRect().height()/2);

    QPointF localPoint2(boundingRect().x()+boundingRect().width(),
                       boundingRect().y()+boundingRect().height()/2);

    // And then map to scene coordinates
    auto scenePoint1 = mapToScene(localPoint1);
    auto scenePoint2 = mapToScene(localPoint2);
    dots.push_back(std::pair<int, int>(scenePoint1.x(), scenePoint1.y()));
    dots.push_back(std::pair<int, int>(scenePoint2.x(), scenePoint2.y()));
    return dots;
}

void Resistor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
	if(event->button() == Qt::LeftButton) {
		Dialog* dialog = new Dialog(this);
		dialog->setModal(false);
        dialog->show();

		update();
	}
	QGraphicsItem::mouseDoubleClickEvent(event);
}
#endif

double Resistor::resistance() const {
	return _resistance;
}

void Resistor::setResistance(double resistance) {
	_resistance = resistance;
}

double Resistor::voltage() const {
    if (_nodes.size() != 2) return 0;
	return _nodes[1]->_v - _nodes[0]->_v;
}

double Resistor::current() const {
	return voltage() / _resistance;
}

void Resistor::addNode(int x, int y) {
    if (_nodes.size() >= 2) {
        throw std::runtime_error("Resistor already connected!");
    }
    Component::addNode(x, y);
}




//DCVoltage
DCVoltage::DCVoltage(double voltage)
	:Component("U" + std::to_string(_counter+1)),
	_voltage(voltage)
{}

#ifdef QTPAINT
void DCVoltage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    // Component::paint(painter, option, widget);

    // Setting color for drawing lines
    painter->setPen(penForLines);

    // Vertical line
    if(voltage() > 0)
        painter->setPen(penForLeadsGreen);
    else if(voltage() < 0)
        painter->setPen(penForLeadsRed);
    else
        painter->setPen(penForLines);
    painter->drawLine(50, 0, 50, 40);

    // Connection points
    painter->setPen(penForDots);
	QPointF p(50, 1);
    painter->drawPoint(p);

    painter->setPen(QPen(Qt::black));
    painter->setFont(QFont("Times", 12, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(_voltage) + " V");
}

std::vector<std::pair<int, int>> DCVoltage::connectionPoints(void) const {
    std::vector<std::pair<int, int>> dots;

    // Find local coordinates of connection point
    QPointF localPoint(boundingRect().x()+boundingRect().width()/2,
                       boundingRect().y());

    // And then map to scene coordinates
    auto scenePoint = mapToScene(localPoint);
    dots.push_back(std::pair<int, int>(scenePoint.x(), scenePoint.y()));
    return dots;
}

void DCVoltage::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
	if(event->button() == Qt::LeftButton) {
		Dialog* dialog = new Dialog(this);
		dialog->setModal(false);
		dialog->show();

		update();
	}
	QGraphicsItem::mouseDoubleClickEvent(event);
}
#endif

void DCVoltage::addNode(int x, int y) {
    if (_nodes.size() >= 1) {
        throw std::runtime_error("DCVoltage already connected!");
    }
    Component::addNode(x, y);
    _nodes.back()->_v = _voltage;
}

double DCVoltage::voltage() const {
	return _voltage;
}

void DCVoltage::setVoltage(double voltage) {
    _voltage = voltage;
    //if connected to node, set voltage in node
    if (_nodes.size() != 0) {
        _nodes.back()->_v = voltage;
        updateVoltages(_nodes.back());
    }
}

//TODO
double DCVoltage::current() const {
	return 0;
}

/* TODO 2 terminals
#ifdef QTPAINT
void VoltageSource::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    // Component::draw(painter, option, widget);

    // Setting color for drawing lines
    painter->setPen(penForLines);

    QLineF lineUpVertical(50,0, 50, 45);
    QLineF lineUpHorizontal(25, 45, 75, 45);
    QLineF lineDownHorizontal(35, 55, 65, 55);
    QLineF lineDownVertical(50, 55, 50, 100);
    painter->drawLine(lineUpVertical);
    painter->drawLine(lineUpHorizontal);
    painter->drawLine(lineDownHorizontal);
    painter->drawLine(lineDownVertical);

    // Connection points
    painter->setPen(penForDots);
    QPointF pUp(50,0.5);
    QPointF pDown(50,99.5);
    painter->drawPoint(pUp);
    painter->drawPoint(pDown);
}


std::vector<std::pair<int, int>> VoltageSource::connectionPoints(void) const {
    // TODO AKO DODAMO
    std::vector<std::pair<int, int>> dots(2);
    dots.push_back(std::pair<int, int>(this->x(), this->y()+boundingRect().height()/2));
    dots.push_back(std::pair<int, int>(this->x()+boundingRect().width(), this->y()+boundingRect().height()/2));
    return dots;
}
#endif
*/


void DCVoltage::disconnect(int x, int y) {
    auto it = Node::find(x, y);
    if (it == Node::_allNodes.end()) return;

    (*it)->_v = 0;
    updateVoltages((*it));
    Component::disconnect(x, y);
}

void DCVoltage::disconnect() {
    for (const auto& node : _nodes) {
        node->_v = 0;
        updateVoltages(node);
    }
    Component::disconnect();
}

void DCVoltage::reconnect(int xFrom, int yFrom, int xTo, int yTo) {
    auto start = Node::find(xFrom, yFrom);
    if (start != Node::_allNodes.end()) (*start)->_v = 0;
    updateVoltages(*start);
    Component::reconnect(xFrom, yFrom, xTo, yTo);
}

//Switch
Switch::Switch(state s)
:Component("S" + std::to_string(_counter+1)),
_state(s)
{

}

void Switch::addNode(int x, int y) {
    if (_nodes.size() >= 2) {
        throw std::runtime_error("Switch already connected!");
    }
    Component::addNode(x, y);
}

void Switch::open() {
    _state = OPEN;

    //Switch should be connected
    if (_nodes.size() != 2) return;

    if (_nodes[0]->components("voltage").size() != 0) {
        _nodes[1]->_v = 0;
    }
    else if (_nodes[1]->components("voltage").size() != 0) {
        _nodes[0]->_v = 0;
    } else {
        _nodes[0]->_v = 0;
        _nodes[1]->_v = 0;
    }
}

bool Switch::isOpened() const {
    return _state == OPEN;
}

void Switch::close() {
    _state = CLOSE;

    //Switch should be connected
    if (_nodes.size() != 2) return;

    //There is voltage source (exactly one) on one side
    if (_nodes[0]->_v != 0.0) {
        assert(_nodes[1]->_v == 0.0);
        //Delegate that voltage to another side
        _nodes[1]->_v = _nodes[0]->_v;
    } else {
        assert(_nodes[0]->_v == 0.0);
        _nodes[0]->_v = _nodes[1]->_v;
    }
}

bool Switch::isClosed() const {
    return _state == CLOSE;
}

void Switch::changeState() {
    if (_state == OPEN) close();
    else open();
}

double Switch::voltage() const {
    if (_nodes.size() != 2) return 0;
    return _nodes[0]->_v;
}

//TODO
double Switch::current() const {
    return 0;
}

#ifdef QTPAINT
void Switch::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    // Component::paint(painter, option, widget);

    // Setting color for drawing lines
    painter->setPen(penForLines);

    // Open/Closed line
    painter->setPen(penForLinesWhite);
    if(this->_state == OPEN)
        painter->drawLine(35, 50, 65, 30);
    else
        painter->drawLine(35, 47, 65, 47);

	// Input line
	if(_nodes[0]->_v > 0)
		painter->setPen(penForLeadsGreen);
	else if(_nodes[0]->_v < 0)
		painter->setPen(penForLeadsRed);
	else
		painter->setPen(penForLines);

	painter->drawLine(0, 50, 35, 50);

	// Output line
	if(_nodes[1]->_v > 0)
		painter->setPen(penForLeadsGreen);
	else if(_nodes[1]->_v < 0)
		painter->setPen(penForLeadsRed);
	else
		painter->setPen(penForLines);

	painter->drawLine(65, 50, 100, 50);

    // Connection points
    painter->setPen(penForDots);
	QPointF p1(1, 50);
	QPointF p2(99, 50);
    painter->drawPoint(p1);
    painter->drawPoint(p2);
}

std::vector<std::pair<int, int>> Switch::connectionPoints(void) const{
    std::vector<std::pair<int, int>> dots;
    dots.reserve(2);

    // Find local coordinates of connection point
    QPointF localPoint1(boundingRect().x(),
                       boundingRect().y()+boundingRect().height()/2);

    QPointF localPoint2(boundingRect().x()+boundingRect().width(),
                       boundingRect().y()+boundingRect().height()/2);

    // And then map to scene coordinates
    auto scenePoint1 = mapToScene(localPoint1);
    auto scenePoint2 = mapToScene(localPoint2);
    dots.push_back(std::pair<int, int>(scenePoint1.x(), scenePoint1.y()));
    dots.push_back(std::pair<int, int>(scenePoint2.x(), scenePoint2.y()));
    return dots;
}

void Switch::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    Component::mousePressEvent(event);
    if(event->button() == Qt::LeftButton) {
        this->changeState();
        update();
    }

    QGraphicsItem::mousePressEvent(event);
}
#endif

