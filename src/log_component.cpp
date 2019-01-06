#include "log_component.hpp"

#ifdef QTPAINT
#include <QFontMetrics>
#endif

template <typename T>
int Counter<T>::_counter(0);
LogicGate::LogicGate(const std::string &name)
    :Component(name)
{}

LogicGate::~LogicGate() {
    disconnect();
}

void LogicGate::disconnect(int x, int y) {
    auto it = Node::find(x, y);
    if (it == Node::_allNodes.end()) return;

    //Remove possible voltage on output node
    if (_nodes.size() == 3 && (*it) == _nodes[2]) (*it)->_v = 0;
    updateVoltages((*it));
    Component::disconnect(x, y);
}

void LogicGate::disconnect() {
    if (_nodes.size() == 3) {
        //Remove possible voltage on output node
        _nodes.back()->_v = 0;
        updateVoltages(_nodes.back());
    }
    Component::disconnect();
}

bool LogicGate::getBoolVoltage(double v){
    if(v >= 2.5){
        return true;
    }
    else{
        return false;
    }
}

void LogicGate::connect(const std::vector<std::pair<int, int>>& connPts) {
    Component::connect(connPts);
    voltage();
}

std::string LogicGate::toString() const {
    std::stringstream str;
    str << name() << std::endl;

    str << std::fixed << std::setprecision(2);
    str << "in1: " << _nodes[0]->_v << " V" << std::endl;
    str << "in2: " << _nodes[1]->_v << " V" << std::endl;
    str << "out: " << _nodes[2]->_v << " V" << std::endl;
    return str.str();
}


ANDGate::ANDGate()
   : LogicGate("AND" + std::to_string(_counter+1))
   {}

double ANDGate::voltage() const {
    if (_nodes.size() != 3) return 0;

    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);
    _nodes[2]->_v = (a && b ? 5.0 : 0.0);
    updateVoltages(_nodes[2]);
    return _nodes[2]->_v;
}



ORGate::ORGate()
   : LogicGate("OR" + std::to_string(_counter+1))
   {}

double ORGate::voltage() const {
    if (_nodes.size() != 3) return 0;

    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);
    _nodes[2]->_v = (a || b ? 5.0 : 0.0);
    updateVoltages(_nodes[2]);
    return _nodes[2]->_v;
}


XORGate::XORGate()
   : LogicGate("XOR" + std::to_string(_counter+1))
   {}

double XORGate::voltage() const {
    if (_nodes.size() != 3) return 0;

    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);
    _nodes[2]->_v = (a == b ? 0.0 : 5.0);
    updateVoltages(_nodes[2]);
    return _nodes[2]->_v;
}


NORGate::NORGate()
   : LogicGate("NOR" + std::to_string(_counter+1))
   {}

double NORGate::voltage() const {
    if (_nodes.size() != 3) return 0;

    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);
    _nodes[2]->_v = (a || b ? 0.0 : 5.0);
    updateVoltages(_nodes[2]);
    return _nodes[2]->_v;
}


NANDGate::NANDGate()
   : LogicGate("AND" + std::to_string(_counter+1))
   {}

double NANDGate::voltage() const {
    if (_nodes.size() != 3) return 0;

    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);
    _nodes[2]->_v = (a && b ? 0.0 : 5.0);
    updateVoltages(_nodes[2]);
    return _nodes[2]->_v;
}


NXORGate::NXORGate()
   : LogicGate("XOR" + std::to_string(_counter+1))
   {}

double NXORGate::voltage() const {
    if (_nodes.size() != 3) return 0;

    bool a = LogicGate::getBoolVoltage(_nodes[0]->_v);
    bool b = LogicGate::getBoolVoltage(_nodes[1]->_v);
    _nodes[2]->_v = (a == b ? 5.0 : 0.0);
    updateVoltages(_nodes[2]);
    return _nodes[2]->_v;
}


NOTGate::NOTGate()
    : LogicGate ("NOT" + std::to_string(_counter+1))
    {}

NOTGate::~NOTGate() {
    disconnect();
}

