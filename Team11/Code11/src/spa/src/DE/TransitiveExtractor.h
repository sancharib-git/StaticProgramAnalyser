#pragma once
#include "Graph.h"
#include <string>
#include <utility>
#include <unordered_map>
#include <unordered_set>

class TransitiveExtractor {
private:
    bool addFollowsStar();
    bool addParentStar();
    bool addCallsStar();
    bool depthFirstSearch(Graph& inputGraph, std::string designAbstraction);
    bool depthFirstSearchUtil(Graph& inputGraph, std::pair<int, int>& startEndPair, std::unordered_map<int, bool>& isVisit);
    bool processFoundPath(std::string designAbstraction, int currentNodeKey, int innerNodeKey);
    bool setFalse(std::unordered_set<int>& graphNodes, std::unordered_map<int, bool>& visited);
public:
    TransitiveExtractor() {};
    bool addTransitiveRelationship();
};