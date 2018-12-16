#include "log_component.hpp"

LogicGate::~LogicGate()
{}
double LogicGate::voltage(){
    return _nodes.back()->_v;
}
bool LogicGate::getLogValue(double v){
    if(v >= 2.5){
        return true;
    }
    else{
        return false;
    }
}
ANDGate::ANDGate(){
   : Component("AND" + std::to_string(_counter+1))
   {}
}
double ANDGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){
    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getLogValue(_nodes[0]->_v);  
    bool b = LogicGate::getLogValue(_nodes[1]->_v);  
    return _nodes[2]->_v = a && b == true ? 5.0 : 0.0;
}
ORGate::ORGate(){
   : Component("OR" + std::to_string(_counter+1))
   {}
}
double ORGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){
    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getLogValue(_nodes[0]->_v);  
    bool b = LogicGate::getLogValue(_nodes[1]->_v);  
    return _nodes[2]->_v = a || b == true ? 5.0 : 0.0;
}
XORGate::XORGate(){
   : Component("XOR" + std::to_string(_counter+1))
   {}
}
double XORGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){

    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getLogValue(_nodes[0]->_v);  
    bool b = LogicGate::getLogValue(_nodes[1]->_v);  
    return _nodes[2]->_v = a == b  ? 0.0 : 5.0;
}
NORGate::NORGate(){
   : Component("NOR" + std::to_string(_counter+1))
   {}
}
double NORGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){

    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getLogValue(_nodes[0]->_v);  
    bool b = LogicGate::getLogValue(_nodes[1]->_v);  
    return _nodes[2]->_v = a || b == true ? 0.0 : 5.0;
}
NANDGate::NANDGate(){
   : Component("AND" + std::to_string(_counter+1))
   {}
}
double NANDGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){

    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getLogValue(_nodes[0]->_v);  
    bool b = LogicGate::getLogValue(_nodes[1]->_v);  
    return _nodes[2]->_v = a && b == true ? 0.0 : 5.0;
}
NXORGate::NXORGate(){
   : Component("XOR" + std::to_string(_counter+1))
   {}
}
double NXORGate::connect(int x1, int y1, int x2, int y2, int x3, int y3){

    addNode(x1, y1);
    addNode(x2, y2);
    addNode(x3, y3);
    bool a = LogicGate::getLogValue(_nodes[0]->_v);  
    bool b = LogicGate::getLogValue(_nodes[1]->_v);  
    return _nodes[2]->_v = a == b  ? 5.0 : 0.0;
}
void ANDGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override{
    //TODO
}

void ORGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override{
    //TODO
}
void XORGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override{
    //TODO
}
void NORGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override{
    //TODO
}
void NANDGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override{
    //TODO
}
void NXORGate::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override{
    //TODO
}