double NOTGate::voltage() const {
    if (_nodes.size() != 2) return 0;

    bool in = LogicGate::getBoolVoltage(_nodes[0]->_v);
    _nodes[1]->_v = (in ? 0.0 : 5.0);
    updateVoltages(_nodes[1]);
    return _nodes[1]->_v;
}


void NOTGate::disconnect(int x, int y) {
    auto it = Node::find(x, y);
    if (it == Node::_allNodes.end()) return;

    //Remove possible voltage on output node
    if (_nodes.size() == 2 && (*it) == _nodes[1]) (*it)->_v = 0;
    updateVoltages((*it));
    Component::disconnect(x, y);
}

void NOTGate::disconnect() {
    if (_nodes.size() == 2) {
        //Remove possible voltage on output node
        _nodes.back()->_v = 0;
        updateVoltages(_nodes.back());
    }
    Component::disconnect();
}


JKFlipFlop::JKFlipFlop()
    : LogicGate ("JKFlipFlop" + std::to_string(_counter+1)),
      _old_clk(false)
    {}

JKFlipFlop::~JKFlipFlop() {
    disconnect();
}

void JKFlipFlop::set() const {
    _nodes[Q]->_v = 5;
    _nodes[Qc]->_v = 0;
    updateVoltages(_nodes[Q]);
    updateVoltages(_nodes[Qc]);
}

void JKFlipFlop::reset() const {
    _nodes[Q]->_v = 0;
    _nodes[Qc]->_v = 5;
    updateVoltages(_nodes[Q]);
    updateVoltages(_nodes[Qc]);
}

double JKFlipFlop::voltage() const {
    //take new input values
    int new_j = getBoolVoltage(_nodes[J]->_v);
    int new_k = getBoolVoltage(_nodes[K]->_v);
    int new_clk = getBoolVoltage(_nodes[CLK]->_v);

    //we can set or reset flip-flip
    //this is down edge of clock: from 1 to 0
    if (_old_clk && !new_clk) {
        //set J=1, K=0
        if(new_j && !new_k) {
            set();
        }
        //reset J=0, K=1
        else if(!new_j && new_k) {
            reset();
        }
        //change state J=1, K=1
        else if(new_j && new_k) {
            //state was set
            if (getBoolVoltage(_nodes[Q]->_v)) {
                reset();
            } else {
                set();
            }
        }
        //keep old state J=0, K=0
    }
    _old_clk = new_clk;
    return _nodes[Q]->_v;
}

void JKFlipFlop::disconnect(int x, int y) {
    auto it = Node::find(x, y);
    if (it == Node::_allNodes.end()) return;

    //Remove possible voltage on output nodes
    if (_nodes.size() == 5 && ((*it) == _nodes[Q] || *(it) == _nodes[Qc])) (*it)->_v = 0;
    updateVoltages((*it));
    Component::disconnect(x, y);
}

void JKFlipFlop::disconnect() {
    if (_nodes.size() == 5) {
        //Remove possible voltage on output nodes
        _nodes[Q]->_v = 0;
        _nodes[Qc]->_v = 0;
        updateVoltages(_nodes[Q]);
        updateVoltages(_nodes[Qc]);
    }
    Component::disconnect();
}

std::string JKFlipFlop::toString() const {
    //TODO David
    return Component::toString();
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

	// Setting color for painter depended on voltage
    if(nodes()[id]->_v > 0)
        painter->setPen(penForLeadsGreen);
    else if(nodes()[id]->_v < 0)
        painter->setPen(penForLeadsRed);
    else
        painter->setPen(penForLines);
}

