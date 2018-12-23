#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include "components.hpp"

class Circuit {
public:
    Circuit();
    ~Circuit();

    std::vector<Component*> components() const;

    //Adds component to circuit
    void addComponent(Component* c);

    //Removes all components from circuit
    void removeComponents();

    //Removes component from circuit by index. Throws exception if index is out of range.
    void removeComponent(unsigned i);

    //TODO
    std::vector<Component*> componentsBetween() const;

    //Access components by index. Throws exception if index is out of range.
    Component* operator[](unsigned i);

    friend std::ostream& operator<<(std::ostream& out, const Circuit& cir);

private:
    std::vector<Component*> _components;
};

#endif /* CIRCUIT_HPP */
