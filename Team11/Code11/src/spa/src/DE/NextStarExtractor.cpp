#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "NextStarExtractor.h"
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "PKB/PKB.h"
#include "StatementTypes.h"
#include "BinaryTree.h"

using namespace std;

int NextStarExtractor::nextStarNNFirstStatement = 0;
bool NextStarExtractor::nextStarNTBoolean = false;
bool NextStarExtractor::nextStarNTBooleanFound = false;
bool NextStarExtractor::nextStarTNBoolean = false;
bool NextStarExtractor::nextStarTNBooleanFound = false;
bool NextStarExtractor::nextStarTTBoolean = false;
bool NextStarExtractor::nextStarTTBooleanFound = false;

vector<pair<int, int>> NextStarExtractor::getNextStarNN(int stmt1, int stmt2) {
    PKB& sharedPkb = sharedPkb.getInstance();
    unordered_set<int>* isVisited = new unordered_set<int>();
    nextStarNNFirstStatement = stmt1;

    bool isFound = getNextStarNNUtil(stmt1, stmt2, isVisited);
    vector<pair<int, int>> allNextStar;
    if (isFound) {
        allNextStar.push_back(make_pair(stmt1, stmt2));
    }
    delete isVisited;
    return allNextStar;
}

void NextStarExtractor::setNextStarNTBoolean(bool isBooleanClause) {
    nextStarNTBoolean = isBooleanClause;
}

vector<pair<int, int>> NextStarExtractor::getNextStarNT(int stmt1, StatementTypes stmt2Type) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> nextStarPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    getNextStarNTUtil(make_pair(stmt1, stmt1), stmt2Type, nextStarPairs, isVisited);
    delete isVisited;
    nextStarNTBoolean = false;
    nextStarNTBooleanFound = false;
    return nextStarPairs;
}

void NextStarExtractor::setNextStarTNBoolean(bool isBooleanClause) {
    nextStarTNBoolean = isBooleanClause;
}

vector<pair<int, int>> NextStarExtractor::getNextStarTN(StatementTypes stmt1Type, int stmt2) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> nextStarPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    getNextStarTNUtil(stmt1Type, make_pair(stmt2, stmt2), nextStarPairs, isVisited);
    delete isVisited;
    nextStarTNBoolean = false;
    nextStarTNBooleanFound = false;
    return nextStarPairs;
}

void NextStarExtractor::setNextStarTTBoolean(bool isBooleanClause) {
    nextStarTTBoolean = isBooleanClause;
}

vector<pair<int, int>> NextStarExtractor::getNextStarTT(StatementTypes stmt1Type, StatementTypes stmt2Type) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> nextStarPairs;
    getNextStarTTUtil(stmt1Type, stmt2Type, nextStarPairs);
    nextStarTTBoolean = false;
    nextStarTTBooleanFound = false;
    return nextStarPairs;
}

bool NextStarExtractor::getNextStarNNUtil(int stmt1, int stmt2, unordered_set<int>* statementsVisited) {
    statementsVisited->insert(stmt1);
    PKB& sharedPkb = sharedPkb.getInstance();
    // 1) Get all statements in Next relationship with stmt1
    vector<pair<int, int>> allNext = sharedPkb.getNextNT(stmt1, NO_TYPE);
    if (allNext.size() == 0) {
        return false;
    }
    for (int i = 0; i < allNext.size(); i++) {
        pair<int, int> currentPair = allNext[i];
        int next1ProgLine = currentPair.first;
        int next2ProgLine = currentPair.second;
        sharedPkb.addNextStar(nextStarNNFirstStatement, next2ProgLine);
        if (next2ProgLine == stmt2) {
            return true;
        }
        // before recurse, check if I have visited that node before
        if (statementsVisited->find(next2ProgLine) != statementsVisited->end()) {
            continue;
        }
        bool isFound = getNextStarNNUtil(next2ProgLine, stmt2, statementsVisited);
        if (isFound) {
            return true;
        }
        //statementsVisited->erase(next2ProgLine);
    }
    return false;
}

