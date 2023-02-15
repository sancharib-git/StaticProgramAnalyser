//
// Created by Xuan Zhi on 6/11/21.
//

#ifndef AUTOTESTER_AFFECTSBIPEXTRACTOR_H
#define AUTOTESTER_AFFECTSBIPEXTRACTOR_H
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <utility>
#include "StatementTypes.h"

class AffectsBipExtractor {
private:
    static void getAffectsBipNNUtil(std::pair<int, int> currAndTargetStmt, int originStmt, std::vector<std::pair<int, int>>& affectsPairs, std::unordered_set<int>* statementsVisited);
    static bool isReverseModifiedBip(int stmtNum, std::unordered_set<std::string>& targetVars);
    static void addNTBipAffectsPairs(std::pair<int, int> originalAndTargetNext, std::vector<std::pair<int, int>>& affectsPairs, std::unordered_set<int>* resultsAdded);
    static bool isModifiedBip(int stmtNum, std::unordered_set<std::string>& targetVars);
    static void getAffectsBipNTUtil(std::pair<int, int> currStmtAndOriginalStmt, std::vector<std::pair<int, int>>& affectsPairs,
                                 std::unordered_set<int>* statementsVisited, std::unordered_set<int>* resultsAdded);
    static void getAffectsBipTNUtil(std::pair<int, int> currStmtAndOriginalStmt, std::vector<std::pair<int, int>>& affectsPairs, std::unordered_set<std::string>& targetVars, std::unordered_set<int>* statementsVisited);


public:
    static std::vector<std::pair<int, int>> getAffectsBipNN(int stmt1, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsBipNT(int stmt1, StatementTypes stmt2Type);
    static std::vector<std::pair<int, int>> getAffectsBipTN(StatementTypes stmt1Type, int stmt2);
    static std::vector<std::pair<int, int>> getAffectsBipTT(StatementTypes stmt1Type, StatementTypes stmt2Type);
};


#endif //AUTOTESTER_AFFECTSBIPEXTRACTOR_H
