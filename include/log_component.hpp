#ifndef LOG_COMPONENTS
#define LOG_COMPONENTS 
#include "components.hpp"

class LogicGate : public Component{
public:
    LogicGate(const std::string &name);
    // From given voltage returns their logical value
    static bool getBoolVoltage(double v);

    double voltage() const override;
    double power() const override{
        return 0;
    }
	double current() const override{
        return 0;
    }

#ifdef QTPAINT
	std::vector<std::pair<int, int>> connectionPoints(void) const override;
#endif

protected:
	std::string toString() const override;

#ifdef QTPAINT
    QRectF boundingRect() const override;

    void voltageDependedSetPen(QPainter* painter, unsigned id);
    void voltageDependedDrawLine(QLineF line, QPainter* painter, unsigned id);
#endif
};

class ANDGate : public LogicGate, public Counter<ANDGate>{
public:
    ANDGate();

    std::string componentType() const override { return "and"; }
    
    void connect(const std::vector<std::pair<int, int>>& connPts) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class ORGate : public LogicGate, public Counter<ORGate>{
public:
    ORGate();

    std::string componentType() const override { return "or"; }

    void connect(const std::vector<std::pair<int, int>>& connPts) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class XORGate : public LogicGate, public Counter<XORGate>{
public:
    XORGate();

    std::string componentType() const override { return "xor"; }

    void connect(const std::vector<std::pair<int, int>>& connPts) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NANDGate : public LogicGate, public Counter<NANDGate>{
public:
    NANDGate();

    std::string componentType() const override { return "nand"; }
    
    void connect(const std::vector<std::pair<int, int>>& connPts) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NORGate : public LogicGate, public Counter<NORGate>{
public:
    NORGate();
    
    std::string componentType() const override { return "nor"; }

    void connect(const std::vector<std::pair<int, int>>& connPts) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NXORGate : public LogicGate, public Counter<NXORGate>{
public:
    NXORGate();
    
    std::string componentType() const override { return "nxor"; }

    void connect(const std::vector<std::pair<int, int>>& connPts) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NOTGate: public LogicGate, public Counter<NOTGate> {
public:
    NOTGate();

    std::string componentType() const override { return "not"; }

    void connect(const std::vector<std::pair<int, int>>& connPts) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    std::vector<std::pair<int, int>> connectionPoints(void) const override;
#endif

protected:
	std::string toString() const override;
};

#endif /* ifndef LOG_COMPONENTS */
