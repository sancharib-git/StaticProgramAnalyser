#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "AffectsExtractor.h"
#include <unordered_set>
#include <utility>
#include "PKB/PKB.h"
#include "StatementTypes.h"

using namespace std;

bool AffectsExtractor::affectsNTBoolean = false;
bool AffectsExtractor::affectsNTBooleanFound = false;
bool AffectsExtractor::affectsTNBoolean = false;
bool AffectsExtractor::affectsTNBooleanFound = false;
bool AffectsExtractor::affectsTTBoolean = false;
bool AffectsExtractor::affectsTTBooleanFound = false;
bool AffectsExtractor::affectsStarNTBoolean = false;
bool AffectsExtractor::affectsStarNTBooleanFound = false;
bool AffectsExtractor::affectsStarTNBoolean = false;
bool AffectsExtractor::affectsStarTNBooleanFound = false;
bool AffectsExtractor::affectsStarTTBoolean = false;
bool AffectsExtractor::affectsStarTTBooleanFound = false;

vector<pair<int, int>> AffectsExtractor::getAffectsNN(int stmt1, int stmt2) {
    PKB& sharedPkb = sharedPkb.getInstance();
    // stmt types check done in QE
    string stmt1LHSVar = sharedPkb.getAssignLHSVar(stmt1);
    unordered_set<std::string> stmt2RHSVars = sharedPkb.getAssignRHSVars(stmt2);
    vector<pair<int, int>> affectsPairs;

    // short circuit if stmt rhs does not contain stmt1 lhs var
    if (stmt2RHSVars.find(stmt1LHSVar) == stmt2RHSVars.end()) {
        return affectsPairs;
    }

    unordered_set<int>* isVisited = new unordered_set<int>();

    if (getAffectsNNUtil(stmt1, stmt2, stmt1, isVisited)) {
        sharedPkb.addAffects(stmt1, stmt2);
        affectsPairs.push_back(make_pair(stmt1, stmt2));
    }
    delete isVisited;
    return affectsPairs;
}

vector<pair<int, int>> AffectsExtractor::getAffectsNT(int stmt1, StatementTypes stmt2Type) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> affectsPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    unordered_set<int>* stmtAdded = new unordered_set<int>();

    getAffectsNTUtil(make_pair(stmt1, stmt1), affectsPairs, isVisited, stmtAdded);

    affectsNTBoolean = false;
    affectsNTBooleanFound = false;
    delete isVisited;
    delete stmtAdded;
    return affectsPairs;
}

vector<pair<int, int>> AffectsExtractor::getAffectsTN(StatementTypes stmt1Type, int stmt2) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> affectsPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    unordered_set<string> targetVars = sharedPkb.getAssignRHSVars(stmt2);
    if (targetVars.size() == 0) {
        return affectsPairs;
    }
    getAffectsTNUtil(make_pair(stmt2, stmt2), affectsPairs, targetVars, isVisited);

    affectsTNBoolean = false;
    affectsTNBooleanFound = false;
    delete isVisited;
    return affectsPairs;
}

vector<pair<int, int>> AffectsExtractor::getAffectsTT(StatementTypes stmt1Type, StatementTypes stmt2Type) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> affectsPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    unordered_set<int>* listOfAssign = sharedPkb.getAssignList();

    for (const auto assignStmt : *listOfAssign) {
        string targetVar = sharedPkb.getAssignLHSVar(assignStmt);
        unordered_set<int>* stmtAdded = new unordered_set<int>();
        unordered_set<int>* isVisited = new unordered_set<int>();
        getAffectsNTUtil(make_pair(assignStmt, assignStmt), affectsPairs, isVisited, stmtAdded);
        if (affectsPairs.size() > 0 && affectsTTBoolean) {
            affectsTTBoolean = false;
            affectsTTBooleanFound = false;
            break;
        }
        delete isVisited;
        delete stmtAdded;
    }
    return affectsPairs;
}

bool AffectsExtractor::getAffectsNNUtil(int stmt1, int stmt2, int originStmt, std::unordered_set<int>* statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> nextLines = sharedPkb.getNextNT(stmt1, NO_TYPE);
    if (nextLines.size() == 0) {
        return false;
    }
    string targetVar = sharedPkb.getAssignLHSVar(originStmt);
    if (nextLines.size() == 2) {
        if (getAffectsNNUtil2Way(nextLines, stmt2, originStmt, statementsVisited)) {
            return true;
        }
    }
    else {
        if (getAffectsNNUtil1Way(nextLines.front(), stmt2, originStmt, statementsVisited)) {
            return true;
        }
    }
    return false;
}

