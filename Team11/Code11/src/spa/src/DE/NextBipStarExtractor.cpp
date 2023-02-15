#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "NextBipStarExtractor.h"
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "PKB/PKB.h"
#include "StatementTypes.h"
#include "BinaryTree.h"

using namespace std;

int NextBipStarExtractor::nextBipStarNNFirstStatement = 0;
bool NextBipStarExtractor::nextBipStarNTBoolean = false;
bool NextBipStarExtractor::nextBipStarNTBooleanFound = false;
bool NextBipStarExtractor::nextBipStarTNBoolean = false;
bool NextBipStarExtractor::nextBipStarTNBooleanFound = false;
bool NextBipStarExtractor::nextBipStarTTBoolean = false;
bool NextBipStarExtractor::nextBipStarTTBooleanFound = false;

vector<pair<int, int>> NextBipStarExtractor::getNextBipStarNN(int stmt1, int stmt2) {
    PKB& sharedPkb = sharedPkb.getInstance();
    unordered_set<int>* isVisited = new unordered_set<int>();
    nextBipStarNNFirstStatement = stmt1;
    bool isFound = getNextBipStarNNUtil(stmt1, stmt2, isVisited);
    vector<pair<int, int>> allNextBipStar;
    if (isFound) {
        allNextBipStar.push_back(make_pair(stmt1, stmt2));
    }
    delete isVisited;
    return allNextBipStar;
}

void NextBipStarExtractor::setNextBipStarNTBoolean(bool isBooleanClause) {
    nextBipStarNTBoolean = isBooleanClause;
}

vector<pair<int, int>> NextBipStarExtractor::getNextBipStarNT(int stmt1, StatementTypes stmt2Type) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> nextBipStarPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();

    getNextBipStarNTUtil(make_pair(stmt1, stmt1), stmt2Type, nextBipStarPairs, isVisited);
    delete isVisited;
    nextBipStarNTBoolean = false;
    nextBipStarNTBooleanFound = false;
    return nextBipStarPairs;
}

void NextBipStarExtractor::setNextBipStarTNBoolean(bool isBooleanClause) {
    nextBipStarTNBoolean = isBooleanClause;
}

vector<pair<int, int>> NextBipStarExtractor::getNextBipStarTN(StatementTypes stmt1Type, int stmt2) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> nextBipStarPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    getNextBipStarTNUtil(stmt1Type, make_pair(stmt2, stmt2), nextBipStarPairs, isVisited);
    delete isVisited;
    nextBipStarTNBoolean = false;
    nextBipStarTNBooleanFound = false;
    return nextBipStarPairs;
}

void NextBipStarExtractor::setNextBipStarTTBoolean(bool isBooleanClause) {
    nextBipStarTTBoolean = isBooleanClause;
}

vector<pair<int, int>> NextBipStarExtractor::getNextBipStarTT(StatementTypes stmt1Type, StatementTypes stmt2Type) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> nextBipStarPairs;
    getNextBipStarTTUtil(stmt1Type, stmt2Type, nextBipStarPairs);
    nextBipStarTTBoolean = false;
    nextBipStarTTBooleanFound = false;
    return nextBipStarPairs;
}

bool NextBipStarExtractor::getNextBipStarNNUtil(int stmt1, int stmt2, unordered_set<int>* statementsVisited) {
    statementsVisited->insert(stmt1);
    PKB& sharedPkb = sharedPkb.getInstance();
    // 1) Get all statements in NextBip relationship with stmt1
    vector<pair<int, int>> allNextBip = sharedPkb.getNextBipNT(stmt1, NO_TYPE);
    if (allNextBip.size() == 0) {
        return false;
    }
    for (int i = 0; i < allNextBip.size(); i++) {
        pair<int, int> currentPair = allNextBip[i];
        int next1ProgLine = currentPair.first;
        int next2ProgLine = currentPair.second;
        sharedPkb.addNextBipStar(nextBipStarNNFirstStatement, next2ProgLine);
        if (next2ProgLine == stmt2) {
            return true;
        }
        // before recurse, check if I have visited that node before
        if (statementsVisited->find(next2ProgLine) != statementsVisited->end()) {
            continue;
        }
        bool isFound = getNextBipStarNNUtil(next2ProgLine, stmt2, statementsVisited);
        if (isFound) {
            return true;
        }
    }
    return false;
}

