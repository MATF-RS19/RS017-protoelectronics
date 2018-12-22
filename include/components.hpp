#ifndef COMPONENTS_H
#define COMPONENTS_H

#define QTPAINT

#ifdef QTPAINT
#include "scene.h"
#include "math.h"

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
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
#include <cassert>

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

    //Creates node with given coordinates and optionally adds connection to component
	Node(int x, int y, Component* const component);

    //Returns node coordinates
	int x() const;
	int y() const;

    //Adds component to node. Establishes connection from node to component
	void addComponent(Component* const e);

    //Returns all direct components connected to node
	std::vector<Component*> directComponents() const;

    //Returns only direct components of type 'componentType' connected to node
	std::vector<Component*> directComponents(const std::string& componentType) const;

    //Returns all components connected to node with those connected with wire
	std::vector<Component*> components() const;

    //Returns all components of type 'componentType' connected to node, including wire connections
	std::vector<Component*> components(const std::string& componentType) const;

    //Returns iterator to component
    std::vector<Component*>::iterator find(Component* const e);

    /*
     * Checks if node have connection with component
     * NOTE: only connections from node to component
    */
     bool isConnectedTo(Component* const e);

    //Disconnects node from component (delete connection node->component)
	void disconnectFromComponent(Component* const e);

	//Two nodes are the same if they have the same coordinates
	struct lex_node_cmp {
		bool operator() (const std::shared_ptr<Node>& n1,
				const std::shared_ptr<Node>& n2) const {
			return n1->to_string() < n2->to_string();
		}
	};

    //Set of all different nodes, compared by coordinates
	static std::set<std::shared_ptr<Node>, lex_node_cmp> _allNodes;

    //Finds all components with componentType directly connected to node (x, y)
	static std::vector<Component*> findDirectlyConnected(const std::string& componentType, int x, int y);
    
    //Finds all components with componentType directly or by wire connected to node (x, y)
	static std::vector<Component*> find(const std::string& componentType, int x, int y);

    //Finds node by coordinates
    static std::set<std::shared_ptr<Node>, lex_node_cmp>::iterator find(int x, int y);

    static size_t size() {
        return _allNodes.size();
    }

private:
	//node coordinates
	int _x, _y;
	//connected components to node
	std::vector<Component*> _components;

    //string representation of coordinates: from (x, y) to "xy"
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
    virtual QRectF boundingRect() const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual std::vector<std::pair<int, int>> connectionPoints(void) const = 0;
#endif

    virtual ~Component();

    virtual std::string componentType() const = 0;

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

	std::string name() const;

    //All nodes that component have
	std::vector<std::shared_ptr<Node>> nodes() const;

    //Finds first node connected to component by coordinates
    std::vector< std::shared_ptr<Node> >::iterator find(int x, int y);

    /*
     * Checks if component have connection with node
     * NOTE: only connections from component to node
    */
    bool isConnectedTo(int x, int y);

    /*
     * Connects component to node if that node exist,
     * if not, makes new node and connects
    */
    virtual void addNode(int x, int y);

    //Disconnects component from all nodes with given coordinates
    void disconnect(int x, int y);

    /*
     * Disconnects component from all nodes with given coordinates
     * (connection node->component will be removed also)
     * and connects them to another node
	*/
    void reconnect(int xFrom, int yFrom, int xTo, int yTo);

	virtual double voltage() const = 0;
	virtual double current() const = 0;
	virtual double power() const;

private:
	std::string _name;

    /*
     * Removes both connections: component->node and node->component
     * and keeps nullptr at that place because another node will took his place
     * For use only in `addNodeAt` method
    */
    template <typename Iter>
    void disconnectAndPreserveEmptyPlace(Iter &it);

    /*
     * Same as function addNode, but adds node at given position
     * overrides anything that was at that position
     * For use only in `reconnect` method
    */
    template <typename Iter>
	void addNodeAt(Iter &pos, int x, int y);

    //friend std::ostream& operator<<(std::ostream& out, const Component& c);

protected:
	//component is connected to nodes
    std::vector<std::shared_ptr<Node>> _nodes;
    //std::string toString() const;

#ifdef QTPAINT
    QPen penForLines;
    QPen penForDots;
    QPen penForLeadsGreen;
    QPen penForLeadsRed;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

#endif
};


class Ground : public Component, public Counter<Ground> {
public:
	Ground();
    
    std::string componentType() const override { return "ground"; }

	double voltage() const override;

	double current() const override;

    void addNode(int x, int y) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    std::vector<std::pair<int, int>> connectionPoints(void) const override;
#endif
};


class Wire : public Component, public Counter<Wire> {
public:
	Wire();
    
    std::string componentType() const override { return "wire"; }

	double voltage() const override;

	double current() const override;

    std::shared_ptr<Node> otherNode(int id);

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    std::vector<std::pair<int, int>> connectionPoints(void) const override;
#endif
};


class Resistor : public Component, public Counter<Resistor> {
public:
	Resistor(double resistance);
    
    std::string componentType() const override {return "resistor";}

	double resistance() const;

	double voltage() const override;

	double current() const override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    std::vector<std::pair<int, int>> connectionPoints(void) const override;
#endif

private:
	double _resistance;
};


class DCVoltage : public Component, public Counter<DCVoltage> {
public:
	DCVoltage(double voltage);

    std::string componentType() const override {return "voltage";}

	double voltage() const override;

	double current() const override;

	void addNode(int x, int y) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    std::vector<std::pair<int, int>> connectionPoints(void) const override;
#endif

private:
	double _voltage;
};


std::ostream& operator<<(std::ostream& out, const Component& r);

std::ostream& operator<<(std::ostream& out, const Node& r);

#endif /*COMPONENTS_H */
