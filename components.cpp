#include "components.hpp"
#include <iostream>
#include <algorithm>

template<typename T>
int Counter<T>::_counter(0);

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

void Node::disconnectAll() {
	for (unsigned i = 0; i < _components.size(); ++i) {
		_components[i] = nullptr;
	}
	_components.resize(0);
}

//TODO
void Node::connectTo(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
	for (unsigned i = 0; i < _components.size(); ++i) {
		_components[i]->reconnectTo(n1, n2);

		std::cout << _components[i]->name() << " : " << _components[i]->nodes()[0]->id() << std::endl;
	}
}


//Component
Component::Component(const std::string &name,
		std::vector<std::shared_ptr<Node>> nodes):
	_name(name), _nodes(nodes)
{
}

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

std::string Component::name() const {
	return _name;
}

std::vector<std::shared_ptr<Node>> Component::nodes() const {
	return _nodes;
}

double Component::power() const {
	return voltage() * current();
}


//Ground
Ground::Ground(std::shared_ptr<Node> node)
	:Component("GND" + std::to_string(_counter+1), node)
{
	if (_nodes[0]) _nodes[0]->addComponent(std::make_shared<Ground>(*this));

	if (_nodes[0]->_v != 0) {
		std::cerr << "Error! Short circuit!" << std::endl;
	}

	//set Voltage to 0
	_nodes[0]->_v = 0;
}

double Ground::voltage() const {
	return 0;
}

//TODO
double Ground::current() const {
	return 0;
}

void Ground::reconnectTo(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
	for (unsigned i = 0; i < _nodes.size(); ++i) {
		if (_nodes[i] == n1) {
			_nodes[i] = n2;
			n2->addComponent(std::make_shared<Ground>(*this));
			//TODO osloboditi cvor koji pokazuje na njega
		}
	}
}


//Wire
Wire::Wire(std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2)
	:Component("W" + std::to_string(_counter+1), node1, node2)
{
	if (_nodes[0]) _nodes[0]->addComponent(std::make_shared<Wire>(*this));
	if (_nodes[1]) _nodes[1]->addComponent(std::make_shared<Wire>(*this));
}

double Wire::voltage() const {
	return 0;
}

//TODO
double Wire::current() const {
	return 0;
}

void Wire::reconnectTo(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
	for (unsigned i = 0; i < _nodes.size(); ++i) {
		if (_nodes[i] == n1) {
			_nodes[i] = n2;
			n2->addComponent(std::make_shared<Wire>(*this));
			//TODO osloboditi cvor koji pokazuje na njega
		}
	}
}

std::shared_ptr<Node> Wire::otherNode(int id) {
	return _nodes[0]->id() == id ? _nodes[1] : _nodes[0];
}


//Resistor
Resistor::Resistor(double resistance,
		std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2)
	:Component("R" + std::to_string(_counter+1), node1, node2), _resistance(resistance)
{
	if (_nodes[0]) _nodes[0]->addComponent(std::make_shared<Resistor>(*this));
	if (_nodes[1]) _nodes[1]->addComponent(std::make_shared<Resistor>(*this));
}

double Resistor::resistance() const {
	return _resistance;
}

double Resistor::voltage() const {
	return _nodes[1]->_v - _nodes[0]->_v;
}

double Resistor::current() const {
	return voltage() / _resistance;
}

void Resistor::reconnectTo(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
	for (unsigned i = 0; i < _nodes.size(); ++i) {
		if (_nodes[i] == n1) {
			_nodes[i] = n2;
			n2->addComponent(std::make_shared<Resistor>(*this));
			//TODO osloboditi cvor koji pokazuje na njega
		}
	}
}


//DCVoltage
DCVoltage::DCVoltage(double voltage,
		std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2):
	Component("U" + std::to_string(_counter+1), node1, node2), _voltage(voltage)
{
	if (_nodes[0]) _nodes[0]->addComponent(std::make_shared<DCVoltage>(*this));
	if (_nodes[1]) _nodes[1]->addComponent(std::make_shared<DCVoltage>(*this));
	//TODO
}

DCVoltage::DCVoltage(double voltage,
		std::shared_ptr<Node> node):
	Component("U" + std::to_string(_counter+1), node), _voltage(voltage)
{
	if (_nodes[0]) _nodes[0]->addComponent(std::make_shared<DCVoltage>(*this));
	if (_nodes[1]) _nodes[1]->addComponent(std::make_shared<DCVoltage>(*this));

	_nodes[0]->_v = voltage;
}

double DCVoltage::voltage() const {
	return _voltage;
}

//TODO
double DCVoltage::current() const {
	return 0;
}

void DCVoltage::reconnectTo(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
	for (unsigned i = 0; i < _nodes.size(); ++i) {
		if (_nodes[i] == n1) {
			_nodes[i] = n2;
			n2->addComponent(std::make_shared<DCVoltage>(*this));
			//TODO osloboditi cvor koji pokazuje na njega
		}
	}
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