bool AffectsExtractor::getAffectsNNUtil2Way(vector<pair<int, int>> nextLines, int stmt2, int originStmt,
    std::unordered_set<int>* statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    string targetVar = sharedPkb.getAssignLHSVar(originStmt);

    pair<int, int> firstRoute = nextLines.at(0);
    unordered_set<int>* firstRouteSet(statementsVisited);
    bool isFirstRoute = getAffectsNNUtil1Way(firstRoute, stmt2, originStmt, firstRouteSet);

    pair<int, int> secondRoute = nextLines.at(1);
    unordered_set<int>* secondRouteSet(statementsVisited);
    bool isSecondRoute = getAffectsNNUtil1Way(secondRoute, stmt2, originStmt, secondRouteSet);

    if (isFirstRoute || isSecondRoute) {
        return true;
    }
    else {
        return false;
    }
}

bool AffectsExtractor::getAffectsNNUtil1Way(pair<int, int> route, int stmt2, int originStmt,
    std::unordered_set<int>* statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int nextLine1 = route.first;
    vector<pair<int, int>> nextLines = sharedPkb.getNextNT(route.first, NO_TYPE);
    if (nextLines.size() == 0) {
        return false;
    }
    string targetVar = sharedPkb.getAssignLHSVar(originStmt);
    unordered_set<string> listOfVars{ targetVar };
    if (nextLine1 != originStmt && isModified(nextLine1, listOfVars)) {
        return false;
    }
    int nextLine2 = route.second;

    if (nextLine2 == stmt2) {
        return true;
    }
    if (statementsVisited->find(nextLine2) != statementsVisited->end()) {
        return false;
    }
    statementsVisited->insert(nextLine1);

    return getAffectsNNUtil(nextLine2, stmt2, originStmt, statementsVisited);
}

void AffectsExtractor::addNTAffectsPairs(std::pair<int, int> originalAndTargetNext, vector<pair<int, int>>& affectsPairs, unordered_set<int>* resultsAdded) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int originalStmt = originalAndTargetNext.first;
    int targetNext = originalAndTargetNext.second;
    string targetVar = sharedPkb.getAssignLHSVar(originalStmt);
    if (sharedPkb.getStatementType(targetNext) == ASSIGNMENT) {
        unordered_set<string> rhsVars = sharedPkb.getAssignRHSVars(targetNext);
        if (rhsVars.find(targetVar) != rhsVars.end()) {
            sharedPkb.addAffects(originalStmt, targetNext);
            pair<int, int> pairToAdd = make_pair(originalStmt, targetNext);
            resultsAdded->insert(targetNext);
            if (affectsNTBoolean) {
                affectsNTBooleanFound = true;
            }
            affectsPairs.push_back(pairToAdd);
        }
    }
}

void AffectsExtractor::getAffectsNTLoopFound(std::pair<int, int> originalAndCurrStmt, std::vector<std::pair<int, int>>& affectsPairs,
                           std::unordered_set<int>* statementsVisited, std::unordered_set<int>* resultsAdded) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int originalStmt = originalAndCurrStmt.first;
    int currStmt = originalAndCurrStmt.second;
    string targetVar = sharedPkb.getAssignLHSVar(originalStmt);
    vector<pair<int, int>> nextLines = sharedPkb.getNextNT(currStmt, NO_TYPE);

    for (int j = 0; j < nextLines.size(); j++) {
        pair<int, int> nextPair = nextLines.at(j);
        int targetNext = nextPair.second;
        unordered_set<string> listOfVars{ targetVar };
        addNTAffectsPairs(make_pair(originalStmt, targetNext), affectsPairs, resultsAdded);
        if (isModified(targetNext, listOfVars)) {
            continue;
        }
        if (statementsVisited->find(targetNext) == statementsVisited->end()) {
            getAffectsNTUtil(make_pair(targetNext, originalStmt), affectsPairs, statementsVisited, resultsAdded);
        }
    }
}

