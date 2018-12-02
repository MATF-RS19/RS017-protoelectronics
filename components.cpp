#include "components.hpp"
#include <iostream>
#include <algorithm>

template<typename T>
int Counter<T>::_counter(0);

std::ostream& operator<<(std::ostream& out, const Component& c) {
	out << c.name() << std::endl
	<< "Nodes: [ ";
	for (auto n : c.nodes()) {
		out << n.lock()->id() << " ";
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
Node::Node(int x, int y)
:_x(x), _y(y)
{
}

int Node::x() const { return _x; }

int Node::y() const { return _y; }

void Node::addComponent(std::shared_ptr<Component> e){
	_components.push_back(e);
}

std::vector<std::shared_ptr<Component>> Node::components() const{
	return _components;
}

bool Node::operator==(const Node& n) const {
	if (this->id() != n.id()) return false;
	if (this->_x != n.x()) return false;
	if (this->_y != n.y()) return false;

	return true;
}

void Node::disconnectAll() {
	for (unsigned i = 0; i < _components.size(); ++i) {
		_components[i] = nullptr;
	}
	_components.resize(0);
}

void Node::disconnectComponent(const std::shared_ptr<Component>& e) {
	std::vector<std::vector<std::shared_ptr<Component>>::iterator> forDelete;

	for (auto i = _components.begin(); i < _components.end(); ++i) {
		if (**i == *e) {
			forDelete.push_back(i);
			*i = nullptr;
		}
	}

	for (auto j : forDelete) {
		_components.erase(j);
	}
}

//TODO
void Node::connectTo(std::shared_ptr<Node> n2) {
	auto n1 = std::make_shared<Node>(*this);
	auto size = _components.size();
	do {
		auto i = _components.begin();
		std::cout << "pocetak: " << **i << std::endl;
		(*i)->reconnectTo(n1, n2);
		size = _components.size();
	} while (size != 0);
}



//Component
Component::Component(const std::string &name,
		std::shared_ptr<Node> node1)
	:_name(name)
{
	_nodes.resize(0);
	_nodes.push_back(node1);
}

Component::Component(const std::string &name,
		std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2)
	:_name(name)
{
	_nodes.resize(0);
	_nodes.push_back(node1);
	_nodes.push_back(node2);
}

Component::Component(const std::string &name,
		std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2,
		std::shared_ptr<Node> node3)
	:_name(name)
{
	_nodes.resize(0);
	_nodes.push_back(node1);
	_nodes.push_back(node2);
	_nodes.push_back(node3);
}

Component::~Component()
{}
void Component::initNodeConnections(const std::shared_ptr<Component>& p) {

	for (auto n : _nodes) {
		n.lock()->addComponent(p);
	}
}

std::string Component::name() const {
	return _name;
}

std::vector<std::weak_ptr<Node>> Component::nodes() const {
	return _nodes;
}

bool Component::operator==(const Component& e) {
	if (this->_name != e.name()) return false;
	if (this->_nodes.size() != e.nodes().size()) return false;

	for (unsigned i = 0; i < _nodes.size(); ++i) {
		if (this->_nodes[i].lock() != e.nodes()[i].lock())
			return false;
	}

	return true;
}

/*
	reconnect component from nodeFrom to nodeTo
	for all leads connected to nodeFrom
*/
void Component::reconnectTo(const std::shared_ptr<Node>& from, const std::shared_ptr<Node>& to) {
	auto p = make_shared_ptr();
	for (unsigned i = 0; i < _nodes.size(); ++i) {
		if (*_nodes[i].lock() == *from) {
			_nodes[i].lock()->disconnectComponent(p);
			_nodes[i] = to;
			to->addComponent(p);
		}
	}
}

double Component::power() const {
	return voltage() * current();
}


//Ground
Ground::Ground(std::shared_ptr<Node> node)
	:Component("GND" + std::to_string(_counter+1), node)
{
	auto p = make_shared_ptr();
	initNodeConnections(p);

	//set Voltage to 0
	_nodes[0].lock()->_v = 0;
}

std::shared_ptr<Component> Ground::make_shared_ptr() const {
	return std::make_shared<Ground>(*this);
}

double Ground::voltage() const {
	return 0;
}

//TODO
double Ground::current() const {
	return 0;
}



//Wire
Wire::Wire(std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2)
	:Component("W" + std::to_string(_counter+1), node1, node2)
{
	auto p = make_shared_ptr();
	initNodeConnections(p);
}

std::shared_ptr<Component> Wire::make_shared_ptr() const {
	return std::make_shared<Wire>(*this);
}

double Wire::voltage() const {
	return 0;
}

//TODO
double Wire::current() const {
	return 0;
}

std::shared_ptr<Node> Wire::otherNode(int id) {
	return _nodes[0].lock()->id() == id ? _nodes[1].lock() : _nodes[0].lock();
}


//Resistor
Resistor::Resistor(double resistance,
		std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2)
	:Component("R" + std::to_string(_counter+1), node1, node2), _resistance(resistance)
{
	auto p = make_shared_ptr();
	initNodeConnections(p);
}

std::shared_ptr<Component> Resistor::make_shared_ptr() const {
	return std::make_shared<Resistor>(*this);
}

double Resistor::resistance() const {
	return _resistance;
}

double Resistor::voltage() const {
	return _nodes[1].lock()->_v - _nodes[0].lock()->_v;
}

double Resistor::current() const {
	return voltage() / _resistance;
}


//DCVoltage
DCVoltage::DCVoltage(double voltage,
		std::shared_ptr<Node> node):
	Component("U" + std::to_string(_counter+1), node), _voltage(voltage)
{
	auto p = make_shared_ptr();
	initNodeConnections(p);

	_nodes[0].lock()->_v = voltage;
}

std::shared_ptr<Component> DCVoltage::make_shared_ptr() const {
	return std::make_shared<DCVoltage>(*this);
}

double DCVoltage::voltage() const {
	return _voltage;
}

//TODO
double DCVoltage::current() const {
	return 0;
}


//Circuit
Circuit::Circuit(std::vector<std::shared_ptr<Node>> nodes)
	:_nodes(nodes)
{}

void Circuit::addNode(std::shared_ptr<Node> node) {
	_nodes.push_back(node);
}

void Circuit::addComponent(std::shared_ptr<Component> component) {
	(void)component;
	//TODO
}

std::vector<std::shared_ptr<Node>> Circuit::nodes() const {
	return _nodes;
}

std::vector<std::shared_ptr<Component>> Circuit::components(std::shared_ptr<Node> node) const {
	return node->components();
}

std::vector<std::shared_ptr<Component>> Circuit::components(std::shared_ptr<Node> node, char componentType) const {
	std::vector<std::shared_ptr<Component>> filterd;

	auto components = node->components();
	for (auto component : components ) {
		if (component->name()[0] == componentType) {
			filterd.push_back(component);
		}
	}
	return filterd;
}

