#include "catch2/catch.hpp"

#include "components.hpp"
#include "log_component.hpp"
#include "circuit.hpp"

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

        WHEN("Resistor added with no resistance") {
            Resistor r;

            THEN("Resistance is 1000 Ohm"){
                REQUIRE(r.resistance() == Approx(1000).epsilon(EPS));
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

        WHEN("resistor added with negative resistance") {

            THEN("Throws Exception"){
                REQUIRE_THROWS(new Resistor(-5));
            }
        }

        WHEN("resistor added with 0 resistance") {

            THEN("Throws Exception"){
                REQUIRE_THROWS(new Resistor(0));
            }
        }

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
        int x = 1, y = 5;
        Resistor r(1000);

        WHEN("Connect one lead to node (x,y)"){
            r.addNode(x, y);

            THEN("Component have one node with (x,y)") {
                REQUIRE(r.nodes().size() == 1);
                REQUIRE(r.isConnectedTo(x, y));
            }

            THEN("Component node have one component (self)"){
                REQUIRE((*r.find(x,y))->isConnectedTo(&r));
            }

            THEN("Collection of all nodes have one node with (x,y)"){
                REQUIRE(Node::size() == 1);
                REQUIRE(Node::find(1, 5) != Node::_allNodes.end());
            }

            THEN("Node point to that component"){
                REQUIRE((*Node::find(1, 5))->isConnectedTo(&r));
            }

            THEN("Node from all nodes is same as node in component"){
                REQUIRE((*Node::find(x, y)) == *r.find(x, y));
            }
        }

        WHEN("Connect two leads to nodes (x1,y1) and (x2,y2)"){
            int x1 = 1, y1 = 5;
            int x2 = 10, y2 = 8;
            r.addNode(x1, y1);
            r.addNode(x2, y2);

            THEN("Component have two nodes with (x1,y1) and (x2,y2)") {
                REQUIRE(r.nodes().size() == 2);
                REQUIRE(r.isConnectedTo(x1, y1));
                REQUIRE(r.isConnectedTo(x2, y2));
            }

            THEN("Component nodes both have one component (self)"){
                REQUIRE(r.nodes()[0]->components().size() == 1);
                REQUIRE(r.nodes()[0]->isConnectedTo(&r));
                REQUIRE(r.nodes()[1]->components().size() == 1);
                REQUIRE(r.nodes()[1]->isConnectedTo(&r));
            }

            THEN("Collection of all nodes have two nodes"){
                REQUIRE(Node::size() == 2);
            }

            THEN("All nodes point only to that component"){
                REQUIRE((*Node::find(x1, y1))->components().size() == 1);
                REQUIRE((*Node::find(x1, y1))->isConnectedTo(&r));

                REQUIRE((*Node::find(x2, y2))->components().size() == 1);
                REQUIRE((*Node::find(x2, y2))->isConnectedTo(&r));
            }

            THEN("Nodes from all nodes is same as nodes in component"){
                REQUIRE(*Node::find(x1, y1) == *r.find(x1, y1));
                REQUIRE(*Node::find(x2, y2) == *r.find(x2, y2));
            }
    }

        WHEN("Connect three leads"){
            r.addNode(1, 3);
            r.addNode(2, 2);
            THEN("Throws exception") {
                REQUIRE_THROWS(r.addNode(3, 4));
            }
        }
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

    GIVEN("One resistor connected twice to the same node") {
        Resistor* r = new Resistor(1000);
        r->addNode(1, 2);
        r->addNode(1, 2);

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
        int x3 = 6, y3 = 1;
        int x4 = 8, y4 = 10;
        r1.addNode(x1, y1);
        r1.addNode(x2, y2);

        WHEN("Connect new resistor to node3 and node4") {
            Resistor r2(500);
            r2.addNode(x3, y3);
            r2.addNode(x4, y4);

            THEN("There are 4 nodes"){
                REQUIRE(Node::size() == 4);
            }
        }

        WHEN("Connect new resistor to node2 and node3") {
            Resistor r2(500);
            r2.addNode(x2, y2);
            r2.addNode(x3, y3);

            THEN("There are 3 nodes"){
                REQUIRE(Node::size() == 3);
            }
        }
    }
}

SCENARIO("reconnect one component", "[reconnect one]"){
    GIVEN("2 resistors connected to node1, node2 and node2, node3") {
        Resistor r1(1000);
        Resistor r2(500);
        int x1 = 5, y1 = 1;
        int x2 = 4, y2 = 10;
        int x3 = 6, y3 = 1;
        r1.addNode(x1, y1);
        r1.addNode(x2, y2);
        r2.addNode(x2, y2);
        r2.addNode(x3, y3);

        WHEN("reconnect from node1 to node3") {
            r1.reconnect(x1, y1, x3, y3);

            THEN("R1 connected to node2 and node3") {
                REQUIRE(r1.nodes().size() == 2 );
                REQUIRE(r1.isConnectedTo(x2, y2));
                REQUIRE(r1.isConnectedTo(x3, y3));
            }

            THEN("R2 connected to node2 and node3") {
                REQUIRE(r2.nodes().size() == 2 );
                REQUIRE(r2.isConnectedTo(x2, y2));
                REQUIRE(r2.isConnectedTo(x3, y3));
            }

            THEN("There are 2 nodes n2 and n3") {
                REQUIRE(Node::size() == 2 );
                REQUIRE(Node::find(x2, y2) != Node::_allNodes.end());
                REQUIRE(Node::find(x3, y3) != Node::_allNodes.end());
            }

            THEN("Node2 is connected to R1 and R2") {
                REQUIRE((*Node::find(x2, y2))->isConnectedTo(&r1));
            }

            THEN("Node3 is is connected to R1 and R2") {
                REQUIRE((*Node::find(x3, y3))->isConnectedTo(&r1));
            }
        }
    }
}

SCENARIO("connect two leads to same node", "[connect2one]"){
    GIVEN("unconnected resistor") {
        Resistor r1(1000);
        int x1 = 5, y1 = 1;

        WHEN("connect to the same node twice"){
            r1.addNode(x1, y1);
            r1.addNode(x1, y1);

            THEN("resistor have 2 nodes") {
                REQUIRE(r1.nodes().size() == 2 );
            }

            THEN("resistors nodes are the same node") {
                REQUIRE(r1.nodes()[0] == r1.nodes()[1]);
            }

            THEN("collection of all nodes have one node with (x,y)"){
                REQUIRE(Node::size() == 1);
                REQUIRE(Node::find(x1, y1) != Node::_allNodes.end());
            }

            THEN("connection component->node"){
                REQUIRE(r1.isConnectedTo(x1, y1));
            }

            THEN("connection node->component"){
                REQUIRE((*Node::find(x1, y1))->isConnectedTo(&r1));
            }

            THEN("node from all nodes is same as node in component"){
                REQUIRE((*Node::find(x1, y1)) == *r1.find(x1, y1));
            }
        }
    }
}


SCENARIO("disconnect component from node", "[disconnect]"){
    int x1 = 5, y1 = 1;
    int x2 = 10, y2 = 10;
    Resistor r1(1000);

    GIVEN("resistor connect to one node"){
        r1.addNode(x1, y1);

        WHEN("disconnect from node"){
            r1.disconnect(x1, y1);

            THEN("Resistor is unconnected") {
                REQUIRE(r1.nodes().size() == 0);
            }

            THEN("_allNodes is empty") {
                REQUIRE(Node::size() == 0);
            }
        }
    }

    GIVEN("resistor and DCVoltage connect to one node"){
        DCVoltage e1(5);
        e1.addNode(x1, y1);
        r1.addNode(x1, y1);

        WHEN("disconnect resistor from node"){
            r1.disconnect(x1, y1);

            THEN("Resistor is unconnected") {
                REQUIRE(r1.nodes().size() == 0);
            }

            THEN("DCVoltage is connected") {
                REQUIRE(e1.nodes().size() == 1);
            }

            THEN("_allNodes have one node") {
                REQUIRE(Node::size() == 1);
            }
        }
    }

    GIVEN("resistor connect to two nodes"){
        r1.addNode(x1, y1);
        r1.addNode(x2, y2);

        WHEN("Disconnect one"){
            r1.disconnect(x1, y1);

            THEN("Resistor have one node") {
                REQUIRE(r1.nodes().size() == 1);
                REQUIRE(r1.find(x2, y2) != r1.nodes().end());
            }

            THEN("There is one node") {
                REQUIRE(Node::size() == 1);
                REQUIRE((*Node::find(x2, y2))->isConnectedTo(&r1) == true);
            }
        }
    }

    GIVEN("resistor connect to the same node twice"){
        r1.addNode(x1, y1);
        r1.addNode(x1, y1);

        WHEN("disconnect from node"){
            r1.disconnect(x1, y1);

            THEN("Resistor is unconnected") {
                REQUIRE(r1.nodes().size() == 0);
            }

            THEN("_allNodes is empty") {
                REQUIRE(Node::size() == 0);
            }
        }

        WHEN("disconnect from unconnected node"){
            r1.disconnect(x2, y2);

            THEN("No effect") {
                REQUIRE(r1.nodes().size() == 2);
                REQUIRE(Node::size() == 1);
            }
        }
    }
}

SCENARIO("disconnect component completely", "[disconnectAll]"){
    int x1 = 5, y1 = 1;
    int x2 = 10, y2 = 10;
    Resistor r1(1000);
    Resistor g(500);

    GIVEN("resistor connect to 2 nodes"){
        r1.addNode(x1, y1);
        r1.addNode(x2, y2);

        WHEN("disconnect from all nodes"){
            r1.disconnect();

            THEN("Resistor is unconnected") {
                REQUIRE(r1.nodes().size() == 0);
            }

            THEN("_allNodes is empty") {
                REQUIRE(Node::size() == 0);
            }
        }
    }

    GIVEN("resistor connect to 2 nodes and ground connected to both"){
        r1.addNode(x1, y1);
        r1.addNode(x2, y2);
        g.addNode(x1, y1);
        g.addNode(x2, y2);

        WHEN("disconnect Resistor from all nodes"){
            r1.disconnect();

            THEN("Resistor is unconnected") {
                REQUIRE(r1.nodes().size() == 0);
            }

            THEN("There are two nodes connected to ground") {
                REQUIRE(Node::size() == 2);
                REQUIRE((*Node::find(x1, y1))->isConnectedTo(&g));
                REQUIRE((*Node::find(x2, y2))->isConnectedTo(&g));
            }
        }
    }
}

SCENARIO("connect two leads to same node and then reconnect to another", "[connect2one&reconnect]"){
    GIVEN("resistor connect to the same node twice"){
        Resistor r1(1000);
        int x1 = 5, y1 = 1;
        int x2 = 10, y2 = 10;
        r1.addNode(x1, y1);
        r1.addNode(x1, y1);

        WHEN("reconnect to another node"){
            r1.reconnect(x1, y1, x2, y2);

            THEN("resistor have 2 nodes") {
                REQUIRE(r1.nodes().size() == 2 );
            }

            THEN("resistors nodes are the same node") {
                REQUIRE(r1.nodes()[0] == r1.nodes()[1]);
            }

            THEN("collection of all nodes have one node with (x,y)"){
                REQUIRE(Node::size() == 1);
                REQUIRE(Node::find(x2, y2) != Node::_allNodes.end());
            }

            THEN("connection component->node"){
                REQUIRE(r1.isConnectedTo(x2, y2));
            }

            THEN("connection node->component"){
                REQUIRE((*Node::find(x2, y2))->isConnectedTo(&r1));
            }

            THEN("node from all nodes is same as node in component"){
                REQUIRE((*Node::find(x2, y2)) == *r1.find(x2, y2));
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
                and1.connect(std::vector<std::pair<int, int>>{
                        std::pair<int, int>(x1, y1),
                        std::pair<int, int>(x2, y2),
                        std::pair<int, int>(x3, y3)});
                REQUIRE(Node::size() == 3);
            }
        }
    }

}

SCENARIO("logic components circuit", "[.circuit]"){
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
        and1.connect(std::vector<std::pair<int, int>>{
                    std::pair<int, int>(x1,y1),
                    std::pair<int, int>(x2,y2),
                    std::pair<int, int>(x3,y3)
                    });
        or1.connect(std::vector<std::pair<int, int>>{
                    std::pair<int, int>(x1,y1),
                    std::pair<int, int>(x2,y2),
                    std::pair<int, int>(x4,y4)
                    });
        and2.connect(std::vector<std::pair<int, int>>{
                    std::pair<int, int>(x3,y3),
                    std::pair<int, int>(x4,y4),
                    std::pair<int, int>(x5,y5)
                    });

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



SCENARIO("get components when connected with wire", "[getComponentWire]") {
    GIVEN("4 Resistor and 2 Wire connect") {
        Resistor r1(100), r2(200), r3(300), r4(400), r5(500);
        std::string allComponentsNames= r1.name() + " " + r2.name() + " " + r3.name() + " " + r4.name() + " " + r5.name() ;

        Wire w1, w2, w3, w4;
        int x1 = 1, y1 = 5;
        int x2 = 2, y2 = 6;
        int x3 = 3, y3 = 7;
        int x4 = 4, y4 = 8;
        int x5 = 5, y5 = 9;

        //Node1
        r1.addNode(x1, y1);
        r2.addNode(x1, y1);
        w1.addNode(x1, y1);
        w2.addNode(x1, y1);

        //Node2
        w1.addNode(x2, y2);
        r3.addNode(x2, y2);

        //Node3
        w2.addNode(x3, y3);
        w3.addNode(x3, y3);
        w4.addNode(x3, y3);

        //Node4
        r4.addNode(x4, y4);
        w3.addNode(x4, y4);

        //Node5
        r5.addNode(x5, y5);
        w4.addNode(x5, y5);

        WHEN("get components from node1") {
            auto allComponents = (*Node::find(x1, y1))->components();
            THEN("all components is \"connect\" to node1") {
                REQUIRE(allComponents.size() == 5);
                for (const auto& c : allComponents) {
                    REQUIRE(allComponentsNames.find(c->name()) != std::string::npos );
                }
            }
        }

        WHEN("get components from node2") {
            auto allComponents = (*Node::find(x2, y2))->components();
            THEN("all components is \"connect\" to node2") {
                REQUIRE(allComponents.size() == 5);
                for (const auto& c : allComponents) {
                    REQUIRE(allComponentsNames.find(c->name()) != std::string::npos );
                }
            }
        }

        WHEN("get components from node3") {
            auto allComponents = (*Node::find(x3, y3))->components();
            THEN("all components is \"connect\" to node3") {
                REQUIRE(allComponents.size() == 5);
                for (const auto& c : allComponents) {
                    REQUIRE(allComponentsNames.find(c->name()) != std::string::npos );
                }
            }
        }

        WHEN("get components from node4") {
            auto allComponents = (*Node::find(x4, y4))->components();
            THEN("all components is \"connect\" to node4") {
                REQUIRE(allComponents.size() == 5);
                for (const auto& c : allComponents) {
                    REQUIRE(allComponentsNames.find(c->name()) != std::string::npos );
                }
            }
        }

        WHEN("get components from node5") {
            auto allComponents = (*Node::find(x5, y5))->components();
            THEN("all components is \"connect\" to node5") {
                REQUIRE(allComponents.size() == 5);
                for (const auto& c : allComponents) {
                    REQUIRE(allComponentsNames.find(c->name()) != std::string::npos );
                }
            }
        }
    }
}

SCENARIO("Get components by type when connected with wire", "[getComponentTypeWire]") {
    GIVEN("2 Resistor, 2 DCVoltage and 1 Wire connect") {
        DCVoltage e1(5), e2(10);
        std::string allDCVNames = e1.name() + " " + e2.name();
        Resistor r1(100), r2(200);
        std::string allResistorsNames = r1.name() + " " + r2.name();
        Wire w1;

        int x1 = 1, y1 = 5;
        int x2 = 2, y2 = 6;

        //Node1
        r1.addNode(x1, y1);
        e1.addNode(x1, y1);
        w1.addNode(x1, y1);

        //Node2
        w1.addNode(x2, y2);
        r2.addNode(x2, y2);
        e2.addNode(x2, y2);

        WHEN("get voltages from node1") {
            auto allComponents = (*Node::find(x1, y1))->components("voltage");
            THEN("all DCV is \"connect\" to node1") {
                REQUIRE(allComponents.size() == 2);
                for (const auto& c : allComponents) {
                    REQUIRE(allDCVNames.find(c->name()) != std::string::npos );
                }
            }
        }

        WHEN("get resistors from node1") {
            auto allComponents = (*Node::find(x1, y1))->components("resistor");
            THEN("all resistors is \"connect\" to node1") {
                REQUIRE(allComponents.size() == 2);
                for (const auto& c : allComponents) {
                    REQUIRE(allResistorsNames.find(c->name()) != std::string::npos );
                }
            }
        }

        WHEN("get voltages from node2") {
            auto allComponents = (*Node::find(x2, y2))->components("voltage");
            THEN("all DCV is \"connect\" to node2") {
                REQUIRE(allComponents.size() == 2);
                for (const auto& c : allComponents) {
                    REQUIRE(allDCVNames.find(c->name()) != std::string::npos );
                }
            }
        }

        WHEN("get resistors from node2") {
            auto allComponents = (*Node::find(x2, y2))->components("resistor");
            THEN("all resistors is \"connect\" to node2") {
                REQUIRE(allComponents.size() == 2);
                for (const auto& c : allComponents) {
                    REQUIRE(allResistorsNames.find(c->name()) != std::string::npos );
                }
            }
        }
    }

}

SCENARIO("Circuit", "[circuit]"){
    GIVEN("Empty circuit") {
        Circuit c;

        WHEN("Add component") {
            c.addComponent(new Resistor(10));

            THEN("Component is in circuit") {
                REQUIRE(c[0]->name() == "R1" );
            }
        }

        WHEN("Add 2 components") {
            c.addComponent(new Resistor(10));
            c.addComponent(new DCVoltage(10));

            THEN("Component 1 is in circuit") {
                REQUIRE(c[0]->name() == "R1" );
            }

            THEN("Component 2 is in circuit") {
                REQUIRE(c[1]->name() == "U1" );
            }
        }

        WHEN("Add component then remove") {
            c.addComponent(new Resistor(10));
            c.removeComponent(0);

            THEN("Circuit is empty") {
                REQUIRE(c.size() == 0 );
            }
        }

        WHEN("get component") {
            THEN("Exception is thrown") {
                REQUIRE_THROWS(c[0]);
                REQUIRE_THROWS(c[-5]);
                REQUIRE_THROWS(c[5]);
            }
        }
    }
}



#include <iostream>
#include <Eigen/Dense>

SCENARIO("main", "[main]"){
    Circuit c;
    c.addComponent(new DCVoltage());
    c[0]->addNode(-1, -1);
    c.addComponent(new Wire());
    c[1]->addNode(-1, -1);
    c[1]->addNode(1, 1);
    c.addComponent(new Resistor());
    c[2]->addNode(1, 1);
    c[2]->addNode(2, 2);
    c.addComponent(new Wire());
    c[3]->addNode(2, 2);
    c[3]->addNode(3, 3);
    c.addComponent(new Wire());
    c[4]->addNode(3, 3);
    c[4]->addNode(4, 4);
    c.addComponent(new DCVoltage(10));
    c[5]->addNode(4, 4);
    c.addComponent(new Wire());
    c[6]->addNode(3, 3);
    c[6]->addNode(5, 5);
    c.addComponent(new Resistor());
    c[7]->addNode(5, 5);
    c[7]->addNode(6, 6);
    c.addComponent(new Ground());
    c[8]->addNode(6, 6);


    std::cout << c << std::endl;
    std::cout << "-----------------" << std::endl;
    for (const auto& n : Node::_allNodes) {
        std::cout << *n << std::endl;
    }
    std::cout << "-------connectedNodes---------" << std::endl;
    
    {
        std::vector<std::shared_ptr<Node>> razliciti;
        std::vector<std::shared_ptr<Node>> ponovljeni;

        for (const auto& n : Node::_allNodes) {
            //ako se nije pojavio
            if (ponovljeni.size() == 0 || std::find(ponovljeni.begin(), ponovljeni.end(), n) == ponovljeni.end()) {
                //ako sadrzi ground
                if (n->components("ground").size() != 0) {
                    continue;
                }
                //stavi medj razlicite
                razliciti.push_back(n);
                //obidji njegove susede
                for(const auto& istiSused : n->connectedNodes()) {
                    ponovljeni.push_back(istiSused);
                }
            } 
        }

        /*
        std::cout << "\nrazliciti:" << std::endl;
        for (auto i : razliciti) {
            std::cout << *i << std::endl;
        }
        std::cout << "\nponovljeni:" << std::endl;
        for (auto i : ponovljeni) {
            std::cout << *i << std::endl;
        }
        */

        int numOfNodes = razliciti.size();
        Eigen::MatrixXd Gmatrix(numOfNodes, numOfNodes);

        std::cout << "==================Analiza=======================" << std::endl;
        for (const auto &i : razliciti) {
            std::cout << *i << std::endl;
        }

        for (int i = 0; i < numOfNodes; ++i) {
            for (int j = 0; j < numOfNodes; ++j) {
                //TODO implementirati ovo
                Gmatrix(i, j) = c.componentsBetween(i, j) ;
            }   
        }

        std::cout << Gmatrix << std::endl;
    }
}