void LogicGate::voltageDependedDrawLine(QLineF line, QPainter* painter, unsigned id) {
	// Draw line depending on voltage
    voltageDependedSetPen(painter, id);
    painter->drawLine(line);
    update();

	// Set color of line back to default
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
	QPointF in1(1, 30);
	QPointF in2(1, 90);
	QPointF out(179, 60);
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
	QPointF in1(1, 30);
	QPointF in2(1, 90);
	QPointF out(179, 60);
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
	QPointF in1(1, 30);
	QPointF in2(1, 90);
	QPointF out(179, 60);
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
	QPointF in1(1, 30);
	QPointF in2(1, 90);
	QPointF out(179, 60);
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
	QPointF in1(1, 30);
	QPointF in2(1, 90);
	QPointF out(179, 60);
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
	QPointF in1(1, 30);
	QPointF in2(1, 90);
	QPointF out(179, 60);
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
	QPointF in(1, 60);
	QPointF out(179, 60);
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

	// Find local coordinates of connection point
	QPointF localPoint1(boundingRect().x(),
						boundingRect().y()+boundingRect().height()/2);

	QPointF localPoint2(boundingRect().x()+boundingRect().width(),
						boundingRect().y()+boundingRect().height()/2);

	// And then map to scene coordinates
	auto scenePoint1 = mapToScene(localPoint1);
	auto scenePoint2 = mapToScene(localPoint2);
	dots.push_back(std::pair<int, int>(scenePoint1.x(), scenePoint1.y()));
	dots.push_back(std::pair<int, int>(scenePoint2.x(), scenePoint2.y()));

	return dots;
}

QRectF JKFlipFlop::boundingRect() const {
	return QRectF(0, 0, 160, 180);
}

void JKFlipFlop::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);

	// Input lines
	voltageDependedDrawLine(QLineF(0, 40, 20, 40), painter, 0);
	voltageDependedDrawLine(QLineF(0, 90, 20, 90), painter, 1);
	voltageDependedDrawLine(QLineF(0, 140, 20, 140), painter, 2);

	// Output liness
	voltageDependedDrawLine(QLineF(140, 40, 160, 40), painter, 3);
	voltageDependedDrawLine(QLineF(140, 140, 160, 140), painter, 4);

	// Body
	QRectF rect(20, 15, 120, 150);
	painter->drawRect(rect);

	// Letters
    painter->setFont(QFont("Times", 25, QFont::Bold));
    painter->drawText(rect, Qt::AlignLeft, " J");
    painter->drawText(rect, Qt::AlignRight, "Q ");
    painter->drawText(rect, Qt::AlignBottom , " K");
    painter->drawText(rect, Qt::AlignBottom | Qt::AlignRight, "Qc ");

	// Connection points
	painter->setPen(penForDots);
    QPointF in1(1, 40);
	QPointF in2(1, 90);
    QPointF in3(1, 140);
    QPointF out1(159, 40);
    QPointF out2(159, 140);
	painter->drawPoint(in1);
	painter->drawPoint(in2);
	painter->drawPoint(in3);
	painter->drawPoint(out1);
	painter->drawPoint(out2);

	// Circle on middle line
	QPainterPath path;
	path.addEllipse(QPointF(20, 90), 1, 1);
	painter->drawPath(path);
}

std::vector<std::pair<int, int>> JKFlipFlop::connectionPoints(void) const {
	std::vector<std::pair<int, int>> dots;
	dots.reserve(5);

	// Find local coordinates of connection point
	QPointF localPoint1(boundingRect().x(),
                        boundingRect().y()+40);

	QPointF localPoint2(boundingRect().x(),
						boundingRect().y()+boundingRect().height()/2);

	QPointF localPoint3(boundingRect().x(),
                        boundingRect().y()+140);

	QPointF localPoint4(boundingRect().x()+boundingRect().width(),
                        boundingRect().y()+40);

	QPointF localPoint5(boundingRect().x()+boundingRect().width(),
                        boundingRect().y()+140);

	// And then map to scene coordinates
	auto scenePoint1 = mapToScene(localPoint1);
	auto scenePoint2 = mapToScene(localPoint2);
	auto scenePoint3 = mapToScene(localPoint3);
	auto scenePoint4 = mapToScene(localPoint4);
	auto scenePoint5 = mapToScene(localPoint5);

	dots.push_back(std::pair<int, int>(scenePoint1.x(), scenePoint1.y()));
	dots.push_back(std::pair<int, int>(scenePoint2.x(), scenePoint2.y()));
	dots.push_back(std::pair<int, int>(scenePoint3.x(), scenePoint3.y()));
	dots.push_back(std::pair<int, int>(scenePoint4.x(), scenePoint4.y()));
	dots.push_back(std::pair<int, int>(scenePoint5.x(), scenePoint5.y()));

	return dots;
}

