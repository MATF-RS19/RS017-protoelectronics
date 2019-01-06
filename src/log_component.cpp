#include "log_component.hpp"

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

    //Remove possible voltage on output node
    if (_nodes.size() == 4 && ((*it) == _nodes[Q] || *(it) == _nodes[Qc])) (*it)->_v = 0;
    updateVoltages((*it));
    Component::disconnect(x, y);
}

void JKFlipFlop::disconnect() {
    if (_nodes.size() == 4) {
        //Remove possible voltage on output node
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

	// Setting color for drawing lines
	painter->setPen(penForLines);

	// Input lines
    painter->drawLine(0, 40, 20, 40);
    painter->drawLine(0, 90, 20, 90);
    painter->drawLine(0, 140, 20, 140);

	// Output liness
    painter->drawLine(140, 40, 160, 40);
    painter->drawLine(140, 140, 160, 140);

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
    //TODO verovatno je promenjeno jer sam tacke pomerio za 0.5
    //J, K, Q i Qc. Clock je ostao isti

	// Find local coordinates of connection point
	QPointF localPoint1(boundingRect().x(),
						boundingRect().y()+boundingRect().height()/4);

	QPointF localPoint2(boundingRect().x(),
						boundingRect().y()+boundingRect().height()/2);

	QPointF localPoint3(boundingRect().x(),
						boundingRect().y()+boundingRect().height()*3/4);

	QPointF localPoint4(boundingRect().x()+boundingRect().width(),
						boundingRect().y()+boundingRect().height()/4);

	QPointF localPoint5(boundingRect().x()+boundingRect().width(),
						boundingRect().y()+boundingRect().height()*3/4);

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

void Decoder::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //TODO
}

std::vector<std::pair<int, int>> Decoder::connectionPoints(void) const {
    //TODO
}

void LCDDisplay::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //TODO
}

std::vector<std::pair<int, int>> LCDDisplay::connectionPoints(void) const {
    //TODO
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


LCDDisplay::LCDDisplay()
    :LogicGate("LCD display" + std::to_string(_counter+1))
{}

LCDDisplay::~LCDDisplay() {
    disconnect();
}