void NextBipStarExtractor::getNextBipStarNTUtil(pair<int, int> currStmtAndTargetStmt, StatementTypes stmt2Type,
    vector <pair<int, int>>& nextBipStarPairs, unordered_set<int>* statementsVisited) {
    int currentStmt = currStmtAndTargetStmt.first;
    int targetStmt = currStmtAndTargetStmt.second;
    PKB& sharedPkb = sharedPkb.getInstance();
    // 1) Get all statements in NextBip relationship with stmt1
    vector<pair<int, int>> allNextBip = sharedPkb.getNextBipNT(currentStmt, NO_TYPE);
    if ((allNextBip.size() == 0)) {
        return;
    }

    statementsVisited->insert(currentStmt);
    for (int i = 0; i < allNextBip.size(); i++) {
        pair<int, int> currentPair = allNextBip[i];
        int next1ProgLine = currentPair.first;
        int next2ProgLine = currentPair.second;
        sharedPkb.addNextBipStar(targetStmt, next2ProgLine);
        StatementTypes next2ProgLineType = sharedPkb.getStatementType(next2ProgLine);
        if ((next2ProgLineType == stmt2Type) || (stmt2Type == NO_TYPE)) {
            pair<int, int> pairToAdd = make_pair(targetStmt, next2ProgLine);
            nextBipStarPairs.push_back(pairToAdd);
            if (nextBipStarNTBoolean) {
                nextBipStarNTBooleanFound = true;
                break;
            }
        }

        if (statementsVisited->find(next2ProgLine) != statementsVisited->end()) {
            // there is a loop
            continue;
        }
        getNextBipStarNTUtil(make_pair(next2ProgLine, targetStmt), stmt2Type,
            nextBipStarPairs, statementsVisited);
        if (nextBipStarNTBooleanFound) {
            break;
        }
    }
}

void NextBipStarExtractor::getNextBipStarTNUtil(StatementTypes stmt1Type, pair<int, int> currentStatementAndTargetStatement,
    vector <pair<int, int>>& nextBipStarPairs, unordered_set<int>* statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int currentStatement = currentStatementAndTargetStatement.first;
    int targetStatement = currentStatementAndTargetStatement.second;
    // 1) Get all statements in NextBip relationship with stmt1
    vector<pair<int, int>> allNextBip = sharedPkb.getNextBipTN(NO_TYPE, currentStatement);

    if (allNextBip.size() == 0) {
        return;
    }
    statementsVisited->insert(currentStatement);
    for (int i = 0; i < allNextBip.size(); i++) {

        pair<int, int> currentPair = allNextBip[i];
        int next1ProgLine = currentPair.first;
        int next2ProgLine = currentPair.second;
        sharedPkb.addNextBipStar(next1ProgLine, targetStatement);
        StatementTypes next1ProgLineType = sharedPkb.getStatementType(next1ProgLine);
        if ((next1ProgLineType == stmt1Type) || (stmt1Type == NO_TYPE)) {
            pair<int, int> pairToAdd = make_pair(next1ProgLine, targetStatement);
            nextBipStarPairs.push_back(pairToAdd);
            if (nextBipStarTNBoolean) {
                nextBipStarTNBooleanFound = true;
                break;
            }
        }
        if (statementsVisited->find(next1ProgLine) != statementsVisited->end()) {
            // there is a loop
            continue;
        }
        getNextBipStarTNUtil(stmt1Type, make_pair(next1ProgLine, targetStatement),
            nextBipStarPairs, statementsVisited);
        if (nextBipStarTNBooleanFound) {
            break;
        }
    }
}