QRectF Decoder::boundingRect() const {
	return QRectF(0, 0, 150, 180);
}

void Decoder::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
//	LogicGate::paint(painter, option, widget);

	// Setting color for drawing lines
	painter->setPen(penForLines);

	// Input lines
	voltageDependedDrawLine(QLineF(0, 30, 10, 30), painter, 0);
	voltageDependedDrawLine(QLineF(0, 50, 10, 50), painter, 1);
	voltageDependedDrawLine(QLineF(0, 70, 10, 70), painter, 2);
	voltageDependedDrawLine(QLineF(0, 90, 10, 90), painter, 3);

	// Output lines
	voltageDependedDrawLine(QLineF(140, 30, 150, 30), painter, 4);
	voltageDependedDrawLine(QLineF(140, 50, 150, 50), painter, 5);
	voltageDependedDrawLine(QLineF(140, 70, 150, 70), painter, 6);
	voltageDependedDrawLine(QLineF(140, 90, 150, 90), painter, 7);
	voltageDependedDrawLine(QLineF(140, 110, 150, 110), painter, 8);
	voltageDependedDrawLine(QLineF(140, 130, 150, 130), painter, 9);
	voltageDependedDrawLine(QLineF(140, 150, 150, 150), painter, 10);

	// Body
	QRectF rect(10, 5, 130, 170);
	painter->drawRect(rect);

	// Connection points
	painter->setPen(penForDots);
	QPointF in1(1, 30);
	QPointF in2(1, 50);
	QPointF in3(1, 70);
	QPointF in4(1, 90);
	QPointF out1(149, 30);
	QPointF out2(149, 50);
	QPointF out3(149, 70);
	QPointF out4(149, 90);
	QPointF out5(149, 110);
	QPointF out6(149, 130);
	QPointF out7(149, 150);

	painter->drawPoint(in1);
	painter->drawPoint(in2);
	painter->drawPoint(in3);
	painter->drawPoint(in4);
	painter->drawPoint(out1);
	painter->drawPoint(out2);
	painter->drawPoint(out3);
	painter->drawPoint(out4);
	painter->drawPoint(out5);
	painter->drawPoint(out6);
	painter->drawPoint(out7);

	// Letters
	painter->setFont(QFont("Times", 20, QFont::Thin));
	painter->drawText(in1 + QPointF(15, 5), "I3");
	painter->drawText(in2 + QPointF(15, 5), "I2");
	painter->drawText(in3 + QPointF(15, 5), "I1");
	painter->drawText(in4 + QPointF(15, 5), "I0");
	painter->drawText(out1 - QPointF(23, 0), "a");
	painter->drawText(out2 - QPointF(23, 0), "b");
	painter->drawText(out3 - QPointF(23, 0), "c");
	painter->drawText(out4 - QPointF(23, 0), "d");
	painter->drawText(out5 - QPointF(23, 0), "e");
	painter->drawText(out6 - QPointF(23, 0), "f");
	painter->drawText(out7 - QPointF(23, 0), "g");
}

