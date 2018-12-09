#ifndef LOGCOMPONENT_H
#define LOGCOMPONENT_H

#include "scene.h"
#include "math.h"

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QApplication>
#include <QPen>
#include <QDebug>
#include <QSize>

class LOGComponent: public QGraphicsItem
{
public:
    LOGComponent();
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    QRectF rec;
    QPen penForLines;
    QPen penForDots;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

class ANDComponent: public LOGComponent {
public:
    ANDComponent();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // LOGCOMPONENT_H
