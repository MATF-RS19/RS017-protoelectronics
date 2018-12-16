#ifndef LOG_COMPONENTS
#define LOG_COMPONENTS 
#include "components.hpp"

class LogicGate : public Component{
public:
    LogicGate(const std::string &name);
    ~LogicGate();
    // From given voltage returns their logical value
    static bool getBoolVoltage(double v);

    virtual void connect(int x1,int y1, int x2,int y2, int x3, int y3) = 0;
    double voltage() const override;
    double power() const override{
        return 0;
    };
	double current() const override{
        return 0;
    };
};
class ANDGate : public LogicGate, public Counter<ANDGate>{
public:
    ANDGate();


    void connect(int x1,int y1, int x2,int y2, int x3, int y3);
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};
class ORGate : public LogicGate, public Counter<ORGate>{
public:
    ORGate();

    void connect(int x1,int y1, int x2,int y2, int x3, int y3);

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};
class XORGate : public LogicGate, public Counter<XORGate>{
public:
    XORGate();


    void connect(int x1,int y1, int x2,int y2, int x3, int y3);
#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};
class NANDGate : public LogicGate, public Counter<NANDGate>{
public:
    NANDGate();

    void connect(int x1,int y1, int x2,int y2, int x3, int y3);

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};
class NORGate : public LogicGate, public Counter<NORGate>{
public:
    NORGate();

    void connect(int x1,int y1, int x2,int y2, int x3, int y3);

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};
class NXORGate : public LogicGate, public Counter<NXORGate>{
public:
    NXORGate();

    void connect(int x1,int y1, int x2,int y2, int x3, int y3);

#ifdef QTPAINT
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
#endif

};
#endif /* ifndef LOG_COMPONENTS */
