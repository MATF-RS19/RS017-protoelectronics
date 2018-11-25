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
	std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 1000, n1, n2);
	std::shared_ptr<DCVoltage> u = std::make_shared<DCVoltage>("U1", 5, n2);
	std::shared_ptr<Resistor> r2 = std::make_shared<Resistor>("R2", 2000, n2, n3);
	std::shared_ptr<Wire> w = std::make_shared<Wire>("W1", n3, n1);
	std::shared_ptr<Ground> gnd = std::make_shared<Ground>("gnd", n1);

	//calculete voltage, current...
	std::cout << "I" << r1->name() << ": " << r1->current() << std::endl;
	std::cout << "U" << r1->name() << ": " << r1->voltage() << std::endl;
	std::cout << "U" << r2->name() << ": " << r2->voltage() << std::endl;


	std::cout << "Resistors on node n2:" << std::endl;
	for (auto component : n2->components()) {
		if(component->name()[0] == 'R')
			std::cout << component->name()
				<< ": "
				<< std::static_pointer_cast<Resistor>(component)->resistance()
				<< std::endl;
	}


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
