#ifndef LOG_COMPONENTS
#define LOG_COMPONENTS 
#include "components.hpp"

class LogicGate : public Component{
public:
    LogicGate(const std::string &name);
    // From given voltage returns their logical value
    static bool getBoolVoltage(double v);

    virtual void connect(int x1,int y1, int x2,int y2, int x3, int y3) = 0;
    double voltage() const override;
    double power() const override{
        return 0;
    }
	double current() const override{
        return 0;
    }
#ifdef QTPAINT
    QRectF boundingRect() const override;
#endif
};

class ANDGate : public LogicGate, public Counter<ANDGate>{
public:
    ANDGate();

    std::string componentType() const override { return "and"; }
    

    void connect(int x1,int y1, int x2,int y2, int x3, int y3) override;
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class ORGate : public LogicGate, public Counter<ORGate>{
public:
    ORGate();

    std::string componentType() const override { return "or"; }

    void connect(int x1,int y1, int x2,int y2, int x3, int y3) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class XORGate : public LogicGate, public Counter<XORGate>{
public:
    XORGate();

    std::string componentType() const override { return "xor"; }

    void connect(int x1,int y1, int x2,int y2, int x3, int y3) override;
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NANDGate : public LogicGate, public Counter<NANDGate>{
public:
    NANDGate();

    std::string componentType() const override { return "nand"; }
    
    void connect(int x1,int y1, int x2,int y2, int x3, int y3) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NORGate : public LogicGate, public Counter<NORGate>{
public:
    NORGate();
    
    std::string componentType() const override { return "nor"; }

    void connect(int x1,int y1, int x2,int y2, int x3, int y3) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NXORGate : public LogicGate, public Counter<NXORGate>{
public:
    NXORGate();
    
    std::string componentType() const override { return "nxor"; }

    void connect(int x1,int y1, int x2,int y2, int x3, int y3) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};

class NOTGate: public LogicGate, public Counter<NOTGate> {
public:
    NOTGate();

    std::string componentType() const override { return "not"; }

    void connect(int x1,int y1, int x2,int y2, int x3, int y3) override;

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif
};

#endif /* ifndef LOG_COMPONENTS */
