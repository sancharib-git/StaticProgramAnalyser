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

class NextStarExtractor {
private:
    static bool getNextStarNNUtil(int stmt1, int stmt2, std::unordered_set<int>* statementsVisited);
    static int nextStarNNFirstStatement;
    static void getNextStarNTUtil(std::pair<int, int> currStmtAndTargetStmt, StatementTypes stmt2Type,
        std::vector<std::pair<int, int>>& nextStarPairs, std::unordered_set<int>* statementsVisited);
    static void getNextStarTNUtil(StatementTypes stmt1Type,
        std::pair<int, int> currentStatementAndTargetStatement,
        std::vector<std::pair<int, int>>& nextStarPairs, std::unordered_set<int>* statementsVisited);
    static void getNextStarTTUtil(StatementTypes stmt1Type, StatementTypes stmt2Type,
        std::vector<std::pair<int, int>>& nextStarPairs);
    static void getNextStarTTInnerLoop(
        std::pair<int, int> currentStmtNumAndTargetStmtNum,
        std::pair<StatementTypes, StatementTypes> stmt1TypeAndStmt2Type,
        std::vector<std::pair<int, int>>& nextStarPairs, std::unordered_set<int>* statementsVisited);
    static bool nextStarNTBoolean;
    static bool nextStarNTBooleanFound;
    static bool nextStarTNBoolean;
    static bool nextStarTNBooleanFound;
    static bool nextStarTTBoolean;
    static bool nextStarTTBooleanFound;
public:
    static std::vector<std::pair<int, int>> getNextStarNN(int stmt1, int stmt2);
    static std::vector<std::pair<int, int>> getNextStarNT(int stmt1, StatementTypes stmt2Type);
    static void setNextStarNTBoolean(bool isBooleanClause);
    static void setNextStarTNBoolean(bool isBooleanClause);
    static void setNextStarTTBoolean(bool isBooleanClause);
    static std::vector<std::pair<int, int>> getNextStarTN(StatementTypes stmt1Type, int stmt2);
    static std::vector<std::pair<int, int>> getNextStarTT(StatementTypes stmt1Type,
        StatementTypes stmt2Type);
};