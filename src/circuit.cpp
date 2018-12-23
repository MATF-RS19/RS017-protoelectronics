#include "circuit.hpp"
#include <iostream>
#include <stdexcept>

std::ostream& operator<<(std::ostream& out, const Circuit& cir) {
    for (const auto& c : cir._components) {
        out << *c << std::endl;
    }
    return out;
}


Circuit::Circuit() {

}

Circuit::~Circuit() {
    removeComponents();
}


void Circuit::addComponent(Component* c) {
    _components.push_back(c);
}

void Circuit::removeComponents() {
    for (auto c : _components) {
        delete c;
    }
}

std::vector<Component*> Circuit::components() const {
    return _components;
}

size_t Circuit::size() const {
    return _components.size();
}

std::vector<Component*> Circuit::componentsBetween() const {
    //TODO
    return {};
}

Component* Circuit::operator[](unsigned i) {
    if (i >= _components.size()) {
        throw std::runtime_error("Error: Index " + std::to_string(i) + " out of range " );
    }
    return _components[i];
}

void Circuit::removeComponent(unsigned i) {
    delete (*this)[i];
    auto it = _components.begin() + i;
    _components.erase(it);
}
