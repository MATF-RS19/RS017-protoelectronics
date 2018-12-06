#include <iostream>
#include <vector>
#include <memory>
#include "components.hpp"
//#include "logical_components.hpp"

int main() {
	//add components
	std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>(1000);
	std::shared_ptr<DCVoltage> u = std::make_shared<DCVoltage>(5);
	std::shared_ptr<Resistor> r2 = std::make_shared<Resistor>(2000);
	//std::shared_ptr<Wire> w = std::make_shared<Wire>(2, 4, 2, 3);
	std::shared_ptr<Ground> gnd = std::make_shared<Ground>();

    //connect to nodes
    r1->addNodes(2, 3, 1, 4);
    u->addNode(1, 4);
    r2->addNodes(1, 4, 2, 4);
    gnd->addNode(2, 3);

	//reconnect resistor 1 from (2,3) to (2,4)
	r1->reconnect(2,3,2,4);

	std::cout << "----------Nodes----------" << std::endl;
	for (auto const& e : Node::_allNodes) {
		std::cout << *e << std::endl;
	}

	std::cout << "--------Components--------" << std::endl;
	std::cout << *r1 << std::endl;
	std::cout << *r2 << std::endl;
	std::cout << *u << std::endl;
	std::cout << *gnd << std::endl;

	std::cout << std::endl << "-----Resistors on node n2:-----" << std::endl;
	auto resistorsN2 = Node::components('R', 1, 4);
	for (auto const& component : resistorsN2) {
		std::cout << component.lock()->name() << ": ";
		std::cout << std::static_pointer_cast<Resistor>(component.lock())->resistance() << std::endl;
	}

	std::cout << "Number of nodes: " << Node::counter() << std::endl;
	std::cout << "Number of resistors: " << Resistor::counter() << std::endl;
	std::cout << "Number of voltage sources: " << DCVoltage::counter() << std::endl;

    /*Logic
	double in_1 = 5.0;
	double in_2 = 1;
	std::shared_ptr<InputComponent> in1 =  std::make_shared<InputComponent>(in_1);
	std::shared_ptr<InputComponent> in2 =  std::make_shared<InputComponent>(in_2);
	std::shared_ptr<ANDComponent> and2 = std::make_shared<ANDComponent>(in2, in2);
	std::shared_ptr<ORComponent> or1 = std::make_shared<ORComponent>(and1,in1);
	XORComponent xor1 = XORComponent(or1, in1);
	NXORComponent nxor1(std::make_shared<XORComponent>(xor1), 2.0);
	std::cout << "Result: " << nxor1.voltage() << std::endl;
	*/

	return 0;
}
