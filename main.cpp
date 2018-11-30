#include <iostream>
#include <vector>
#include <memory>
#include "components.hpp"
//#include "logical_components.hpp"

int main() {
    //add nodes
	std::shared_ptr<Node> n1 = std::make_shared<Node>(2, 3);
	std::shared_ptr<Node> n2 = std::make_shared<Node>(1, 4);
	std::shared_ptr<Node> n3 = std::make_shared<Node>(2, 4);

	//add components
	std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>(1000, n1, n2);
	std::shared_ptr<DCVoltage> u = std::make_shared<DCVoltage>(5, n2);
	std::shared_ptr<Resistor> r2 = std::make_shared<Resistor>(2000, n2, n3);
	std::shared_ptr<Wire> w = std::make_shared<Wire>(n3, n1);
	std::shared_ptr<Ground> gnd = std::make_shared<Ground>(n1);

	std::vector<std::shared_ptr<Node>> nodes = {n1, n2, n3};

	//calculate voltage, current...
	std::cout << *r1 << std::endl;
	std::cout << *r2 << std::endl;
	std::cout << *u << std::endl;

	for (auto node : nodes) {
		std::cout << "Node" << node->id() << std::endl;
		for (auto component : node->components()) {
			std::cout << component->name() << std::endl;
			if (component->name()[0] == 'W') {
				std::cout << "Connected with: "
					<< std::static_pointer_cast<Wire>(component)->otherNode(node->id())->id()
					<< std::endl;
			}
		}
	}

	Circuit sim(nodes);
	std::cout << "DCVoltages: " << sim.numOfDCVoltages() << std::endl;

	std::cout << std::endl << "Resistors on node n2:" << std::endl;
	auto resistorsN2 = sim.components(sim.nodes()[1], 'R');
	for (auto component : resistorsN2) {
		std::cout << component->name() << ": ";
		std::cout << std::static_pointer_cast<Resistor>(component)->resistance() << std::endl;
	}

	std::cout << r2->name() << std::endl;
	std::cout << r2->id() << std::endl;
	std::cout << r2->counter() << std::endl;

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