void AffectsExtractor::getAffectsNTUtil(pair<int, int> currStmtAndOriginalStmt,
    vector<pair<int, int>>& affectsPairs,
    unordered_set<int>* statementsVisited, unordered_set<int>* resultsAdded) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int currStmt = currStmtAndOriginalStmt.first;
    int originalStmt = currStmtAndOriginalStmt.second;
    string targetVar = sharedPkb.getAssignLHSVar(originalStmt);
    vector<pair<int, int>> nextLines = sharedPkb.getNextNT(currStmt, NO_TYPE);
    if (nextLines.size() == 0) {
        return;
    }
    if (statementsVisited->find(currStmt) != statementsVisited->end()) {
        getAffectsNTLoopFound(make_pair(originalStmt, currStmt), affectsPairs, statementsVisited, resultsAdded);
        return;
    }
    statementsVisited->insert(currStmt);
    for (int i = 0; i < nextLines.size(); i++) {
        pair<int, int> currPair = nextLines.at(i);
        int currLine = currPair.first;
        int nextLine = currPair.second;
        if (resultsAdded->find(nextLine) != resultsAdded->end()) {
            continue;
        }

        addNTAffectsPairs(make_pair(originalStmt, nextLine), affectsPairs, resultsAdded);
        if (affectsNTBooleanFound) {
            return;
        }
        unordered_set<string> listOfVars{ targetVar };
        if (isModified(nextLine, listOfVars)) {
            continue;
        }

        getAffectsNTUtil(make_pair(nextLine, originalStmt), affectsPairs, statementsVisited, resultsAdded);
    }
}

void AffectsExtractor::getAffectsTNUtil(pair<int, int> currStmtAndOriginalStmt, vector<pair<int, int>>& affectsPairs, unordered_set<string>& targetVars,
    unordered_set<int>* statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int currStmt = currStmtAndOriginalStmt.first;
    int originalStmt = currStmtAndOriginalStmt.second;
    vector<pair<int, int>> nextLines = sharedPkb.getNextTN(NO_TYPE, currStmt);
    if (nextLines.size() == 0 || targetVars.size() == 0) {
        return;
    }
    statementsVisited->insert(currStmt);
    for (int i = 0; i < nextLines.size(); i++) {
        pair<int, int> currPair = nextLines.at(i);
        int prevLine = currPair.first;
        StatementTypes prevLineType = sharedPkb.getStatementType(prevLine);

        unordered_set<string> fourth(targetVars);
        if (prevLineType == ASSIGNMENT) {
            string lhsVar = sharedPkb.getAssignLHSVar(prevLine);
            if (targetVars.find(lhsVar) != targetVars.end()) {
                pair<int, int> pairToAdd = make_pair(prevLine, originalStmt);
                sharedPkb.addAffects(prevLine, originalStmt);
                affectsPairs.push_back(pairToAdd);
                if (affectsTNBoolean) {
                    affectsTNBooleanFound = true;
                }
                if (statementsVisited->find(prevLine) != statementsVisited->end()) {
                    continue;
                }
                fourth.erase(lhsVar);
                getAffectsTNUtil(make_pair(prevLine, originalStmt), affectsPairs, fourth, statementsVisited);
                statementsVisited->erase(prevLine);
            }
        }
        if (affectsTNBooleanFound) {
            return;
        }
        isReverseModified(prevLine, fourth);
        if (statementsVisited->find(prevLine) != statementsVisited->end()) {
            continue;
        }
        getAffectsTNUtil(make_pair(prevLine, originalStmt), affectsPairs, fourth, statementsVisited);
        statementsVisited->erase(prevLine);
    }
}

bool AffectsExtractor::isModified(int stmtNum, unordered_set<string>& targetVars) {
    PKB& sharedPkb = sharedPkb.getInstance();
    StatementTypes stmtType = sharedPkb.getStatementType(stmtNum);
    if (stmtType == ASSIGNMENT) {
        string lhsVar = sharedPkb.getAssignLHSVar(stmtNum);
        if (targetVars.find(lhsVar) != targetVars.end()) {
            targetVars.erase(lhsVar);
            return true;
        }
    }
    else if (stmtType == READ) {
        string readVar = sharedPkb.getReadVar(stmtNum);
        if (targetVars.find(readVar) != targetVars.end()) {
            targetVars.erase(readVar);
            return true;
        }
    }
    else if (stmtType == CALL) {
        vector<std::pair<int, string>> callPairs = sharedPkb.getCallsSST(stmtNum, NO_TYPE);
        string procName = callPairs.front().second;
        bool isFound = false;
        unordered_set<string> fourth(targetVars);
        for (auto it = fourth.begin(); it != fourth.end(); ++it) {
            if (sharedPkb.getModifiesSSV(stmtNum, *it).size() != 0) {
                targetVars.erase(*it);
                isFound = true;
            }
        }
        return isFound;
    }
    return false;
}

