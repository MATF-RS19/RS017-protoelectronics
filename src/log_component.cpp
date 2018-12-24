#include "log_component.hpp"

template <typename T>
int Counter<T>::_counter(0);
LogicGate::LogicGate(const std::string &name)
    :Component(name)
{}
double LogicGate::voltage() const{
    return _nodes.back()->_v;
}
bool LogicGate::getBoolVoltage(double v){
    if(v >= 2.5){
        return true;
    }
    else{
        return false;
    }
}
ANDGate::ANDGate()
   : LogicGate("AND" + std::to_string(_counter+1))
   {}

void ANDGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){
    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);  
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);  
    _nodes[2]->_v = a && b == true ? 5.0 : 0.0;
}
ORGate::ORGate()
   : LogicGate("OR" + std::to_string(_counter+1))
   {}

void ORGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){
    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);  
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);  
    _nodes[2]->_v = a || b == true ? 5.0 : 0.0;
}
XORGate::XORGate()
   : LogicGate("XOR" + std::to_string(_counter+1))
   {}

void XORGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){

    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);  
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);  
    _nodes[2]->_v = a == b  ? 0.0 : 5.0;
}
NORGate::NORGate()
   : LogicGate("NOR" + std::to_string(_counter+1))
   {}

void NORGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){

    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);  
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);  
    _nodes[2]->_v = a || b == true ? 0.0 : 5.0;
}
NANDGate::NANDGate()
   : LogicGate("AND" + std::to_string(_counter+1))
   {}

void NANDGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){

    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);  
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);  
    _nodes[2]->_v = a && b == true ? 0.0 : 5.0;
}
NXORGate::NXORGate()
   : LogicGate("XOR" + std::to_string(_counter+1))
   {}

void NXORGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){

    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);  
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);  
    _nodes[2]->_v = a == b  ? 5.0 : 0.0;
}

NOTGate::NOTGate()
    : LogicGate ("NOT" + std::to_string(_counter+1))
    {}

void NOTGate::connect(int x1,int y1, int x2,int y2, int x3, int y3) {
    // TODO DENISE
}

#ifdef QTPAINT
QRectF LogicGate::boundingRect() const {
    return QRectF(0, 0, 180, 120);
}

void LogicGate::voltageDependedSetPen(QPainter* painter, unsigned id) {
    if(nodes().size() < id+1) {
        painter->setPen(penForLines);
        return;
    }

    if(nodes()[id]->_v > 0)
        painter->setPen(penForLeadsGreen);
    else if(nodes()[id]->_v < 0)
        painter->setPen(penForLeadsRed);
    else
        painter->setPen(penForLines);
}

void LogicGate::voltageDependedDrawLine(QLineF line, QPainter* painter, unsigned id) {
    voltageDependedSetPen(painter, id);
    painter->drawLine(line);
    painter->setPen(penForLines);
}

std::vector<std::pair<int, int>> LogicGate::connectionPoints(void) const {
    std::vector<std::pair<int, int>> dots;
    dots.reserve(3);

    // Find local coordinates of connection point
    QPointF localPoint1(boundingRect().x(),
                       boundingRect().y()+30);

    QPointF localPoint2(boundingRect().x(),
                       boundingRect().y()+90);

    QPointF localPoint3(boundingRect().x()+boundingRect().width(),
                       boundingRect().y()+boundingRect().height()/2);

    // And then map to scene coordinates
    auto scenePoint1 = mapToScene(localPoint1);
    auto scenePoint2 = mapToScene(localPoint2);
    auto scenePoint3 = mapToScene(localPoint3);
    dots.push_back(std::pair<int, int>(scenePoint1.x(), scenePoint1.y()));
    dots.push_back(std::pair<int, int>(scenePoint2.x(), scenePoint2.y()));
    dots.push_back(std::pair<int, int>(scenePoint3.x(), scenePoint3.y()));

    return dots;
}

void ANDGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //painter->drawRect(boundingRect());

    // First input lead
    voltageDependedDrawLine(QLineF(0,30,50,30), painter, 0);

    // Second input
    voltageDependedDrawLine(QLineF(0,90,50,90), painter, 1);

    // Component body
    painter->drawLine(50,10,50,110);
    painter->drawLine(50,10,100,10);
    painter->drawLine(50,110,100,110);
    painter->drawArc(QRect(62,10,75,100), -90*16, 180*16);

    // Output lead
    voltageDependedDrawLine(QLineF(137,60,180,60), painter, 2);

    // Connection points
    painter->setPen(penForDots);
    QPointF in1(0.5, 30);
    QPointF in2(0.5, 90);
    QPointF out(179.5, 60);
    painter->drawPoint(in1);
    painter->drawPoint(in2);
    painter->drawPoint(out);
}

void ORGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //LOGComponent::paint(painter, option, widget);

    // First input lead
    voltageDependedDrawLine(QLineF(0,30,58,30), painter, 0);

    // Second input
    voltageDependedDrawLine(QLineF(0,90,58,90), painter, 1);

    // Component body
    painter->drawArc(QRect(15,10,50,100), -90*16, 180*16);
    painter->drawLine(45,10,80,10);
    painter->drawLine(45,110,80,110);

    // Up and bottom arc
    painter->drawArc(QRect(10,10,140,110), 0, 90*16);
    painter->drawArc(QRect(10,10,140,100), -90*16, 90*16);

    // Output lead
    voltageDependedDrawLine(QLineF(150,60,180,60), painter, 2);

    // Connection points
    painter->setPen(penForDots);
    QPointF in1(0.5, 30);
    QPointF in2(0.5, 90);
    QPointF out(179.5, 60);
    painter->drawPoint(in1);
    painter->drawPoint(in2);
    painter->drawPoint(out);
}

void XORGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //LOGComponent::paint(painter, option, widget);

    // First input lead
    voltageDependedDrawLine(QLineF(0,30,58,30), painter, 0);

    // Second input
    voltageDependedDrawLine(QLineF(0,90,58,90), painter, 1);

    // Component body
    painter->drawArc(QRect(15,10,50,100), -90*16, 180*16);
    painter->drawLine(45,10,80,10);
    painter->drawLine(45,110,80,110);

    // Up and bottom arc
    painter->drawArc(QRect(10,10,140,110), 0, 90*16);
    painter->drawArc(QRect(10,10,140,100), -90*16, 90*16);

    // Xor input arc
    painter->drawArc(QRect(8,10,50,100), -90*16, 180*16);

    // Output lead
    voltageDependedDrawLine(QLineF(150,60,180,60), painter, 2);

    // Connection points
    painter->setPen(penForDots);
    QPointF in1(0.5, 30);
    QPointF in2(0.5, 90);
    QPointF out(179.5, 60);
    painter->drawPoint(in1);
    painter->drawPoint(in2);
    painter->drawPoint(out);
}

void NORGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //LOGComponent::paint(painter, option, widget);

    // First input lead
    voltageDependedDrawLine(QLineF(0,30,58,30), painter, 0);

    // Second input
    voltageDependedDrawLine(QLineF(0,90,58,90), painter, 1);

    // Component body
    painter->drawArc(QRect(15,10,50,100), -90*16, 180*16);
    painter->drawLine(45,10,80,10);
    painter->drawLine(45,110,80,110);

    // Up and bottom arc
    painter->drawArc(QRect(10,10,140,110), 0, 90*16);
    painter->drawArc(QRect(10,10,140,100), -90*16, 90*16);

    // Output lead
    voltageDependedDrawLine(QLineF(150,60,180,60), painter, 2);

    // Connection points
    painter->setPen(penForDots);
    QPointF in1(0.5, 30);
    QPointF in2(0.5, 90);
    QPointF out(179.5, 60);
    painter->drawPoint(in1);
    painter->drawPoint(in2);
    painter->drawPoint(out);

    // Negation at the end
    QPainterPath path;
    path.addEllipse(QPointF(150, 60), 1, 1);
    painter->drawPath(path);
}

void NANDGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //LOGComponent::paint(painter, option, widget);

    // First input lead
    voltageDependedDrawLine(QLineF(0,30,50,30), painter, 0);

    // Second input
    voltageDependedDrawLine(QLineF(0,90,50,90), painter, 1);

    // Component body
    painter->drawLine(50,10,50,110);
    painter->drawLine(50,10,100,10);
    painter->drawLine(50,110,100,110);
    painter->drawArc(QRect(62,10, 75,100), -90*16, 180*16);

    // Output lead
    voltageDependedDrawLine(QLineF(140,60,180,60), painter, 2);

    // Connection points
    painter->setPen(penForDots);
    QPointF in1(0.5, 30);
    QPointF in2(0.5, 90);
    QPointF out(179.5, 60);
    painter->drawPoint(in1);
    painter->drawPoint(in2);
    painter->drawPoint(out);

    // Negation at the end
    QPainterPath path;
    path.addEllipse(QPointF(139, 60), 1, 1);
    painter->drawPath(path);
}

void NXORGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //LOGComponent::paint(painter, option, widget);

    // First input lead
    voltageDependedDrawLine(QLineF(0,30,58,30), painter, 0);

    // Second input
    voltageDependedDrawLine(QLineF(0,90,58,90), painter, 1);

    // Component body
    painter->drawArc(QRect(15,10,50,100), -90*16, 180*16);
    painter->drawLine(45,10,80,10);
    painter->drawLine(45,110,80,110);

    // Up and bottom arc
    painter->drawArc(QRect(10,10,140,110), 0, 90*16);
    painter->drawArc(QRect(10,10,140,100), -90*16, 90*16);

    // Xor input arc
    painter->drawArc(QRect(8,10,50,100), -90*16, 180*16);

    // Output lead
    voltageDependedDrawLine(QLineF(150,60,180,60), painter, 2);

    // Connection points
    painter->setPen(penForDots);
    QPointF in1(0.5, 30);
    QPointF in2(0.5, 90);
    QPointF out(179.5, 60);
    painter->drawPoint(in1);
    painter->drawPoint(in2);
    painter->drawPoint(out);

    // Negation at the end
    QPainterPath path;
    path.addEllipse(QPointF(150, 60), 1, 1);
    painter->drawPath(path);
}

void NOTGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //LogicGate::paint(painter, option, widget);

    // Input lead
    voltageDependedDrawLine(QLineF(0,60,60,60), painter, 0);

    // Component body
    static const QPointF points[3] = {
        QPointF(60, 30),
        QPointF(120, 60),
        QPointF(60, 90)
    };
    painter->drawPolygon(points, 3);

    // Output lead
    voltageDependedDrawLine(QLineF(120,60,180,60), painter, 1);

    // Connection points
    painter->setPen(penForDots);
    QPointF in(0.5, 60);
    QPointF out(179.5, 60);
    painter->drawPoint(in);
    painter->drawPoint(out);

    // Negation at the end
    QPainterPath path;
    path.addEllipse(QPointF(120, 60), 1, 1);
    painter->drawPath(path);
}

std::vector<std::pair<int, int>> NOTGate::connectionPoints(void) const {
    std::vector<std::pair<int, int>> dots;
    dots.reserve(2);
    dots.push_back(std::pair<int, int>(this->x(), this->y()+boundingRect().height()/2));
    dots.push_back(std::pair<int, int>(this->x()+this->y()+boundingRect().width(), this->y()+boundingRect().height()/2));
    return dots;
}

#endif
