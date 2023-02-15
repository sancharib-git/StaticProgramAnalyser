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

class DesignExtractor {
public:
    DesignExtractor() {};
    bool populateDesignAbstractions();
    bool populateAssignmentSubExpressions();
    static std::vector<std::pair<int, int>> getNextStarNN(int stmt1, int stmt2);
    static std::vector<std::pair<int, int>> getNextStarNT(int stmt1, StatementTypes stmt2Type);
    static std::vector<std::pair<int, int>> getNextStarTN(StatementTypes stmt1Type, int stmt2);
    static std::vector<std::pair<int, int>> getNextStarTT(StatementTypes stmt1Type,
        StatementTypes stmt2Type);
    static void setNextStarNTBoolean(bool isBooleanClause);
    static void setNextStarTNBoolean(bool isBooleanClause);
    static void setNextStarTTBoolean(bool isBooleanClause);

    static std::vector<std::pair<int, int>> getNextBipStarNN(int stmt1, int stmt2);
    static std::vector<std::pair<int, int>> getNextBipStarNT(int stmt1, StatementTypes stmt2Type);
    static std::vector<std::pair<int, int>> getNextBipStarTN(StatementTypes stmt1Type, int stmt2);
    static std::vector<std::pair<int, int>> getNextBipStarTT(StatementTypes stmt1Type,
        StatementTypes stmt2Type);
    static void setNextBipStarNTBoolean(bool isBooleanClause);
    static void setNextBipStarTNBoolean(bool isBooleanClause);
    static void setNextBipStarTTBoolean(bool isBooleanClause);

    static void setAffectsNTBoolean(bool isBooleanClause);
    static void setAffectsTNBoolean(bool isBooleanClause);
    static void setAffectsTTBoolean(bool isBooleanClause);
    static void setAffectsStarNTBoolean(bool isBooleanClause);
    static void setAffectsStarTNBoolean(bool isBooleanClause);
    static void setAffectsStarTTBoolean(bool isBooleanClause);

    static std::vector<std::pair<int, int>> getAffectsNN(int stmt1, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsNT(int stmt1, StatementTypes stmt2Type);
    static std::vector<std::pair<int, int>> getAffectsTN(StatementTypes stmt1Type, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsTT(StatementTypes stmt1Type, StatementTypes stmt2Type);
    static std::vector<std::pair<int, int>> getAffectsStarNN(int stmt1, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsStarNT(int stmt1, StatementTypes stmt2Type);
    static std::vector<std::pair<int, int>> getAffectsStarTN(StatementTypes stmt1Type, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsStarTT(StatementTypes stmt1Type, StatementTypes stmt2Type);

    static bool extractNextBip();
    static std::vector<std::pair<int, int>> getAffectsBipNN(int stmt1, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsBipNT(int stmt1, StatementTypes stmt2Type);
    static std::vector<std::pair<int, int>> getAffectsBipTN(StatementTypes stmt1Type, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsBipTT(StatementTypes stmt1Type, StatementTypes stmt2Type);

};