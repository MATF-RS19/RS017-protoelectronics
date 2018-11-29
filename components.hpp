#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__ 1

#include <string>
#include <vector>
#include <memory>


class Component;

class Node {
public:
	double _v;

	Node(int x, int y);

	int x() const;
	int y() const;

	int nodeID() const;

	static int numOfNodes() {
		return nodes_num;
	}

	void addComponent(const std::shared_ptr<Component>& e);

	std::vector<std::shared_ptr<Component>> components() const;

private:
	static int nodes_num;
	int _id;
	int _x, _y;
	std::vector<std::shared_ptr<Component>> _components;
};


class Component {
public:
	Component(std::string name, std::vector<std::shared_ptr<Node>> nodes);

	Component(std::string name,
			std::shared_ptr<Node> node1);

	Component(std::string name,
			std::shared_ptr<Node> node1,
			std::shared_ptr<Node> node2);

	Component(std::string name,
			std::shared_ptr<Node> node1,
			std::shared_ptr<Node> node2,
			std::shared_ptr<Node> node3);

	virtual ~Component();

	std::string name() const;

	virtual double voltage() const = 0;
	virtual double current() const = 0;
	virtual double power() const;

private:
	std::string _name;
protected:
	std::vector<std::shared_ptr<Node>> _nodes;
};


class Ground : public Component {
public:
	Ground(std::string name,
		std::shared_ptr<Node> node);

	double voltage() const override;

	double current() const override;
};


class Wire : public Component {
public:
	Wire(std::string name,
		std::shared_ptr<Node> node1,
		std::shared_ptr<Node> node2);

	double voltage() const override;

	double current() const override;

	std::shared_ptr<Node> otherNode(int id);

};


class Resistor : public Component {
public:
	Resistor(std::string name, double resistance,
			std::shared_ptr<Node> node1,
			std::shared_ptr<Node> node2);

	double resistance() const;

	double voltage() const override;

	double current() const override;

private:
	double _resistance;
};


class DCVoltage : public Component {
public:
	DCVoltage(std::string name, double voltage,
			std::shared_ptr<Node> node1,
			std::shared_ptr<Node> node2);

	DCVoltage(std::string name, double voltage,
			std::shared_ptr<Node> node);

	double voltage() const override;

	double current() const override;

private:
	double _voltage;
};


class Circuit {
public:
	Circuit(std::vector<std::shared_ptr<Node>> nodes);

	void addNode(std::shared_ptr<Node> node);

	void addComponent(std::shared_ptr<Component> component);

	std::vector<std::shared_ptr<Node>> nodes() const;

	std::vector<std::shared_ptr<Component>> components(std::shared_ptr<Node> node) const;
	
	std::vector<std::shared_ptr<Component>> components(std::shared_ptr<Node> node, char componentType) const;

	int numOfDCVoltages() const;

private:
	std::vector<std::shared_ptr<Node>> _nodes;
};



std::ostream& operator<<(std::ostream& out, const Component& r);


#endif /* __COMPONENTS_H__ */