bool AffectsExtractor::isReverseModified(int stmtNum, std::unordered_set<std::string>& targetVars) {
    PKB& sharedPkb = sharedPkb.getInstance();
    StatementTypes stmtType = sharedPkb.getStatementType(stmtNum);
    if (stmtType == READ) {
        string readVar = sharedPkb.getReadVar(stmtNum);
        if (targetVars.find(readVar) != targetVars.end()) {
            targetVars.erase(readVar);
        }
    }
    else if (stmtType == CALL) {
        vector<pair<int, string>> proc = sharedPkb.getCallsSST(stmtNum, NO_TYPE);
        vector<pair<string, string>> modifiedVars = sharedPkb.getModifiesPPT(proc.front().second, NO_TYPE);
        for (int i = 0; i < modifiedVars.size(); i++) {
            string var = modifiedVars.at(i).second;
            if (targetVars.find(var) != targetVars.end()) {
                targetVars.erase(var);
            }
        }
    }
    return true;
}

vector<pair<int, int>> AffectsExtractor::getAffectsStarNN(int stmt1, int stmt2) {
    PKB& sharedPkb = sharedPkb.getInstance();
    string stmt1LHSVar = sharedPkb.getAssignLHSVar(stmt1);
    vector<pair<int, int>> affectsPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();

    unordered_set<string> targetVars = { stmt1LHSVar };
    getAffectsStarNNUtil(make_pair(stmt1, stmt2), stmt1, affectsPairs, isVisited);
    delete isVisited;
    return affectsPairs;
}

vector<pair<int, int>> AffectsExtractor::getAffectsStarNT(int stmt1, StatementTypes stmt2Type) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> affectsPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    string targetVar = sharedPkb.getAssignLHSVar(stmt1);
    unordered_set<string> targetVars = { targetVar };
    getAffectsStarNTUtil(make_pair(stmt1, stmt1), affectsPairs, isVisited);
    delete isVisited;
    affectsStarNTBoolean = false;
    affectsStarNTBooleanFound = false;
    return affectsPairs;
}

vector<pair<int, int>> AffectsExtractor::getAffectsStarTN(StatementTypes stmt1Type, int stmt2) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> affectsPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    unordered_set<string> rhsVars = sharedPkb.getAssignRHSVars(stmt2);
    if (rhsVars.size() == 0) {
        return affectsPairs;
    }
    getAffectsStarTNUtil(make_pair(stmt2, stmt2), rhsVars, affectsPairs, isVisited);
    delete isVisited;
    affectsStarTNBoolean = false;
    affectsStarTNBooleanFound = false;
    return affectsPairs;
}

vector<pair<int, int>> AffectsExtractor::getAffectsStarTT(StatementTypes stmt1Type, StatementTypes stmt2Type) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> affectsPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    unordered_set<int>* listOfAssign = sharedPkb.getAssignList();
    vector<pair<int, int>> allNext = sharedPkb.getNextTT(ASSIGNMENT, NO_TYPE);
    for (const auto assignStmt : *listOfAssign) {
        string targetVar = sharedPkb.getAssignLHSVar(assignStmt);
        unordered_set<int>* isVisited = new unordered_set<int>();
        unordered_set<string> targetVars;
        targetVars.insert(targetVar);
        getAffectsStarNTUtil(make_pair(assignStmt, assignStmt), affectsPairs, isVisited);
        delete isVisited;
        if (affectsStarTTBoolean && affectsPairs.size() > 0) {
            affectsStarTTBoolean = false;
            affectsStarTTBooleanFound = false;
            break;
        }
    }
    return affectsPairs;
}
void AffectsExtractor::getAffectsStarNNUtil(std::pair<int, int> currAndTargetStmt, int originStmt,
    std::vector<std::pair<int, int>>& affectsStarPairs, unordered_set<int>* statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int currStmt = currAndTargetStmt.first;
    int targetStmt = currAndTargetStmt.second;
    vector<pair<int, int>> affectsLines = sharedPkb.getAffectsNT(currStmt, NO_TYPE);
    if (affectsLines.size() == 0) {
        return;
    }
    if (statementsVisited->find(currStmt) != statementsVisited->end()) {
        return;
    }
    statementsVisited->insert(currStmt);
    for (int i = 0; i < affectsLines.size(); i++) {
        pair<int, int> currPair = affectsLines.at(i);
        int currLine = currPair.first;
        int affectedLine = currPair.second;
        if (affectedLine == targetStmt) {
            affectsStarPairs.push_back(make_pair(originStmt, affectedLine));
            sharedPkb.addAffectsStar(originStmt, affectedLine);
            return; // just add
        }
        getAffectsStarNNUtil(make_pair(affectedLine, targetStmt), originStmt, affectsStarPairs, statementsVisited);
    }
}

