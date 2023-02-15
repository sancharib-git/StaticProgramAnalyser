#include "GraphNode.h"
#include "LinkedList.h"

GraphNode::GraphNode(int value) {
	_statementKey = value;
	_listOfNodesConnected = new LinkedList();
}

void GraphNode::addNode(int statementNumber) {
	// each graph node should not be connected to the same statement number
	if (_listOfNodesConnected->exist(statementNumber)) {
		return;
	}
	_listOfNodesConnected->insertHead(statementNumber);
}

int GraphNode::getStatementKey() {
	return _statementKey;
}
