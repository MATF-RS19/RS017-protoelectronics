#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__ 1

#include <cassert>

class ASTComponent {
public:
	ASTComponent(double voltage = 0, double current = 0)
		:_voltage(voltage), _current(current)
	{}

	virtual double voltage() const = 0;
	virtual double current() const = 0;
	virtual double resistance() const = 0;

	virtual ~ASTComponent() {

	}

	void setVoltage(double voltage);

	void setCurrent(double current);

	void update(double voltage = 0, double current = 0);

	double getVoltage();

	double getCurrent();

protected:
	double _voltage, _current;
};


class PassiveComponent : public ASTComponent {
public:
	PassiveComponent(double voltage, double current)
		:ASTComponent(voltage, current)
	{}
};


class IOComponent : public ASTComponent {
public:
	IOComponent(double voltage, double current)
		:ASTComponent(voltage, current)
	{}
};


class Connection : public ASTComponent {
public:
	Connection(ASTComponent* elem1, ASTComponent* elem2, double voltage = 0, double current = 0)
		:ASTComponent(voltage, current), _elem1(elem1), _elem2(elem2)
	{}

	~Connection();

protected:
	ASTComponent *_elem1, *_elem2;

//TODO implemet
private:
	Connection(const Connection& c);
	Connection& operator=(const Connection& c);
};


class SerialConnection : public Connection {
public:
	SerialConnection(ASTComponent*  elem1, ASTComponent* elem2, double voltage = 0, double current = 0)
		:Connection(elem1, elem2, voltage, current)
	{}

	double voltage() const override;

	double current() const override;

	double resistance() const override;
};

class ParallelConnection : public Connection {
public:
	ParallelConnection(ASTComponent*  elem1, ASTComponent* elem2, double voltage = 0, double current = 0)
		:Connection(elem1, elem2, voltage, current)
	{}

	double voltage() const override;

	double current() const override;

	double resistance() const override;
};


class Resistor : public PassiveComponent {
public:
	Resistor(double r, double voltage = 0, double current = 0)
		:PassiveComponent(voltage, current), _resistance(r)
	{}

	double voltage() const override;

	double current() const override;

	double resistance() const override;

	double power() const;

private:
	double _resistance;
};


class DCVoltage : public IOComponent {
public:
	DCVoltage(ASTComponent* elem, double voltage = 5)
		:IOComponent(voltage, 0), _elem(elem)
	{
		_elem->setVoltage(voltage);
	}

	~DCVoltage();

	double voltage() const override;

	double current() const override;

	double resistance() const override;

//TODO implement
private:
	ASTComponent* _elem;

	DCVoltage(const DCVoltage& v);
	DCVoltage& operator=(const DCVoltage& v);
};


class DCCurrent : public IOComponent {
public:
	DCCurrent(ASTComponent* elem, double current = 0.01)
		:IOComponent(0, current), _elem(elem)
	{
		_elem->setCurrent(current);
	}

	~DCCurrent();

	double voltage() const override;

	double current() const override;

	double resistance() const override;

//TODO implement
private:
	ASTComponent* _elem;

	DCCurrent(const DCCurrent& v);
	DCCurrent& operator=(const DCCurrent& v);
};

#endif /* __COMPONENTS_H__ */
