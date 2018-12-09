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
		out << c.lock()->name() << "\t";
	}

	return out;
}


//Node
Node::Node(int x, int y, const std::shared_ptr<Component> &component = nullptr)
:_x(x), _y(y)
{
    _components.clear();
	if (component) _components.push_back(component);
}

//Node coordinates
int Node::x() const { return _x; }
int Node::y() const { return _y; }

void Node::addComponent(const std::shared_ptr<Component> &e){
	_components.push_back(e);
}

//All components connected to node
std::vector<std::weak_ptr<Component>> Node::components() const{
	return _components;
}

//Only components of type 'componentType' connected to node
std::vector<std::weak_ptr<Component>> Node::components(char componentType) const {
	std::vector<std::weak_ptr<Component>> filterd;

	for (auto const& component : _components) {
		if (component.lock()->name()[0] == componentType) {
			filterd.push_back(component);
		}
	}

	return filterd;
}

std::vector<std::weak_ptr<Component>> Node::components(char componentType, int x, int y) {
	auto i = _allNodes.find(std::make_shared<Node>(x, y));
	if (i != _allNodes.end()) {
		return (*i)->components(componentType);
	} else {
		return std::vector<std::weak_ptr<Component>>();
	}
}

//Disconnect component e from this node
void Node::disconnectComponent(const std::shared_ptr<Component>& e) {
	auto i = _components.begin();
	for ( ; i < _components.end(); ++i) {
		if ((*i).lock() == e) {
			break;
		}
	}
	_components.erase(i);
}



//Component
Component::Component(const std::string &name)
	:_name(name)
{
	_nodes.clear();
}

Component::~Component()
{}

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
    auto p = shared_from_this();

    auto new_node_ptr = std::make_shared<Node>(x, y, p);
	_nodes.push_back(new_node_ptr);

	auto res = Node::_allNodes.insert(_nodes.back());
	if (!res.second) {
		_nodes.back() = *res.first;
		_nodes.back()->addComponent(p);
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

    auto p = shared_from_this();

    auto new_node_ptr = std::make_shared<Node>(x, y, p);
	_nodes[pos]->disconnectComponent(p);
	_nodes[pos] = new_node_ptr;

	auto res = Node::_allNodes.insert(_nodes[pos]);
	if (!res.second) {
		_nodes[pos] = *res.first;
		_nodes[pos]->addComponent(p);
	}
}

/*
Reconnect component from nodeFrom (x,y) to nodeTo (x,y)
for all leads connected to nodeFrom
*/
void Component::reconnect(int xFrom, int yFrom, int xTo, int yTo) {
	auto p = std::make_shared<Node>(xTo, yTo);

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


