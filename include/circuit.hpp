#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include "components.hpp"
#include <Eigen/Dense>

class Circuit {
public:
    Circuit();
    ~Circuit();

    //Returns all components in circuit
    std::vector<Component*> components() const;

    //Returns number of components in circuit
    size_t size() const;

    //Adds component to circuit
    void addComponent(Component* c);

    //Removes all components from circuit
    void removeComponents();

    //Removes component from circuit by index. Throws exception if index is out of range.
    void removeComponent(unsigned i);

    //TODO
    std::vector<Component*> componentsBetween() const;

    //Returns all different nodes in circuit
    std::vector<std::shared_ptr<Node>> differentNodes();

    Eigen::MatrixXd makeGMatrix(const std::vector<std::shared_ptr<Node>> &diffNodes);

    //Access components by index. Throws exception if index is out of range.
    Component* operator[](unsigned i);

    friend std::ostream& operator<<(std::ostream& out, const Circuit& cir);

private:
    std::vector<Component*> _components;
};

#endif /* CIRCUIT_HPP */