std::vector<std::pair<int, int>> Decoder::connectionPoints(void) const {
	std::vector<std::pair<int, int>> dots;
	dots.reserve(11);

	// Find local coordinates of connection point
	QPointF localPoint1(boundingRect().x(),
						boundingRect().y()+30);

	QPointF localPoint2(boundingRect().x(),
						boundingRect().y()+50);

	QPointF localPoint3(boundingRect().x(),
						boundingRect().y()+70);

	QPointF localPoint4(boundingRect().x(),
						boundingRect().y()+90);

	QPointF localPoint5(boundingRect().x()+boundingRect().width(),
						boundingRect().y()+30);

	QPointF localPoint6(boundingRect().x()+boundingRect().width(),
						boundingRect().y()+50);

	QPointF localPoint7(boundingRect().x()+boundingRect().width(),
						boundingRect().y()+70);

	QPointF localPoint8(boundingRect().x()+boundingRect().width(),
						boundingRect().y()+90);

	QPointF localPoint9(boundingRect().x()+boundingRect().width(),
						boundingRect().y()+110);

	QPointF localPoint10(boundingRect().x()+boundingRect().width(),
						boundingRect().y()+130);

	QPointF localPoint11(boundingRect().x()+boundingRect().width(),
						boundingRect().y()+150);

	// And then map to scene coordinates
	auto scenePoint1 = mapToScene(localPoint1);
	auto scenePoint2 = mapToScene(localPoint2);
	auto scenePoint3 = mapToScene(localPoint3);
	auto scenePoint4 = mapToScene(localPoint4);
	auto scenePoint5 = mapToScene(localPoint5);
	auto scenePoint6 = mapToScene(localPoint6);
	auto scenePoint7 = mapToScene(localPoint7);
	auto scenePoint8 = mapToScene(localPoint8);
	auto scenePoint9 = mapToScene(localPoint9);
	auto scenePoint10 = mapToScene(localPoint10);
	auto scenePoint11 = mapToScene(localPoint11);

	dots.push_back(std::pair<int, int>(scenePoint1.x(), scenePoint1.y()));
	dots.push_back(std::pair<int, int>(scenePoint2.x(), scenePoint2.y()));
	dots.push_back(std::pair<int, int>(scenePoint3.x(), scenePoint3.y()));
	dots.push_back(std::pair<int, int>(scenePoint4.x(), scenePoint4.y()));
	dots.push_back(std::pair<int, int>(scenePoint5.x(), scenePoint5.y()));
	dots.push_back(std::pair<int, int>(scenePoint6.x(), scenePoint6.y()));
	dots.push_back(std::pair<int, int>(scenePoint7.x(), scenePoint7.y()));
	dots.push_back(std::pair<int, int>(scenePoint8.x(), scenePoint8.y()));
	dots.push_back(std::pair<int, int>(scenePoint9.x(), scenePoint9.y()));
	dots.push_back(std::pair<int, int>(scenePoint10.x(), scenePoint10.y()));
	dots.push_back(std::pair<int, int>(scenePoint11.x(), scenePoint11.y()));

	return dots;
}

QRectF LCDDisplay::boundingRect() const {
	return QRectF(0, 0, 150, 180);
}

void LCDDisplay::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
//	LogicGate::paint(painter, option, widget);

	// Setting color for drawing lines
	painter->setPen(penForLines);

	// Input lines
	voltageDependedDrawLine(QLineF(0, 30, 10, 30), painter, 0);
	voltageDependedDrawLine(QLineF(0, 50, 10, 50), painter, 1);
	voltageDependedDrawLine(QLineF(0, 70, 10, 70), painter, 2);
	voltageDependedDrawLine(QLineF(0, 90, 10, 90), painter, 3);
	voltageDependedDrawLine(QLineF(0, 110, 10, 110), painter, 4);
	voltageDependedDrawLine(QLineF(0, 130, 10, 130), painter, 5);
	voltageDependedDrawLine(QLineF(0, 150, 10, 150), painter, 6);

	// Body
	QRectF rect(10, 5, 130, 170);
	painter->drawRect(rect);

	// Drawing digits
	painter->setPen(penForDigit);
	if(getBoolVoltage(_nodes[a]->_v)) {
		painter->drawLine(50, 30, 80, 30);
		update();
	}
	if(getBoolVoltage(_nodes[b]->_v)) {
		painter->drawLine(80, 30, 80, 60);
		update();
	}
	if(getBoolVoltage(_nodes[c]->_v)) {
		painter->drawLine(80, 60, 80, 90);
		update();
	}
	if(getBoolVoltage(_nodes[d]->_v)) {
		painter->drawLine(50, 90, 80, 90);
		update();
	}
	if(getBoolVoltage(_nodes[e]->_v)) {
		painter->drawLine(50, 60, 50, 90);
		update();
	}
	if(getBoolVoltage(_nodes[f]->_v)) {
		painter->drawLine(50, 30, 50, 60);
		update();
	}
	if(getBoolVoltage(_nodes[g]->_v)) {
		painter->drawLine(50, 60, 80, 60);
		update();
	}

	// Connection points
	painter->setPen(penForDots);
	QPointF in1(1, 30);
	QPointF in2(1, 50);
	QPointF in3(1, 70);
	QPointF in4(1, 90);
	QPointF in5(1, 110);
	QPointF in6(1, 130);
	QPointF in7(1, 150);

	painter->drawPoint(in1);
	painter->drawPoint(in2);
	painter->drawPoint(in3);
	painter->drawPoint(in4);
	painter->drawPoint(in5);
	painter->drawPoint(in6);
	painter->drawPoint(in7);

	// Letters
	painter->setFont(QFont("Times", 20, QFont::Thin));
	painter->drawText(in1 + QPointF(15, 5), "a");
	painter->drawText(in2 + QPointF(15, 5), "b");
	painter->drawText(in3 + QPointF(15, 5), "c");
	painter->drawText(in4 + QPointF(15, 5), "d");
	painter->drawText(in5 + QPointF(15, 5), "e");
	painter->drawText(in6 + QPointF(15, 5), "f");
	painter->drawText(in7 + QPointF(15, 5), "g");
}

