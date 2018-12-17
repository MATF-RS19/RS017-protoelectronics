#ifndef COMPONENTS_H
#define COMPONENTS_H

//#define QTPAINT

#ifdef QTPAINT
#include "scene.h"
#include "math.h"

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QApplication>
#include <QPen>
#include <QDebug>
#include <QSize>
#endif

#include <string>
#include <vector>
#include <memory>
#include <set>
#include <algorithm> //std::find_if

//Interface for counting and naming components in the same class
template <class T>
class Counter {
protected:
	Counter() {
		_id = ++_counter;
	}

	~Counter() {
		--_counter;
	}

	static int _counter;
	int _id;

public:
	static int counter() {
		return _counter;
	}

	int id() const {
		return _id;
	}
};


class Component;

class Node : public Counter<Node> {
public:
    //voltage in node
	double _v;

	Node(int x, int y, Component* const component);

	int x() const;
	int y() const;

	void addComponent(Component* const e);

    void pop_component();

	std::vector<Component*> components() const;

	std::vector<Component*> components(char componentType) const;

	void disconnectComponent(Component* const e);

	//two nodes are same if they have the same coordinates
	struct lex_node_cmp {
		bool operator() (const std::shared_ptr<Node>& n1,
				const std::shared_ptr<Node>& n2) const {
			return n1->to_string() < n2->to_string();
		}
	};

	static std::set<std::shared_ptr<Node>, lex_node_cmp> _allNodes;

    //find all components with componentType connected to node (x, y)
	static std::vector<Component*> find(char componentType, int x, int y);

    //find node by coordinates
    static std::set<std::shared_ptr<Node>, lex_node_cmp>::iterator find(int x, int y);

    static size_t size() {
        return _allNodes.size();
    }

private:
	//node coordinates
	int _x, _y;
	//connected components to node
	std::vector<Component*> _components;

    std::string to_string() const {
        return std::to_string(_x) + std::to_string(_y);
    }
};


class Component
				#ifdef QTPAINT
				: public QGraphicsItem
				#endif
				{
public:
	Component(const std::string &name);

#ifdef QTPAINT
    QRectF boundingRect() const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
#endif

    virtual ~Component();

	std::string name() const;

	std::vector<std::shared_ptr<Node>> nodes() const;

    //Find first node connected to component by coordinates
    std::vector< std::shared_ptr<Node> >::iterator find(int x, int y);

    /*
    Connect component to node if that node exist,
    if not, make new node and connect
    */
    virtual void addNode(int x, int y);

    void addNodes(int x1, int y1, int x2, int y2);
    void addNodes(int x1, int y1, int x2, int y2, int x3, int y3);
	void addNodeAt(unsigned pos, int x, int y);

	void reconnect(int xFrom, int yFrom, int xTo, int yTo);

	virtual double voltage() const = 0;
	virtual double current() const = 0;
	virtual double power() const;

private:
	std::string _name;

protected:
	//component is connected to nodes
    std::vector<std::shared_ptr<Node>> _nodes;

#ifdef QTPAINT
    QPen penForLines;
    QPen penForDots;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
#endif
};


class Ground : public Component, public Counter<Ground> {
public:
	Ground();

	double voltage() const override;

	double current() const override;

    void addNode(int x, int y) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif
};


class Wire : public Component, public Counter<Wire> {
public:
	Wire();

	double voltage() const override;

	double current() const override;

    std::shared_ptr<Node> otherNode(int id);

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif
};


class Resistor : public Component, public Counter<Resistor> {
public:
	Resistor(double resistance);

	double resistance() const;

	double voltage() const override;

	double current() const override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

private:
	double _resistance;
};


class DCVoltage : public Component, public Counter<DCVoltage> {
public:
	DCVoltage(double voltage);

	double voltage() const override;

	double current() const override;

	void addNode(int x, int y) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

private:
	double _voltage;
};


std::ostream& operator<<(std::ostream& out, const Component& r);

std::ostream& operator<<(std::ostream& out, const Node& r);

#endif /*COMPONENTS_H */
