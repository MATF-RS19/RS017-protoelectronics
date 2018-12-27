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

std::vector<std::shared_ptr<Node>> Circuit::differentNodes() {
    std::vector<std::shared_ptr<Node>> diff;
    std::vector<std::shared_ptr<Node>> same;

    for (const auto& n : Node::_allNodes) {
        //if not in same nodes
        if (same.size() == 0 || std::find(same.cbegin(), same.cend(), n) == same.cend()) {
            //and if it isn't ground
            if (n->components("ground").size() != 0) {
                continue;
            }
            //push node in different nodes
            diff.push_back(n);
            //recursively add his neighbors
            for(const auto& adjNode : n->connectedNodes()) {
                same.push_back(adjNode);
            }
        }
    }

    return diff;
}

Eigen::MatrixXd Circuit::makeGMatrix(const std::vector<std::shared_ptr<Node>> &diffNodes) {
    int numOfNodes = diffNodes.size();
    Eigen::MatrixXd Gmatrix(numOfNodes, numOfNodes);

    for (int i = 0; i < numOfNodes; ++i) {
        auto resistorsOnNodeI = diffNodes[i]->components("resistor");
        for (int j = 0; j < numOfNodes; ++j) {
            if (i == j) {
                for(const auto& r : resistorsOnNodeI) {
                    Gmatrix(i, j) += 1/((Resistor*)r)->resistance();
                }
            } else {
                for(const auto& r : resistorsOnNodeI) {
                    //TODO
                    if (r->isConnectedTo( diffNodes[j]->x(), diffNodes[j]->y() )) {
                        Gmatrix(i, j) -= 1/((Resistor*)r)->resistance();
                    }
                }
            }
        }
    }

    return Gmatrix;
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