std::vector<std::pair<int, int>> LCDDisplay::connectionPoints(void) const {
	std::vector<std::pair<int, int>> dots;
	dots.reserve(7);

	// Find local coordinates of connection point
	QPointF localPoint1(boundingRect().x(),
						boundingRect().y()+30);

	QPointF localPoint2(boundingRect().x(),
						boundingRect().y()+50);

	QPointF localPoint3(boundingRect().x(),
						boundingRect().y()+70);

	QPointF localPoint4(boundingRect().x(),
						boundingRect().y()+90);

	QPointF localPoint5(boundingRect().x(),
						boundingRect().y()+110);

	QPointF localPoint6(boundingRect().x(),
						boundingRect().y()+130);

	QPointF localPoint7(boundingRect().x(),
						boundingRect().y()+150);

	// And then map to scene coordinates
	auto scenePoint1 = mapToScene(localPoint1);
	auto scenePoint2 = mapToScene(localPoint2);
	auto scenePoint3 = mapToScene(localPoint3);
	auto scenePoint4 = mapToScene(localPoint4);
	auto scenePoint5 = mapToScene(localPoint5);
	auto scenePoint6 = mapToScene(localPoint6);
	auto scenePoint7 = mapToScene(localPoint7);

	dots.push_back(std::pair<int, int>(scenePoint1.x(), scenePoint1.y()));
	dots.push_back(std::pair<int, int>(scenePoint2.x(), scenePoint2.y()));
	dots.push_back(std::pair<int, int>(scenePoint3.x(), scenePoint3.y()));
	dots.push_back(std::pair<int, int>(scenePoint4.x(), scenePoint4.y()));
	dots.push_back(std::pair<int, int>(scenePoint5.x(), scenePoint5.y()));
	dots.push_back(std::pair<int, int>(scenePoint6.x(), scenePoint6.y()));
	dots.push_back(std::pair<int, int>(scenePoint7.x(), scenePoint7.y()));

	return dots;
}

#endif

std::string NOTGate::toString() const {
    std::stringstream str;
    str << name() << std::endl;

    str << std::fixed << std::setprecision(2);
    str << "in: " << _nodes[0]->_v << " V" << std::endl;
    str << "out: " << _nodes[1]->_v << " V" << std::endl;
    return str.str();
}


Decoder::Decoder()
    :LogicGate("Decoder" + std::to_string(_counter+1))
{}

std::string Decoder::toString() const {
	// TODO David
	return Component::toString();
}

Decoder::~Decoder() {
	disconnect();
}

void Decoder::updateVoltages() const {
	for(unsigned i = a; i <= g; ++i) {
		LogicGate::updateVoltages(_nodes[i]);
	}
}

int Decoder::inputToBinaryInt(double a, double b, double c, double d) const {
	int in = getBoolVoltage(a) * 1000
		   + getBoolVoltage(b) * 100
		   + getBoolVoltage(c) * 10
		   + getBoolVoltage(d);

	return in;
}

