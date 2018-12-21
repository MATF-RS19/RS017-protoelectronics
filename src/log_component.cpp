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
#ifdef QTPAINT
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
#endif
