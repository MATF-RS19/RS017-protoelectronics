#include "components.hpp"
#include <iostream>
#include <algorithm>


std::ostream& operator<<(std::ostream& out, const Component& c) {
	out << c.name() << std::endl
		<< "U = " << c.voltage() << " V" << std::endl
		<< "I = " << c.current() << " A" << std::endl
		<< "P = " << c.power() << " W";
	return out;
}

//Node
int Node::nodes_num = 0;

Node::Node(int x, int y)
:_x(x), _y(y)
{
	_id = ++nodes_num;
}

int Node::x() const { return _x; }

int Node::y() const { return _y; }

int Node::nodeID() const {
	return _id;
}

void Node::addComponent(const std::shared_ptr<Component>& e){
	_components.push_back(e);
}

std::vector<std::shared_ptr<Component>> Node::components() const{
	return _components;
}


//Component
Component::Component(std::string name, std::vector<std::shared_ptr<Node>> nodes):
	_name(name), _nodes(nodes)
{}

Component::Component(std::string name,
		std::shared_ptr<Node> node1)
	:_name(name)
{
	_nodes.resize(0);
	_nodes.push_back(node1);
}

Component::Component(std::string name,
		std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2)
	:_name(name)
{
	_nodes.resize(0);
	_nodes.push_back(node1);
	_nodes.push_back(node2);
}

Component::Component(std::string name,
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

double Component::power() const {
	return voltage() * current();
}


//Ground
Ground::Ground(std::string name,
	std::shared_ptr<Node> node)
	:Component(name, node)
{
	if (_nodes[0]) _nodes[0]->addComponent(std::make_shared<Ground>(*this));

	if (_nodes[0]->_v != 0) {
		std::cerr << "Error! Short circuit!" << std::endl;
	}
	_nodes[0]->_v = 0;
}

double Ground::voltage() const {
	return 0;
}

//TODO
double Ground::current() const {
	return 0;
}


//Wire
Wire::Wire(std::string name,
	std::shared_ptr<Node> node1,
	std::shared_ptr<Node> node2)
	:Component(name, node1, node2)
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

std::shared_ptr<Node> Wire::otherNode(int id) {
	return _nodes[0]->nodeID() == id ? _nodes[1] : _nodes[0];
}


//Resistor
Resistor::Resistor(std::string name, double resistance,
		std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2)
	:Component(name, node1, node2), _resistance(resistance)
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


//DCVoltage
DCVoltage::DCVoltage(std::string name, double voltage,
		std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2):
	Component(name, node1, node2), _voltage(voltage)
{
	if (_nodes[0]) _nodes[0]->addComponent(std::make_shared<DCVoltage>(*this));
	if (_nodes[1]) _nodes[1]->addComponent(std::make_shared<DCVoltage>(*this));
	//TODO
}

DCVoltage::DCVoltage(std::string name, double voltage,
		std::shared_ptr<Node> node):
	Component(name, node), _voltage(voltage)
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

int Circuit::numOfDCVoltages() const {
	int num = 0;

	for (unsigned i = 0; i < _nodes.size(); ++i) {
		auto components = _nodes[i]->components();
		auto j = std::find_if(components.cbegin(), components.cend(),
				[](std::shared_ptr<Component> c) -> bool {return c->name()[0] == 'U' ? true : false;});
		if (j != components.cend()) {
			//std::cout << (*j)->name() << std::endl;
			num++;
		}
	}

	return num;
}
