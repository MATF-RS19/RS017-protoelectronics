#include "components.hpp"
#include <iostream>

template<typename T>
int Counter<T>::_counter(0);

std::set<std::shared_ptr<Node>, Node::lex_node_cmp> Node::_allNodes;


std::ostream& operator<<(std::ostream& out, const Component& c) {
	out << c.name() << std::endl
	<< "Nodes: [ ";
	for (const auto& n : c.nodes()) {
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

std::vector<Component*> Node::components() const{
	return _components;
}

std::vector<Component*> Node::components(char componentType) const {
	std::vector<Component*> filterd;

	for (auto const& component : _components) {
		if (component->name()[0] == componentType) {
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

std::vector<Component*> Node::find(char componentType, int x, int y) {
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
    setFlags(QGraphicsItem::ItemIsSelectable |
            QGraphicsItem::ItemIsMovable |
            QGraphicsItem::ItemSendsGeometryChanges);

    penForLines = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap);
    penForDots = QPen(Qt::white, 6, Qt::SolidLine, Qt::RoundCap);
#endif
}

#ifdef QTPAINT
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

QRectF Component::boundingRect() const {
    return QRectF(0,0,100,100);
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //painter->drawRect(boundingRect());
}
#endif

Component::~Component() {
    for (auto const& node : _nodes) {
        node->components().pop_back();// pop_component();
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

void Component::disconnect(int x, int y) {
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



//Ground
Ground::Ground()
	:Component("GND" + std::to_string(_counter+1))
{}

#ifdef QTPAINT
void Ground::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Component::paint(painter, option, widget);
    painter->setPen(penForLines);

    // vertical line
    painter->drawLine(50, 60, 50, 0);
    // horizontal lines, one beneath other
    painter->drawLine(20, 60, 80, 60);
    painter->drawLine(30, 70, 70, 70);
    painter->drawLine(40, 80, 60, 80);

    painter->setPen(penForDots);
    QPointF up(50, 0.5);
    painter->drawPoint(up);
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
    Component::addNode(x, y);
    _nodes.back()->_v = 0;
}



//Wire
Wire::Wire()
	:Component("W" + std::to_string(_counter+1))
{
}

#ifdef QTPAINT
void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Component::paint(painter, option, widget);
    painter->setPen(penForLines);
    QLineF line(0, 50, 100, 50);
    painter->drawLine(line);

    painter->setPen(penForDots);
    QPointF p1(0.5, 50);
    QPointF p2(99.5, 50);
    painter->drawPoint(p1);
    painter->drawPoint(p2);
}
#endif

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

#ifdef QTPAINT
void Resistor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Component::paint(painter, option, widget);

    painter->setPen(penForLines);

    // input line
    painter->drawLine(0, 50, 14, 50);
    // zigzag lines
    painter->drawLine(14, 50, 20, 30);
    painter->drawLine(20, 30, 31, 65);
    painter->drawLine(31, 65, 42, 30);
    painter->drawLine(42, 30, 53, 65);
    painter->drawLine(53, 65, 64, 30);
    painter->drawLine(64, 30, 75, 65);
    painter->drawLine(75, 65, 86, 50);
    // output line
    painter->drawLine(86, 50, 100, 50);

    painter->setFont(QFont("Times", 12, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignHCenter, QString::number(_resistance) + " Ohm");

    painter->setPen(penForDots);
    QPointF p1(0.5, 50);
    QPointF p2(99.5, 50);
    painter->drawPoint(p1);
    painter->drawPoint(p2);
}
#endif

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

#ifdef QTPAINT
void DCVoltage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Component::paint(painter, option, widget);

    painter->setPen(penForLines);

    // vertical line
    painter->drawLine(50, 0, 50, 40);

    painter->setPen(penForDots);

    QPointF p(50, 0.5);
    painter->drawPoint(p);

    painter->setPen(QPen(Qt::black));
    painter->setFont(QFont("Times", 12, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(_voltage) + " V");
}
#endif

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

    QPointF pUp(50,0.5);
    QPointF pDown(50,99.5);
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
