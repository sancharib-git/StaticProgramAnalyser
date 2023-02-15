//
// Created by Toh Yu Ting on 10 Sept 2021
//

#include "DE/Graph.h"
#include "PKB/PKB.h"
#include "catch.hpp"

// Consider adding a test stub?

PKB& stubPKB = PKB::getInstance();

TEST_CASE("Add nodes connected to GraphNode newNode") {
    GraphNode* newNode = new GraphNode(5);

    SECTION("Add one node") {
        newNode->addNode(6);
        LinkedList* nodesConnected = new LinkedList();
        nodesConnected->insertHead(6);
        REQUIRE(newNode->getNodesConnected()->equals(nodesConnected));

        SECTION("Repeated nodes not added") {
            newNode->addNode(6);
            REQUIRE(newNode->getNodesConnected()->equals(nodesConnected));
        }
    }
}

//tests for createFollowsGraph(int totalNumberOfStatements)
TEST_CASE("Generate follows relationship in a graph") {
    stubPKB.clear();
    // add some Follows r/s into PKB
    REQUIRE(stubPKB.addFollows(1, ASSIGNMENT, 2, ASSIGNMENT));
    REQUIRE(stubPKB.addFollows(2, ASSIGNMENT, 3, ASSIGNMENT));
    REQUIRE(stubPKB.addFollows(4, ASSIGNMENT, 5, ASSIGNMENT));

    // start of test
    Graph* newGraph = new Graph();
    newGraph->createFollowsGraph(5);
    unordered_set<int>* nodesInGraph = newGraph->getNodes();
    unordered_set<int>* expectedNodes = new unordered_set<int>();
    expectedNodes->insert(1);
    expectedNodes->insert(2);
    expectedNodes->insert(3);
    expectedNodes->insert(4);
    expectedNodes->insert(5);

    SECTION("Check nodes are added in Graph") {
        REQUIRE(*nodesInGraph == *expectedNodes);
    }

    SECTION("Check each node is connected to right nodes") {
        // getLinkedNodes
        LinkedList* expectedList = new LinkedList();
        expectedList->insertHead(3);
        REQUIRE(newGraph->getLinkedNodes(2)->equals(expectedList));
    }

}