void AffectsExtractor::getAffectsStarNTUtil(std::pair<int, int> currStmtAndOriginalStmt,
    std::vector<std::pair<int, int>>& affectsStarPairs,
    std::unordered_set<int>* statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int currStmt = currStmtAndOriginalStmt.first;
    int originalStmt = currStmtAndOriginalStmt.second;
    vector<pair<int, int>> affectsLines = sharedPkb.getAffectsNT(currStmt, NO_TYPE);
    if (affectsLines.size() == 0) {
        return;
    }
    if (statementsVisited->find(currStmt) != statementsVisited->end()) {
        return;
    }
    statementsVisited->insert(currStmt);
    for (int i = 0; i < affectsLines.size(); i++) {
        pair<int, int> currPair = affectsLines.at(i);
        int currLine = currPair.first;
        int affectedLine = currPair.second;

        affectsStarPairs.push_back(make_pair(originalStmt, affectedLine));
        sharedPkb.addAffectsStar(originalStmt, affectedLine);
        if (affectsStarNTBoolean) {
            return;
        }
        getAffectsStarNTUtil(make_pair(affectedLine, originalStmt), affectsStarPairs, statementsVisited);
    }
}

void AffectsExtractor::getAffectsStarTNUtil(std::pair<int, int> currStmtAndOriginalStmt,
    std::unordered_set<std::string>& targetVars,
    std::vector<std::pair<int, int>>& affectsStarPairs,
    std::unordered_set<int>* statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int currStmt = currStmtAndOriginalStmt.first;
    int originalStmt = currStmtAndOriginalStmt.second;
    vector<pair<int, int>> affectsLines = sharedPkb.getAffectsTN(NO_TYPE, currStmt);
    if (affectsLines.size() == 0) {
        return;
    }
    if (statementsVisited->find(currStmt) != statementsVisited->end()) {
        return;
    }
    statementsVisited->insert(currStmt);
    for (int i = 0; i < affectsLines.size(); i++) {
        pair<int, int> currPair = affectsLines.at(i);
        int prevLine = currPair.first;
        int currLine = currPair.second;

        affectsStarPairs.push_back(make_pair(prevLine, originalStmt));
        sharedPkb.addAffectsStar(prevLine, originalStmt);

        if (affectsStarTNBoolean) {
            return;
        }
        getAffectsStarTNUtil(make_pair(prevLine, originalStmt), targetVars, affectsStarPairs, statementsVisited);
    }
}

void AffectsExtractor::setAffectsNTBoolean(bool isBooleanClause) {
    affectsNTBoolean = isBooleanClause;
}
void AffectsExtractor::setAffectsTNBoolean(bool isBooleanClause) {
    affectsTNBoolean = isBooleanClause;
}
void AffectsExtractor::setAffectsTTBoolean(bool isBooleanClause) {
    affectsTTBoolean = isBooleanClause;
}
void AffectsExtractor::setAffectsStarNTBoolean(bool isBooleanClause) {
    affectsStarNTBoolean = isBooleanClause;
}
void AffectsExtractor::setAffectsStarTNBoolean(bool isBooleanClause) {
    affectsStarTNBoolean = isBooleanClause;
}
void AffectsExtractor::setAffectsStarTTBoolean(bool isBooleanClause) {
    affectsStarTTBoolean = isBooleanClause;
}