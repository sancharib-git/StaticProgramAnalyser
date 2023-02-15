#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <utility>
#include "StatementTypes.h"

class AffectsExtractor {
private:
    static bool affectsNTBoolean;
    static bool affectsNTBooleanFound;
    static bool affectsTNBoolean;
    static bool affectsTNBooleanFound;
    static bool affectsTTBoolean;
    static bool affectsTTBooleanFound;
    static bool affectsStarNTBoolean;
    static bool affectsStarNTBooleanFound;
    static bool affectsStarTNBoolean;
    static bool affectsStarTNBooleanFound;
    static bool affectsStarTTBoolean;
    static bool affectsStarTTBooleanFound;

    static bool getAffectsNNUtil(int stmt1, int stmt2, int originStmt, std::unordered_set<int>* statementsVisited);
    static bool getAffectsNNUtil2Way(std::vector<std::pair<int, int>> nextLines, int stmt2, int originStmt, std::unordered_set<int>* statementsVisited);
    static bool getAffectsNNUtil1Way(std::pair<int, int> route, int stmt2, int originStmt, std::unordered_set<int>* statementsVisited);
    static void getAffectsNTUtil(std::pair<int, int> currStmtAndOriginalStmt, std::vector<std::pair<int, int>>& affectsPairs,
        std::unordered_set<int>* statementsVisited, std::unordered_set<int>* resultsAdded);
    static void getAffectsNTLoopFound(std::pair<int, int> originalAndCurrStmt, std::vector<std::pair<int, int>>& affectsPairs,
                                      std::unordered_set<int>* statementsVisited, std::unordered_set<int>* resultsAdded);
    static void addNTAffectsPairs(std::pair<int, int> originalAndTargetNext, std::vector<std::pair<int, int>>& affectsPairs, std::unordered_set<int>* resultsAdded);
    static void getAffectsTNUtil(std::pair<int, int> currStmtAndOriginalStmt, std::vector<std::pair<int, int>>& affectsPairs, std::unordered_set<std::string>& targetVars, std::unordered_set<int>* statementsVisited);

    static void getAffectsStarNNUtil(std::pair<int, int> targetAndCurrStmt, int originStmt, std::vector<std::pair<int, int>>& affectsPairs, std::unordered_set<int>* statementsVisited);
    static void getAffectsStarNTUtil(std::pair<int, int> currStmtAndOriginalStmt, std::vector<std::pair<int, int>>& affectsStarPairs,
                                     std::unordered_set<int>* statementsVisited);
    static void getAffectsStarTNUtil(std::pair<int, int> currStmtAndOriginalStmt, std::unordered_set<std::string>& targetVars, std::vector<std::pair<int, int>>& affectsPairs, std::unordered_set<int>* statementsVisited);

    static bool isModified(int stmtNum, std::unordered_set<std::string>& targetVars);
    static bool isReverseModified(int stmtNum, std::unordered_set<std::string>& targetVars);
public:
    static std::vector<std::pair<int, int>> getAffectsNN(int stmt1, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsNT(int stmt1, StatementTypes stmt2Type);
    static std::vector<std::pair<int, int>> getAffectsTN(StatementTypes stmt1Type, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsTT(StatementTypes stmt1Type, StatementTypes stmt2Type);

    static std::vector<std::pair<int, int>> getAffectsStarNN(int stmt1, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsStarNT(int stmt1, StatementTypes stmt2Type);
    static std::vector<std::pair<int, int>> getAffectsStarTN(StatementTypes stmt1Type, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsStarTT(StatementTypes stmt1Type, StatementTypes stmt2Type);
    static void setAffectsNTBoolean(bool isBooleanClause);
    static void setAffectsTNBoolean(bool isBooleanClause);
    static void setAffectsTTBoolean(bool isBooleanClause);
    static void setAffectsStarNTBoolean(bool isBooleanClause);
    static void setAffectsStarTNBoolean(bool isBooleanClause);
    static void setAffectsStarTTBoolean(bool isBooleanClause);
};