#include <iostream>
#include "Graph.h"
#include <utility>
#include "PKB/PKB.h"
#include "PKB/CallsStore.h"
#include <unordered_set>
#include <unordered_map>

using namespace std;

Graph::Graph() {
	_nodesValues = new unordered_set<int>();
	_vectorForStatements = new vector<GraphNode*>();
}

Graph::~Graph() {
	delete _vectorForStatements;
}

void Graph::createFollowsGraph(int totalNumberOfStatements) {
	for (int i = 1; i <= totalNumberOfStatements; i++) {
		addFollowsNode(i);
	}
};

// returns boolean according to whether the statement number is added, not added when statement does not have any follows r/s
bool Graph::addFollowsNode(int statementNumber) {
	PKB& sharedPkb = sharedPkb.getInstance();
	vector<pair<int, int> > followsPairs;
	followsPairs = sharedPkb.getFollowsNT(statementNumber, StatementTypes::NO_TYPE);
	// if vector is empty, there is no Follows r/s, then you just dont add this node
	if (followsPairs.empty()) {
		return false;
	}
	// otherwise, iterate through the pairs, to get what u need, the second element of each pair, and add to linkedlist of the node
	GraphNode* newNode = new GraphNode(statementNumber);
	for (const auto& elem : followsPairs) {
		pair<int, int> obtainedPair = elem;
		int firstStatement = obtainedPair.first;
		int secondStatement = obtainedPair.second;
		newNode->addNode(secondStatement);
		_nodesValues->insert(obtainedPair.first);
		_nodesValues->insert(obtainedPair.second);
		// push to PKB's FollowsStar tables
		StatementTypes statementOneType = sharedPkb.getStatementType(firstStatement);
		StatementTypes statementTwoType = sharedPkb.getStatementType(secondStatement);
		sharedPkb.addFollowsStar(firstStatement, statementOneType, secondStatement, statementTwoType);
	}

	//at the end, add this node to the graph
	_vectorForStatements->push_back(newNode);
	return true;
};

void Graph::createParentGraph(int totalNumberOfStatements) {
	for (int i = 1; i <= totalNumberOfStatements; i++) {
		addParentNode(i);
	}
};

// returns boolean according to whether the statement number is added, not added when statement does not have any follows r/s
bool Graph::addParentNode(int statementNumber) {
	PKB& sharedPkb = sharedPkb.getInstance();
	vector<pair<int, int> > parentPairs;
	parentPairs = sharedPkb.getParentNT(statementNumber, StatementTypes::NO_TYPE);
	// if vector is empty, there is not Follows r/s, then you just dont add this node
	if (parentPairs.empty()) {
		return false;
	}
	// otherwise, iterate through the pairs, to get what u need, the second element of each pair, and add to linkedlist of the node
	GraphNode* newNode = new GraphNode(statementNumber);
	for (const auto& elem : parentPairs) {
		pair<int, int> obtainedPair = elem;
		int firstStatement = obtainedPair.first;
		int secondStatement = obtainedPair.second;
		newNode->addNode(secondStatement);
		_nodesValues->insert(obtainedPair.first);
		_nodesValues->insert(obtainedPair.second);
		// push to PKB's ParentStar tables
		StatementTypes statementOneType = sharedPkb.getStatementType(firstStatement);
		StatementTypes statementTwoType = sharedPkb.getStatementType(secondStatement);
		sharedPkb.addParentStar(firstStatement, statementOneType, secondStatement, statementTwoType);
	}

	//at the end, add this node to the graph
	_vectorForStatements->push_back(newNode);
	return true;
};

void Graph::createCallsGraph() {
	// 1) Obtain the list of procedures in SIMPLE
	PKB& sharedPkb = sharedPkb.getInstance();
	unordered_set<string>* allProcedures = sharedPkb.getProcList();
	// 2) Create table and mirrored table, to represent procedure string and a unique ID

	for (const string& procedure : *allProcedures) {
		addCallsNode(procedure);
	}
}

// returns boolean according to whether the statement number is added, not added when statement does not have any follows r/s
bool Graph::addCallsNode(string procedure) {
	PKB& sharedPkb = sharedPkb.getInstance();
	vector<pair<string, string> > callsPairs;
	callsPairs = sharedPkb.getCallsPPT(procedure, NO_TYPE);
	// if vector is empty, there is not Calls r/s, then you just dont add this node
	if (callsPairs.empty()) {
		return false;
	}
	int procedureID = sharedPkb.getProcDigit(procedure);
	// otherwise, iterate through the pairs, to get what u need, the second element of each pair, and add to linkedlist of the node
	GraphNode* newNode = new GraphNode(procedureID);
	for (const auto& elem : callsPairs) {
		pair<string, string> obtainedPair = elem;
		string firstProcedure = obtainedPair.first;
		string secondProcedure = obtainedPair.second;
		int secondProcedureID = sharedPkb.getProcDigit(secondProcedure);
		newNode->addNode(secondProcedureID);
		_nodesValues->insert(procedureID);
		_nodesValues->insert(secondProcedureID);
		// push to PKB's CallsStar table
		//sharedPkb.addCallsStar(firstProcedure, secondProcedure);
	}

	//at the end, add this node to the graph
	_vectorForStatements->push_back(newNode);
	return true;
};

unordered_set<int>* Graph::getNodes() {
	return _nodesValues;
};

LinkedList* Graph::getLinkedNodes(int headNode) {
	int graphSize = _vectorForStatements->size();
	for (int i = 0; i < graphSize; i++) {
		GraphNode* currNode = (*_vectorForStatements)[i];
		if ((currNode->getValue()) == headNode) {
			return currNode->getNodesConnected();
		}
	}
	return new LinkedList();
};