#include "components.hpp"

void ASTComponent::update(double voltage, double current) {
	setVoltage(voltage);
	setCurrent(current);
}

double ASTComponent::getVoltage() {
	return _voltage;
}

double ASTComponent::getCurrent() {
	return _current;
}

void ASTComponent::setVoltage(double voltage) {
	_voltage = voltage;
}

void ASTComponent::setCurrent(double current) {
	_current = current;
}


Connection::~Connection() {
	delete _elem1;
	delete _elem2;
}


double SerialConnection::voltage() const {
	double r1 = _elem1->resistance();
	double r2 = _elem2->resistance();

	if (_voltage) {
		_elem1->setVoltage(_voltage * r1 / (r1 + r2));
		_elem2->setVoltage(_voltage * r2 / (r1 + r2));
	}
	else {
		_elem1->setVoltage(_current * r1);
		_elem2->setVoltage(_current * r2);
	}

	return _elem1->voltage() + _elem2->voltage();
}

double SerialConnection::current() const {
	if (_current) {
		_elem1->setCurrent(_current);
		_elem2->setCurrent(_current);
	}
	else {
		double current = _voltage / this->resistance();
		_elem1->setCurrent(current);
		_elem2->setCurrent(current);
	}

	double i1 = _elem1->current();
	double i2 = _elem2->current();

	assert(i1 == i2);
	return i1;
}

double SerialConnection::resistance() const {
	return _elem1->resistance() + _elem2->resistance();
}


double ParallelConnection::voltage() const {
	if (_voltage) {
		_elem1->setVoltage(_voltage);
		_elem2->setVoltage(_voltage);
	}
	else {
		double r1 = _elem1->resistance();
		double r2 = _elem2->resistance();

		_elem1->setVoltage(_current * (r1 + r2) / r1);
		_elem2->setVoltage(_current * (r1 + r2) / r2);
	}

	double v1 = _elem1->voltage();
	double v2 = _elem2->voltage();

	assert(v1 == v2);
	return v1;
}

double ParallelConnection::current() const {
	double r1 = _elem1->resistance();
	double r2 = _elem2->resistance();

	if(_current) {
		_elem1->setCurrent(_current * r2 / (r1 + r2));
		_elem2->setCurrent(_current * r1 / (r1 + r2));
	}
	else {
		_elem1->setCurrent(_voltage / r1);
		_elem2->setCurrent(_voltage / r2);
	}

	return _elem1->current() + _elem2->current();
}

double ParallelConnection::resistance() const {
	double r1 = _elem1->resistance();
	double r2 = _elem2->resistance();
	return r1 * r2 / (r1 + r2);
}


double Resistor::voltage() const {
	return _voltage ? _voltage : _current * _resistance;
}

double Resistor::current() const {
	return _current ? _current : _voltage / _resistance;
}

double Resistor::resistance() const {
	return _resistance;
}

double Resistor::power() const {
	return _voltage * _current;
}


DCVoltage::~DCVoltage() {
	delete _elem;
}

double DCVoltage::voltage() const {
	return _elem->voltage();
}

double DCVoltage::current() const {
	double i = _elem->current();
	_elem->setCurrent(i);
	return i;
}

double DCVoltage::resistance() const {
	return _elem->resistance();
}



DCCurrent::~DCCurrent() {
	delete _elem;
}

double DCCurrent::voltage() const {
	double u = _elem->voltage();
	_elem->setVoltage(u);
	return u;
}

double DCCurrent::current() const {
	return _elem->current();
}

double DCCurrent::resistance() const {
	return _elem->resistance();
}