void NextStarExtractor::getNextStarNTUtil(pair<int, int> currStmtAndTargetStmt, StatementTypes stmt2Type,
    vector <pair<int, int>>& nextStarPairs, unordered_set<int>* statementsVisited) {
    int currentStmt = currStmtAndTargetStmt.first;
    int targetStmt = currStmtAndTargetStmt.second;
    PKB& sharedPkb = sharedPkb.getInstance();
    // 1) Get all statements in Next relationship with stmt1
    vector<pair<int, int>> allNext = sharedPkb.getNextNT(currentStmt, NO_TYPE);
    if ((allNext.size() == 0)) {
        return;
    }

    statementsVisited->insert(currentStmt);
    for (int i = 0; i < allNext.size(); i++) {
        pair<int, int> currentPair = allNext[i];
        int next1ProgLine = currentPair.first;
        int next2ProgLine = currentPair.second;
        sharedPkb.addNextStar(targetStmt, next2ProgLine);
        StatementTypes next2ProgLineType = sharedPkb.getStatementType(next2ProgLine);
        if ((next2ProgLineType == stmt2Type) || (stmt2Type == NO_TYPE)) {
            pair<int, int> pairToAdd = make_pair(targetStmt, next2ProgLine);
            nextStarPairs.push_back(pairToAdd);
            if (nextStarNTBoolean) {
                nextStarNTBooleanFound = true;
                break;
            }
        }

        if (statementsVisited->find(next2ProgLine) != statementsVisited->end()) {
            // there is a loop
            //statementsVisited->erase(currentStmt);
            continue;
        }
        getNextStarNTUtil(make_pair(next2ProgLine, targetStmt), stmt2Type,
            nextStarPairs, statementsVisited);
        if (nextStarNTBooleanFound) {
            break;
        }
        //statementsVisited->erase(next2ProgLine);
    }
}

void NextStarExtractor::getNextStarTNUtil(StatementTypes stmt1Type, pair<int, int> currentStatementAndTargetStatement,
    vector <pair<int, int>>& nextStarPairs, unordered_set<int>* statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int currentStatement = currentStatementAndTargetStatement.first;
    int targetStatement = currentStatementAndTargetStatement.second;
    // 1) Get all statements in Next relationship with stmt1
    vector<pair<int, int>> allNext = sharedPkb.getNextTN(NO_TYPE, currentStatement);

    if (allNext.size() == 0) {
        return;
    }
    statementsVisited->insert(currentStatement);
    for (int i = 0; i < allNext.size(); i++) {

        pair<int, int> currentPair = allNext[i];
        int next1ProgLine = currentPair.first;
        int next2ProgLine = currentPair.second;
        sharedPkb.addNextStar(next1ProgLine, targetStatement);
        StatementTypes next1ProgLineType = sharedPkb.getStatementType(next1ProgLine);
        if ((next1ProgLineType == stmt1Type) || (stmt1Type == NO_TYPE)) {
            pair<int, int> pairToAdd = make_pair(next1ProgLine, targetStatement);
            nextStarPairs.push_back(pairToAdd);
            if (nextStarTNBoolean) {
                nextStarTNBooleanFound = true;
                break;
            }
        }
        if (statementsVisited->find(next1ProgLine) != statementsVisited->end()) {
            // there is a loop
            //statementsVisited->erase(currentStatement);
            continue;
        }
        getNextStarTNUtil(stmt1Type, make_pair(next1ProgLine, targetStatement),
            nextStarPairs, statementsVisited);
        //statementsVisited->erase(next1ProgLine);
        if (nextStarTNBooleanFound) {
            break;
        }
    }
}

