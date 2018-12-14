#include "components.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>

template<typename T>
int Counter<T>::_counter(0);

std::set<std::shared_ptr<Node>, Node::node_cmp> Node::_allNodes;


std::ostream& operator<<(std::ostream& out, const Component& c) {
	out << c.name() << std::endl
	<< "Nodes: [ ";
	for (auto n : c.nodes()) {
		out << n->id() << " ";
	}
	out << "]" << std::endl;

	out << "U = " << c.voltage() << " V" << std::endl
		<< "I = " << c.current() << " A" << std::endl
		<< "P = " << c.power() << " W";

	return out;
}

std::ostream& operator<<(std::ostream& out, const Node& n) {
	out << "*  Node " << n.id() << ":" << std::endl
		<< "(" << n.x() << ", " << n.y() << ")" << std::endl
		<< "V = " << n._v << " V" << std::endl
		<< "Connected components: " << std::endl;

	for (auto c : n.components()) {
		out << c->name() << "\t";
	}

	return out;
}


//Node
Node::Node(int x, int y, Component* const component = nullptr)
:_x(x), _y(y)
{
    _components.clear();
	if (component) _components.push_back(component);
}

//Node coordinates
int Node::x() const { return _x; }
int Node::y() const { return _y; }

void Node::addComponent(Component* const e){
	_components.push_back(e);
}

void Node::pop_component() {
    _components.pop_back();
}

//All components connected to node
std::vector<Component*> Node::components() const{
	return _components;
}


//Only components of type 'componentType' connected to node
std::vector<Component*> Node::components(char componentType) const {
	std::vector<Component*> filterd;

	for (auto const& component : _components) {
		if (component->name()[0] == componentType) {
			filterd.push_back(component);
		}
	}

	return filterd;
}


std::vector<Component*> Node::components(char componentType, int x, int y) {
    //BUG wont find (1,4)
	auto i = _allNodes.find(std::make_shared<Node>(x, y));
	if (i != _allNodes.end()) {
		return (*i)->components(componentType);
	} else {
		return std::vector<Component*>();
	}
}


//Disconnect component e from this node
void Node::disconnectComponent(Component* const e) {
    for (auto it = _components.begin(); it != _components.end(); ) {
        if (*it == e) {
            it = _components.erase(it);
        } else {
            ++it;
        }
    }
}



//Component
Component::Component(const std::string &name)
	:_name(name)
{
    _nodes.clear();
/*
    setFlags(QGraphicsItem::ItemIsSelectable |
            QGraphicsItem::ItemIsMovable |
            QGraphicsItem::ItemSendsGeometryChanges);

    penForLines = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap);
    penForDots = QPen(Qt::white, 6, Qt::SolidLine, Qt::RoundCap);
*/
}
/*
QVariant Component::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange && scene()) {
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
*/
/*
QRectF Component::boundingRect() const {
    return QRectF(0,0,100,100);
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //painter->drawRect(boundingRect());
}
*/
Component::~Component() {
    for (auto const& node : _nodes) {
        node->pop_component();
        if (node.use_count() == 2)
            Node::_allNodes.erase(node);
    }
}

std::string Component::name() const {
	return _name;
}

std::vector<std::shared_ptr<Node>> Component::nodes() const {
	return _nodes;
}

/*
Connect component to node if that node exist,
if not, make new node and connect
*/
void Component::addNode(int x, int y) {

    auto new_node_ptr = std::make_shared<Node>(x, y, this);
	_nodes.push_back(new_node_ptr);

	auto res = Node::_allNodes.insert(_nodes.back());
	if (!res.second) {
		_nodes.back() = *res.first;
		_nodes.back()->addComponent(this);
	}
}

void Component::addNodes(int x1, int y1, int x2, int y2) {
    addNode(x1, y1);
    addNode(x2, y2);
}

void Component::addNodes(int x1, int y1, int x2, int y2, int x3, int y3) {
    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
}

void Component::addNodeAt(unsigned pos, int x, int y) {
	assert(pos < _nodes.size());

    auto new_node_ptr = std::make_shared<Node>(x, y, this);
	_nodes[pos]->disconnectComponent(this);
	_nodes[pos] = new_node_ptr;

	auto res = Node::_allNodes.insert(_nodes[pos]);
	if (!res.second) {
		_nodes[pos] = *res.first;
		_nodes[pos]->addComponent(this);
	}
}

/*
Reconnect component from nodeFrom (x,y) to nodeTo (x,y)
for all leads connected to nodeFrom
*/
void Component::reconnect(int xFrom, int yFrom, int xTo, int yTo) {

	for (unsigned i = 0; i < _nodes.size(); ++i) {
		if (_nodes[i]->x() == xFrom && _nodes[i]->y() == yFrom ) {
			addNodeAt(i, xTo, yTo);
		}
	}
}

double Component::power() const {
	return voltage() * current();
}



//Ground
Ground::Ground()
	:Component("GND" + std::to_string(_counter+1))
{}
/*
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
*/
double Ground::voltage() const {
	return 0;
}

//TODO
double Ground::current() const {
	return 0;
}

void Ground::addNode(int x, int y) {
    Component::addNode(x, y);
    _nodes.back()->_v = 0;
}



//Wire
Wire::Wire()
	:Component("W" + std::to_string(_counter+1))
{
}
/*
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
*/
double Wire::voltage() const {
	return 0;
}

//TODO
double Wire::current() const {
	return 0;
}

std::shared_ptr<Node> Wire::otherNode(int id) {
	return _nodes[0]->id() == id ? _nodes[1] : _nodes[0];
}



//Resistor
Resistor::Resistor(double resistance)
	:Component("R" + std::to_string(_counter+1)),
	_resistance(resistance)
{
}
/*
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
    painter->drawText(boundingRect(), Qt::AlignHCenter, QString::number(_resistance) + " Ohm");

    painter->setPen(penForDots);
    QPointF p1(0, 50);
    QPointF p2(100, 50);
    painter->drawPoint(p1);
    painter->drawPoint(p2);
}
*/
double Resistor::resistance() const {
	return _resistance;
}

double Resistor::voltage() const {
    if (_nodes.size() != 2) return 0;
	return _nodes[1]->_v - _nodes[0]->_v;
}

double Resistor::current() const {
	return voltage() / _resistance;
}



//DCVoltage
DCVoltage::DCVoltage(double voltage)
	:Component("U" + std::to_string(_counter+1)),
	_voltage(voltage)
{}
/*
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
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(_voltage) + " V");
}
*/
/* TODO 2 terminals
void VoltageSource::draw(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Component::draw(painter, option, widget);

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
*/

void DCVoltage::addNode(int x, int y) {
    Component::addNode(x, y);
    _nodes.back()->_v = _voltage;
}

double DCVoltage::voltage() const {
	return _voltage;
}

//TODO
double DCVoltage::current() const {
	return 0;
}