void NextBipStarExtractor::getNextBipStarTTUtil(StatementTypes stmt1Type, StatementTypes stmt2Type,
    std::vector<std::pair<int, int>>& nextBipStarPairs) {
    /* To iterate through the entire SIMPLE program's Next r/s. Only populate the vector
     if the types matches the required type*/

    PKB& sharedPkb = sharedPkb.getInstance();
    // 1) Get all statements in NextBip relationship with stmt1
    vector<pair<int, int>> allNextBip = sharedPkb.getNextBipTT(NO_TYPE, NO_TYPE);

    int prevLine = 0;
    for (int i = 0; i < allNextBip.size(); i++) {
        pair<int, int> currentPair = allNextBip[i];
        int next1ProgLine = currentPair.first;
        if (next1ProgLine == prevLine) {
            continue;
        }
        StatementTypes next1ProgLineType = sharedPkb.getStatementType(next1ProgLine);
        if ((next1ProgLineType == stmt1Type) || (stmt1Type == NO_TYPE)) {
            unordered_set<int>* isVisited = new unordered_set<int>();
            getNextBipStarTTInnerLoop(make_pair(next1ProgLine, next1ProgLine),
                make_pair(stmt1Type, stmt2Type), nextBipStarPairs, isVisited);
            if (nextBipStarTTBoolean && nextBipStarTTBooleanFound) {
                break;
            }
        }
        prevLine = next1ProgLine;
    }
}

void NextBipStarExtractor::getNextBipStarTTInnerLoop(pair<int, int> currentStmtNumAndTargetStmtNum, pair<StatementTypes, StatementTypes> stmt1TypeAndStmt2Type, std::vector<std::pair<int, int>>& nextBipStarPairs, unordered_set<int>* statementsVisited) {
    /* To iterate through the entire SIMPLE program's Next r/s. Only populate the vector
     if the types matches the required type*/

    PKB& sharedPkb = sharedPkb.getInstance();
    int currentStmtNum = currentStmtNumAndTargetStmtNum.first;
    int targetStmtNum = currentStmtNumAndTargetStmtNum.second;
    // 1) Get all statements in NextBip relationship with current stmtNum
    vector<pair<int, int>> allNextBip = sharedPkb.getNextBipNT(currentStmtNum, NO_TYPE);

    if (allNextBip.size() == 0) {
        return;
    }
    statementsVisited->insert(currentStmtNum);

    StatementTypes stmt1Type = stmt1TypeAndStmt2Type.first;
    StatementTypes stmt2Type = stmt1TypeAndStmt2Type.second;

    for (int i = 0; i < allNextBip.size(); i++) {
        pair<int, int> currentPair = allNextBip[i];
        int next1ProgLine = currentPair.first;
        int next2ProgLine = currentPair.second;
        sharedPkb.addNextStar(targetStmtNum, next2ProgLine);
        StatementTypes next2ProgLineType = sharedPkb.getStatementType(next2ProgLine);
        if ((next2ProgLineType == stmt2Type) || (stmt2Type == NO_TYPE)) {
            pair<int, int> pairToAdd = make_pair(targetStmtNum, next2ProgLine);
            nextBipStarPairs.push_back(pairToAdd);
            if (nextBipStarTTBoolean) {
                nextBipStarTTBooleanFound = true;
                break;
            }
        }
        if (statementsVisited->find(next2ProgLine) != statementsVisited->end()) {
            // there is a loop
            continue;
        }
        statementsVisited->insert(next2ProgLine);
        getNextBipStarTTInnerLoop(make_pair(next2ProgLine, targetStmtNum),
            stmt1TypeAndStmt2Type, nextBipStarPairs, statementsVisited);
        if (nextBipStarTTBooleanFound) {
            break;
        }
    }

}