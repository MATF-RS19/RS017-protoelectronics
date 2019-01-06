#ifndef LOG_COMPONENTS_HPP
#define LOG_COMPONENTS_HPP
#include "components.hpp"

class LogicGate : public Component{
public:
    LogicGate(const std::string &name);

    virtual ~LogicGate() override;

    // From given voltage returns their logical value
    static bool getBoolVoltage(double v);

    double power() const override{
        return 0;
    }
	double current() const override{
        return 0;
    }

    void disconnect(int x, int y) override;

    void disconnect() override;
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

    double voltage() const override;
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class ORGate : public LogicGate, public Counter<ORGate>{
public:
    ORGate();

    std::string componentType() const override { return "or"; }

    void connect(const std::vector<std::pair<int, int>>& connPts) override;

    double voltage() const override;
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class XORGate : public LogicGate, public Counter<XORGate>{
public:
    XORGate();

    std::string componentType() const override { return "xor"; }

    void connect(const std::vector<std::pair<int, int>>& connPts) override;

    double voltage() const override;
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NANDGate : public LogicGate, public Counter<NANDGate>{
public:
    NANDGate();

    std::string componentType() const override { return "nand"; }
    
    void connect(const std::vector<std::pair<int, int>>& connPts) override;

    double voltage() const override;
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NORGate : public LogicGate, public Counter<NORGate>{
public:
    NORGate();
    
    std::string componentType() const override { return "nor"; }

    void connect(const std::vector<std::pair<int, int>>& connPts) override;

    double voltage() const override;
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NXORGate : public LogicGate, public Counter<NXORGate>{
public:
    NXORGate();
    
    std::string componentType() const override { return "nxor"; }

    void connect(const std::vector<std::pair<int, int>>& connPts) override;

    double voltage() const override;
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NOTGate: public LogicGate, public Counter<NOTGate> {
public:
    NOTGate();

    ~NOTGate() override;

    std::string componentType() const override { return "not"; }

    void connect(const std::vector<std::pair<int, int>>& connPts) override;

    double voltage() const override;

    void disconnect(int x, int y) override;

    void disconnect() override;
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    std::vector<std::pair<int, int>> connectionPoints(void) const override;
#endif

private:
	std::string toString() const override;
};

#endif /* ifndef LOG_COMPONENTS_HPP */