void NextStarExtractor::getNextStarTTUtil(StatementTypes stmt1Type, StatementTypes stmt2Type,
    std::vector<std::pair<int, int>>& nextStarPairs) {
    /* To iterate through the entire SIMPLE program's Next r/s. Only populate the vector
     if the types matches the required type*/

    PKB& sharedPkb = sharedPkb.getInstance();
    // 1) Get all statements in Next relationship with stmt1
    vector<pair<int, int>> allNext = sharedPkb.getNextTT(NO_TYPE, NO_TYPE);

    int prevLine = 0;
    for (int i = 0; i < allNext.size(); i++) {
        pair<int, int> currentPair = allNext[i];
        int next1ProgLine = currentPair.first;
        if (next1ProgLine == prevLine) {
            continue;
        }
        StatementTypes next1ProgLineType = sharedPkb.getStatementType(next1ProgLine);
        if ((next1ProgLineType == stmt1Type) || (stmt1Type == NO_TYPE)) {
            unordered_set<int>* isVisited = new unordered_set<int>();
            getNextStarTTInnerLoop(make_pair(next1ProgLine, next1ProgLine),
                make_pair(stmt1Type, stmt2Type), nextStarPairs, isVisited);
            if (nextStarTTBoolean && nextStarTTBooleanFound) {
                break;
            }
        }
        prevLine = next1ProgLine;
    }
}

void NextStarExtractor::getNextStarTTInnerLoop(pair<int, int> currentStmtNumAndTargetStmtNum, pair<StatementTypes, StatementTypes> stmt1TypeAndStmt2Type, std::vector<std::pair<int, int>>& nextStarPairs, unordered_set<int>* statementsVisited) {
    /* To iterate through the entire SIMPLE program's Next r/s. Only populate the vector
     if the types matches the required type*/

    PKB& sharedPkb = sharedPkb.getInstance();
    int currentStmtNum = currentStmtNumAndTargetStmtNum.first;
    int targetStmtNum = currentStmtNumAndTargetStmtNum.second;
    // 1) Get all statements in Next relationship with current stmtNum
    vector<pair<int, int>> allNext = sharedPkb.getNextNT(currentStmtNum, NO_TYPE);

    if (allNext.size() == 0) {
        return;
    }
    statementsVisited->insert(currentStmtNum);

    StatementTypes stmt1Type = stmt1TypeAndStmt2Type.first;
    StatementTypes stmt2Type = stmt1TypeAndStmt2Type.second;

    for (int i = 0; i < allNext.size(); i++) {
        pair<int, int> currentPair = allNext[i];
        int next1ProgLine = currentPair.first;
        int next2ProgLine = currentPair.second;
        sharedPkb.addNextStar(targetStmtNum, next2ProgLine);
        StatementTypes next2ProgLineType = sharedPkb.getStatementType(next2ProgLine);
        if ((next2ProgLineType == stmt2Type) || (stmt2Type == NO_TYPE)) {
            pair<int, int> pairToAdd = make_pair(targetStmtNum, next2ProgLine);
            nextStarPairs.push_back(pairToAdd);
            if (nextStarTTBoolean) {
                nextStarTTBooleanFound = true;
                break;
            }
        }
        if (statementsVisited->find(next2ProgLine) != statementsVisited->end()) {
            //statementsVisited->erase(currentStmtNum);
            // there is a loop
            continue;
        }
        statementsVisited->insert(next2ProgLine);
        //if (next2ProgLine != next1ProgLine) {
          //  getNextStarTTInnerLoop(make_pair(next2ProgLine, targetStmtNum),
            //    stmt1TypeAndStmt2Type, nextStarPairs, statementsVisited);
       // }
        getNextStarTTInnerLoop(make_pair(next2ProgLine, targetStmtNum),
            stmt1TypeAndStmt2Type, nextStarPairs, statementsVisited);
        //statementsVisited->erase(next2ProgLine);
        if (nextStarTTBooleanFound) {
            break;
        }
    }

}