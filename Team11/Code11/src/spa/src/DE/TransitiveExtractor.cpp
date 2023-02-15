#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "TransitiveExtractor.h"
#include "Graph.h"
#include <unordered_map>
#include <utility>
#include "PKB/PKB.h"
#include "PKB/CallsStore.h"
#include "StatementTypes.h"

using namespace std;

bool TransitiveExtractor::addTransitiveRelationship() {
    return addFollowsStar() && addParentStar() && addCallsStar();
}

bool TransitiveExtractor::addFollowsStar() {
    Graph* followsGraph = new Graph();
    PKB& sharedPkb = sharedPkb.getInstance();
    int getTotalStatements = sharedPkb.getTotalNumberOfStatements();
    followsGraph->createFollowsGraph(getTotalStatements);

    // followsGraph built, now need to walk through in order to add the respective pairs
    // DFS, to walk and get all the values
    bool isDfsSuccess = depthFirstSearch(*followsGraph, "Follows");
    delete followsGraph;
    return true;
}

bool TransitiveExtractor::addParentStar() {
    Graph* parentGraph = new Graph();
    PKB& sharedPkb = sharedPkb.getInstance();
    int getTotalStatements = sharedPkb.getTotalNumberOfStatements();
    parentGraph->createParentGraph(getTotalStatements);

    // followsGraph built, now need to walk through in order to add the respective pairs
    // DFS, to walk and get all the values
    bool isDfsSuccess = depthFirstSearch(*parentGraph, "Parent");
    delete parentGraph;
    return true;
}

bool TransitiveExtractor::addCallsStar() {
    // 1) create graph for Calls
    Graph* callsGraph = new Graph();
    PKB& sharedPkb = sharedPkb.getInstance();
    callsGraph->createCallsGraph();

    // callsGraph build, walk through to add respective pairs DFS, walk and get all values
    bool isDfsSuccess = depthFirstSearch(*callsGraph, "Calls");
    delete callsGraph;
    return true;
}

bool TransitiveExtractor::depthFirstSearch(Graph& inputGraph, string designAbstraction) {
    // get all nodes that are in the graph
    unordered_set<int>* graphNodes = inputGraph.getNodes();
    unordered_map<int, bool>* visited = new unordered_map<int, bool>();
    // set up unordered_map to check if a node is visited
    for (const auto& statementKey : *graphNodes) {
        visited->insert({ statementKey, false });
    }
    if (graphNodes->size() == 0) {
        return false;
    }
    for (const auto& elem : *graphNodes) {
        int currentNodeKey = elem;
        for (const auto& inner : *graphNodes) {
            int innerNodeKey = inner;
            if (innerNodeKey == currentNodeKey) {
                continue;
            }
            pair<int, int> startEndPair = { currentNodeKey, innerNodeKey };
            bool isPathFound = depthFirstSearchUtil(inputGraph, startEndPair, *visited);
            if (isPathFound) {
                processFoundPath(designAbstraction, currentNodeKey, innerNodeKey);
            }
            setFalse(*graphNodes, *visited);
        }
    }

    return true;
}

bool TransitiveExtractor::processFoundPath(std::string designAbstraction, int currentNodeKey, int innerNodeKey) {
    PKB& sharedPkb = sharedPkb.getInstance();
    StatementTypes statementOneType = sharedPkb.getStatementType(currentNodeKey);
    StatementTypes statementTwoType = sharedPkb.getStatementType(innerNodeKey);
    if (designAbstraction == "Follows") {
        sharedPkb.addFollowsStar(currentNodeKey, statementOneType, innerNodeKey, statementTwoType);
    }
    else if (designAbstraction == "Parent") {
        sharedPkb.addParentStar(currentNodeKey, statementOneType, innerNodeKey, statementTwoType);
    }
    else if (designAbstraction == "Calls") {
        string callsProc = sharedPkb.getProcName(currentNodeKey);
        string calledProc = sharedPkb.getProcName(innerNodeKey);
        sharedPkb.addCallsStar(callsProc, calledProc);
    }
    return true;
}

bool TransitiveExtractor::setFalse(unordered_set<int>& graphNodes, unordered_map<int, bool>& visited) {
    // set up unordered_map to check if a node is visited
    for (const auto& statementKey : graphNodes) {
        visited.at(statementKey) = false;
    }
    return true;
}

bool TransitiveExtractor::depthFirstSearchUtil(Graph& inputGraph, pair<int, int>& startEndPair, unordered_map<int, bool>& isVisit) {
    int start = startEndPair.first;
    int end = startEndPair.second;
    if (start == end) {
        return true;
    }

    auto startFind = isVisit.find(start);
    if ((startFind->second)) {
        return false;
    }
    else {
        startFind->second = true;
    }

    // get all connected nodes to start node
    LinkedList* connectedNodes = inputGraph.getLinkedNodes(start);
    int sizeOfList = connectedNodes->getSize();

    // iterate through all neighbours of start node
    // 1. get size of the list, then loop through the list
    for (int j = 0; j < sizeOfList; j++) {
        // 2. get value, then put it in recursion
        int nextNodeValue = connectedNodes->getElementAtIndex(j);
        pair<int, int> startEndPair = { nextNodeValue, end };
        bool canReach = depthFirstSearchUtil(inputGraph, startEndPair, isVisit);
        if (canReach) {
            return true;
        }
    }
    return false;
}