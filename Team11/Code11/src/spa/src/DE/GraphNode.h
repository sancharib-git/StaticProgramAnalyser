#pragma once
#include "LinkedList.h"

class GraphNode {
private:
    int _statementKey;
    LinkedList* _listOfNodesConnected;
public:
    GraphNode(int value);
    int getValue() {
        return _statementKey;
    };
    LinkedList* getNodesConnected() {
        return _listOfNodesConnected;
    };
    void addNode(int statementNumber);
    int getStatementKey();
    friend class Graph;
};