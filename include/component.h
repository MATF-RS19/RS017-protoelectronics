#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "scene.h"
#include "math.h"

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QApplication>
#include <QPen>
#include <QDebug>
#include <QSize>

class Component: public QGraphicsItem
{
public:
    Component();
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    QRectF rec;
    QPen penForLines;
    QPen penForDots;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

class Ground: public Component {
public:
    Ground();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class Wire: public Component {
public:
    Wire();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class Resistor: public Component {
public:
    Resistor();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class DCVoltage: public Component {
public:
    DCVoltage();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class VoltageSource: public Component {
public:
    VoltageSource();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // COMPONENTS_H
