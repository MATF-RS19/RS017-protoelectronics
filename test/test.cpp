#include "catch2/catch.hpp"

#include "components.hpp"
#include "log_component.hpp"

#define EPS 1e-5

SCENARIO("add component", "[add]"){
    GIVEN("Empty circuit"){

        WHEN("Resistor added") {
            Resistor r(1000);

            THEN("Resistor is unconnected"){
                REQUIRE(r.nodes().size() == 0);
                REQUIRE(Node::size() == 0);
            }
        }

        WHEN("Resistor added with 1000 Ohm") {
            double resistance = 1000;
            Resistor r(resistance);

            THEN("Resistance is 1000 Ohm"){
                REQUIRE(r.resistance() == Approx(resistance).epsilon(EPS));
            }
        }

        WHEN("Two resistors and ground added") {
            Resistor r1(1000);
            Resistor r2(500);
            Ground g;

            THEN("Resistor counter is 2 and Ground is 1"){
                REQUIRE(Resistor::counter() == 2);
                REQUIRE(Ground::counter() == 1);
            }
        }

        /*
        WHEN("resistor added with negative resistance") {

            THEN("Throws Exception"){
                REQUIRE_THROWS(Resistor* r = new Resistor(-5));
            }
        }

        WHEN("resistor added with 0 resistance") {

            THEN("Throws Exception"){
                REQUIRE_THROWS(Resistor* r = new Resistor(-5));
            }
        }
        */

        WHEN("DCVoltage added with 5 V") {
            double voltage = 5;
            DCVoltage e(voltage);

            THEN("Voltage is 5"){
                REQUIRE(e.voltage() == Approx(voltage).epsilon(EPS));
            }
        }

        WHEN("Ground added") {
            Ground gnd;

            THEN("Ground is unconnected"){
                REQUIRE(gnd.nodes().size() == 0);
                REQUIRE(Node::size() == 0);
            }
        }
    }
}

SCENARIO("connect one component", "[connect one]"){
   GIVEN("One resistor") {
        Resistor r(1000);

        WHEN("Connect one lead to node (x,y)"){
            r.addNode(1, 5);

            THEN("Component have one node with (x,y)") {
                REQUIRE(r.nodes().size() == 1);
                REQUIRE(r.nodes()[0]->x() == 1);
                REQUIRE(r.nodes()[0]->y() == 5);
            }

            THEN("Component node have one component (self)"){
                REQUIRE(r.nodes()[0]->components()[0] == &r);
            }

            THEN("Collection of all nodes have one node with (x,y)"){
                REQUIRE(Node::size() == 1);
                REQUIRE(Node::find(1, 5) != Node::_allNodes.end());
            }

            THEN("Node point to that component"){
                REQUIRE((*Node::find(1, 5))->components()[0] == &r);
            }

            THEN("Node from all nodes is same as node in component"){
                REQUIRE((*Node::_allNodes.begin()) == r.nodes()[0]);
            }
        }

        WHEN("Connect two leads to nodes (x1,y1) and (x2,y2)"){
            int x1 = 1, y1 = 5;
            int x2 = 10, y2 = 8;
            r.addNode(x1, y1);
            r.addNode(x2, y2);

            THEN("Component have two nodes with (x1,y1) and (x2,y2)") {
                REQUIRE(r.nodes().size() == 2);
                REQUIRE(r.nodes()[0]->x() == x1);
                REQUIRE(r.nodes()[0]->y() == y1);
                REQUIRE(r.nodes()[1]->x() == x2);
                REQUIRE(r.nodes()[1]->y() == y2);
            }

            THEN("Component nodes both have one component (self)"){
                REQUIRE(r.nodes()[0]->components().size() == 1);
                REQUIRE(r.nodes()[1]->components().size() == 1);
                REQUIRE(r.nodes()[0]->components()[0] == &r);
                REQUIRE(r.nodes()[1]->components()[0] == &r);
            }

            THEN("Collection of all nodes have two nodes"){
                REQUIRE(Node::size() == 2);
            }

            THEN("All nodes point only to that component"){
                REQUIRE((*Node::find(x1, y1))->components().size() == 1);
                REQUIRE((*Node::find(x1, y1))->components()[0] == &r);

                REQUIRE((*Node::find(x2, y2))->components().size() == 1);
                REQUIRE((*Node::find(x2, y2))->components()[0] == &r);
            }

            THEN("Nodes from all nodes is same as nodes in component"){
                REQUIRE(*Node::find(x1, y1) == r.nodes()[0]);
                REQUIRE(*Node::find(x2, y2) == r.nodes()[1]);
        }
    }
/*
        WHEN("Connect 3 leads to nodes"){
            r.addNode(1, 5);
            r.addNode(10, 8);
            THEN("Throw exeption"){
                REQUIRE_THROWS(r.addNode(5,3));
            }
        }
*/
   }


    GIVEN("One resistor connected to node1 and node2") {
        Resistor* r = new Resistor(1000);
        r->addNode(1, 2);
        r->addNode(3, 4);

        WHEN("Delete resistor") {
            delete r;

            THEN("Nodes are deleted"){
                REQUIRE(Node::size() == 0);
            }
        }
        if (!r) delete r;
    }

}

