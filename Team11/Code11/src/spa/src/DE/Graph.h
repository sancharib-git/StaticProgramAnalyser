#pragma once
#include <vector>
#include "LinkedList.h"
#include "GraphNode.h"
#include <unordered_set>
#include <unordered_map>

class Graph {
private:
    std::unordered_set<int>* _nodesValues;
    std::vector<GraphNode*>* _vectorForStatements;
    bool addFollowsNode(int statementNumber);
    bool addParentNode(int statementNumber);
    bool addCallsNode(std::string procedure);
public:
    Graph();
    ~Graph();
    void createFollowsGraph(int totalNumberOfStatements);
    void createParentGraph(int totalNumberOfStatements);
    void createCallsGraph();
    std::unordered_set<int>* getNodes();
    LinkedList* getLinkedNodes(int headNode);
};