void Decoder::decodeOutput(int input) const {
	switch (input) {
		case 0:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 5;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 5;
				_nodes[e]->_v = 5;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 0;
		break;
		case 1:
				_nodes[a]->_v = 0;
				_nodes[b]->_v = 5;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 0;
				_nodes[e]->_v = 0;
				_nodes[f]->_v = 0;
				_nodes[g]->_v = 0;
		break;
		case 10:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 5;
				_nodes[c]->_v = 0;
				_nodes[d]->_v = 5;
				_nodes[e]->_v = 5;
				_nodes[f]->_v = 0;
				_nodes[g]->_v = 5;
		break;
		case 11:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 5;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 5;
				_nodes[e]->_v = 0;
				_nodes[f]->_v = 0;
				_nodes[g]->_v = 5;
		break;
		case 100:
				_nodes[a]->_v = 0;
				_nodes[b]->_v = 5;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 0;
				_nodes[e]->_v = 0;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 5;
		break;
		case 101:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 0;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 5;
				_nodes[e]->_v = 0;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 5;
		break;
		case 110:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 0;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 5;
				_nodes[e]->_v = 5;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 5;
		break;
		case 111:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 5;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 0;
				_nodes[e]->_v = 0;
				_nodes[f]->_v = 0;
				_nodes[g]->_v = 0;
		break;
		case 1000:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 5;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 5;
				_nodes[e]->_v = 5;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 5;
		break;
		case 1001:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 5;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 0;
				_nodes[e]->_v = 0;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 5;
		break;
		case 1010:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 5;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 0;
				_nodes[e]->_v = 5;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 5;
		break;
		case 1011:
				_nodes[a]->_v = 0;
				_nodes[b]->_v = 0;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 5;
				_nodes[e]->_v = 5;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 5;
		break;
		case 1100:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 0;
				_nodes[c]->_v = 0;
				_nodes[d]->_v = 5;
				_nodes[e]->_v = 5;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 0;
		break;
		case 1101:
				_nodes[a]->_v = 0;
				_nodes[b]->_v = 5;
				_nodes[c]->_v = 5;
				_nodes[d]->_v = 5;
				_nodes[e]->_v = 5;
				_nodes[f]->_v = 0;
				_nodes[g]->_v = 5;
		break;
		case 1110:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 0;
				_nodes[c]->_v = 0;
				_nodes[d]->_v = 5;
				_nodes[e]->_v = 5;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 5;
		break;
		case 1111:
				_nodes[a]->_v = 5;
				_nodes[b]->_v = 0;
				_nodes[c]->_v = 0;
				_nodes[d]->_v = 0;
				_nodes[e]->_v = 5;
				_nodes[f]->_v = 5;
				_nodes[g]->_v = 5;
		break;
	}
	updateVoltages();
}

double Decoder::voltage() const {
	int in = inputToBinaryInt(_nodes[I3]->_v, _nodes[I2]->_v, _nodes[I1]->_v, _nodes[I0]->_v);
	decodeOutput(in);
	return 0;
}

void Decoder::disconnect(int x, int y) {
    auto it = Node::find(x, y);
    if (it == Node::_allNodes.end()) return;

    //Remove possible voltage on output nodes, pin from 'a' to 'g'
    if (_nodes.size() == 11) {
        for (unsigned i = a; i <= g; ++i) {
            if ((*it) == _nodes[i])
                (*it)->_v = 0;
        }
    }
    LogicGate::updateVoltages((*it));
    Component::disconnect(x, y);
}

void Decoder::disconnect() {
    if (_nodes.size() == 11) {
        //Remove possible voltage on output node
        for (unsigned i = a; i <= g; ++i) {
            _nodes[i]->_v = 0;
        }
        updateVoltages();
    }
    Component::disconnect();
}

LCDDisplay::LCDDisplay()
    :LogicGate("LCD display" + std::to_string(_counter+1))
{}

LCDDisplay::~LCDDisplay() {
    disconnect();
}