SCENARIO("connect 2th component", "[connect 2th]"){
    GIVEN("One resistor connected to node1 and node2") {
        Resistor r1(1000);
        int x1 = 5, y1 = 1;
        int x2 = 4, y2 = 10;
        r1.addNode(x1, y1);
        r1.addNode(x2, y2);

        WHEN("Connect new resistor to node3 and node4") {
            int x3 = 6, y3 = 1;
            int x4 = 8, y4 = 10;
            Resistor r2(500);
            r2.addNode(x3, y3);
            r2.addNode(x4, y4);

            THEN("There are 4 nodes"){
                REQUIRE(Node::size() == 4);
            }
        }

        WHEN("Connect new resistor to node2 and node3") {
            int x3 = 6, y3 = 1;
            Resistor r2(500);
            r2.addNode(x2, y2);
            r2.addNode(x3, y3);

            THEN("There are 3 nodes"){
                REQUIRE(Node::size() == 3);
            }

        }
    }
}



SCENARIO("logic component connect", "[logic connect]"){
    GIVEN("AND gate") {
        ANDGate and1;

        WHEN("Unconnected") {
            THEN("All nodes is empty") {
                REQUIRE(Node::size() == 0);
            }
        }

        int x1 = 4, y1 = 4;
        int x2 = 7, y2 = 5;
        int x3 = 8, y3 = 6;
        WHEN("Connect input to (x1, y1), (x2, y2) and output to (x3, y3)") {
            THEN("There's 3 nodes") {
                and1.connect(x1, y1, x2, y2, x3, y3);
                REQUIRE(Node::size() == 3);
            }
        }
    }

}

SCENARIO("logic components circuit", "[logic circuit]"){
    GIVEN("Connected 2 AND, 1 OR gates") {
        //Logic gates
        ANDGate and1;
        ANDGate and2;
        ORGate or1;

        //Input
        DCVoltage v1(5);
        DCVoltage v2(1);

        //nodes coordinates
        int x1 = 4, y1 = 4;
        int x2 = 7, y2 = 5;
        int x3 = 8, y3 = 6;
        int x4 = 10, y4 = 8;
        int x5 = 5, y5 = 11;

        //Ovako radi, ali treba da radi i kada se povezu logicka kola
        //pa tek onda doda napon na neke cvorove
        //v1.addNode(x1, y1);
        //v2.addNode(x2, y2);

        //connections
        and1.connect(x1, y1, x2, y2, x3, y3);
        or1.connect(x1, y1, x2, y2, x4, y4);
        and2.connect(x3, y3, x4, y4, x5, y5);

        WHEN("Voltage connected on input") {
            v1.addNode(x1, y1);
            v2.addNode(x2, y2);

            THEN("Output of AND1 is 0") {
                REQUIRE(and1.voltage() == Approx(0).epsilon(EPS));
            }

            THEN("Output of OR1 is 5V") {
                REQUIRE(or1.voltage() == Approx(5).epsilon(EPS));
            }

            THEN("Output of AND2 is 0") {
                REQUIRE(and2.voltage() == Approx(0).epsilon(EPS));
            }

            //Now check nodes
            THEN("Output node of AND1 is 0") {
                REQUIRE((*Node::find(x3, y3))->_v == Approx(0).epsilon(EPS));
            }

            THEN("Output of OR is 5V") {
                REQUIRE((*Node::find(x4, y4))->_v == Approx(5).epsilon(EPS));
            }

            THEN("Output of AND2 is 0") {
                REQUIRE((*Node::find(x5, y5))->_v == Approx(0).epsilon(EPS));
            }
        }
    }
}
