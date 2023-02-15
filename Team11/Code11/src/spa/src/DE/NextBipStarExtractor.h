#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "Graph.h"
#include "StatementTypes.h"
#include "BinaryTree.h"

class NextBipStarExtractor {
private:
    static bool getNextBipStarNNUtil(int stmt1, int stmt2, std::unordered_set<int>* statementsVisited);
    static int nextBipStarNNFirstStatement;
    static void getNextBipStarNTUtil(std::pair<int, int> currStmtAndTargetStmt, StatementTypes stmt2Type,
        std::vector<std::pair<int, int>>& nextBipStarPairs, std::unordered_set<int>* statementsVisited);
    static void getNextBipStarTNUtil(StatementTypes stmt1Type,
        std::pair<int, int> currentStatementAndTargetStatement,
        std::vector<std::pair<int, int>>& nextStarPairs, std::unordered_set<int>* statementsVisited);
    static void getNextBipStarTTUtil(StatementTypes stmt1Type, StatementTypes stmt2Type,
        std::vector<std::pair<int, int>>& nextStarPairs);
    static void getNextBipStarTTInnerLoop(
        std::pair<int, int> currentStmtNumAndTargetStmtNum,
        std::pair<StatementTypes, StatementTypes> stmt1TypeAndStmt2Type,
        std::vector<std::pair<int, int>>& nextStarPairs, std::unordered_set<int>* statementsVisited);
    static bool nextBipStarNTBoolean;
    static bool nextBipStarNTBooleanFound;
    static bool nextBipStarTNBoolean;
    static bool nextBipStarTNBooleanFound;
    static bool nextBipStarTTBoolean;
    static bool nextBipStarTTBooleanFound;
public:
    static std::vector<std::pair<int, int>> getNextBipStarNN(int stmt1, int stmt2);
    static std::vector<std::pair<int, int>> getNextBipStarNT(int stmt1, StatementTypes stmt2Type);
    static void setNextBipStarNTBoolean(bool isBooleanClause);
    static void setNextBipStarTNBoolean(bool isBooleanClause);
    static void setNextBipStarTTBoolean(bool isBooleanClause);
    static std::vector<std::pair<int, int>> getNextBipStarTN(StatementTypes stmt1Type, int stmt2);
    static std::vector<std::pair<int, int>> getNextBipStarTT(StatementTypes stmt1Type,
        StatementTypes stmt2Type);
};