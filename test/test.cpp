#include "catch2/catch.hpp"

#include "components.hpp"

SCENARIO("add component", "[add]"){
    GIVEN("empty circut"){

        WHEN("resistor added") {
            Resistor* r = new Resistor(1000);

            THEN("Resistor is unconnected"){
                REQUIRE(r->nodes().size() == 0);
                REQUIRE(Node::_allNodes.size() == 0);
            }

            delete r;
        }

        WHEN("resistor added with 1000 Ohm") {
            Resistor* r = new Resistor(1000);

            THEN("Resistance is 1000 Ohm"){
                REQUIRE(r->resistance() == 1000);
            }

            delete r;
        }

        WHEN("Two resistors and ground added") {
            Resistor* r1 = new Resistor(1000);
            Resistor* r2 = new Resistor(500);
            Ground* g = new Ground();

            THEN("Resistor couter is 2 and Ground is 1"){
                REQUIRE(Resistor::counter() == 2);
                REQUIRE(Ground::counter() == 1);
            }

            delete r1;
            delete r2;
            delete g;
        }

        /*
        WHEN("resistor added with negative resistence") {

            THEN("Throws Exeption"){
                REQUIRE_THROWS(Resistor* r = new Resistor(-5));
            }
        }

        WHEN("resistor added with 0 resistence") {

            THEN("Throws Exeption"){
                REQUIRE_THROWS(Resistor* r = new Resistor(-5));
            }
        }
        */

        WHEN("DCVoltage added with 5 V") {
            DCVoltage* e = new DCVoltage(5);

            THEN("Voltage is 5"){
                REQUIRE(e->voltage() == 5);
            }

            delete e;
        }

        WHEN("Ground added") {
            Ground* gnd = new Ground();

            THEN("Ground is unconnected"){
                REQUIRE(gnd->nodes().size() == 0);
                REQUIRE(Node::_allNodes.size() == 0);
            }

            delete gnd;
        }
    }
}

SCENARIO("connect component", "[connect]"){
   GIVEN("One resistor") {
        Resistor *r = new Resistor(1000);

        WHEN("Connect one lead to node (x,y)"){
            r->addNode(1, 5);

            THEN("Compoment have one node with (x,y)") {
                REQUIRE(r->nodes().size() == 1);
                REQUIRE(r->nodes()[0]->x() == 1);
                REQUIRE(r->nodes()[0]->y() == 5);
            }

            THEN("Component node have one component"){
                REQUIRE(r->nodes()[0]->components()[0]->name() == r->name());
            }

            THEN("Static Node _allNodes have one node with (x,y)"){
                REQUIRE(Node::_allNodes.size() == 1);
                REQUIRE((*Node::_allNodes.begin())->x() == 1);
                REQUIRE((*Node::_allNodes.begin())->y() == 5);
            }

            THEN("Node point to that component"){
                REQUIRE((*Node::_allNodes.begin())->components()[0] == r);
            }

            THEN("Static Node::_allNodes[0] == Compoment nodes[0]"){
                REQUIRE((*Node::_allNodes.begin()) == r->nodes()[0]);
            }
        }

        WHEN("Connect two leads to nodes (x1,y1) and (x2,y2)"){
            r->addNode(1, 5);
            r->addNode(10, 8);

            THEN("Compoment have two nodes with (x1,y1) and (x2,y2)") {
                REQUIRE(r->nodes().size() == 2);
                REQUIRE(r->nodes()[0]->x() == 1);
                REQUIRE(r->nodes()[0]->y() == 5);
                REQUIRE(r->nodes()[1]->x() == 10);
                REQUIRE(r->nodes()[1]->y() == 8);
            }

            THEN("first and second node have one component"){
                REQUIRE(r->nodes()[0]->components().size() == 1);
                REQUIRE(r->nodes()[1]->components().size() == 1);
            }

            THEN("Static Node _allNodes have two nodes"){
                REQUIRE(Node::_allNodes.size() == 2);
            }

            THEN("All nodes point only to that component"){
                REQUIRE((*Node::_allNodes.begin())->components().size() == 1);
                REQUIRE((*Node::_allNodes.begin())->components()[0] == r);

                REQUIRE((*Node::_allNodes.rbegin())->components().size() == 1);
                REQUIRE((*Node::_allNodes.rbegin())->components()[0] == r);
            }

            THEN("Static Node::_allNodes[0] == Compoment nodes[0]"){
                REQUIRE((*Node::_allNodes.begin()) == r->nodes()[0]);
            }
        }
/*
        WHEN("Connect 3 leads to nodes"){
            r->addNode(1, 5);
            r->addNode(10, 8);
            THEN("Throw exeption"){
                REQUIRE_THROWS(r->addNode(5,3));
            }
        }
*/
        delete r;
   }


   GIVEN("One resistor connected to node1 and node2") {
        Resistor* r = new Resistor(1000);
        r->addNode(1, 2);
        r->addNode(3, 4);

        WHEN("Delete resistor") {
            delete r;

            THEN("Nodes are deleted"){
                REQUIRE(Node::_allNodes.size() == 0);
           }
       }
   }

    GIVEN("One resistor connected to node1 and node2") {
        Resistor* r1 = new Resistor(1000);
        r1->addNode(1, 2);
        r1->addNode(3, 4);

        WHEN("Connect new resistor to node3 and node4") {
            Resistor* r2 = new Resistor(500);
            r2->addNode(5, 6);
            r2->addNode(7, 8);

            THEN("There are 4 nodes"){
                REQUIRE(Node::_allNodes.size() == 4);
            }
        }
        delete r1;
    }
}
