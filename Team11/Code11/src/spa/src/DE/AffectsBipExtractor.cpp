//
// Created by Xuan Zhi on 6/11/21.
//

#include "AffectsBipExtractor.h"
#include "PKB/PKB.h"
#include <unordered_set>


void AffectsBipExtractor::addNTBipAffectsPairs(std::pair<int, int> originalAndTargetNext, vector<pair<int, int>>& affectsPairs, unordered_set<int>* resultsAdded) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int originalStmt = originalAndTargetNext.first;
    int targetNext = originalAndTargetNext.second;
    string targetVar = sharedPkb.getAssignLHSVar(originalStmt);
    if (sharedPkb.getStatementType(targetNext) == ASSIGNMENT) {
        unordered_set<string> rhsVars = sharedPkb.getAssignRHSVars(targetNext);
        if (rhsVars.find(targetVar) != rhsVars.end()) {
            // sharedPkb.addAffects(originalStmt, targetNext);
            pair<int, int> pairToAdd = make_pair(originalStmt, targetNext);
            resultsAdded->insert(targetNext);
            affectsPairs.push_back(pairToAdd);
        }
    }
}

vector<pair<int, int>> AffectsBipExtractor::getAffectsBipNN(int stmt1, int stmt2) {
    PKB& sharedPkb = sharedPkb.getInstance();
    string stmt1LHSVar = sharedPkb.getAssignLHSVar(stmt1);
    unordered_set<std::string> stmt2RHSVars = sharedPkb.getAssignRHSVars(stmt2);
    vector<pair<int, int>> affectsPairs;

    if (stmt2RHSVars.find(stmt1LHSVar) == stmt2RHSVars.end()) {
        return affectsPairs;
    }

    unordered_set<int>* isVisited = new unordered_set<int>();

    getAffectsBipNNUtil(make_pair(stmt1, stmt2), stmt1, affectsPairs, isVisited);

    delete isVisited;
    return affectsPairs;
}

void getAffectsBipNNInnerCall(pair<int, int> currAndTargetStmt, int originStmt,
                              std::vector<std::pair<int, int>>& affectsPairs, std::unordered_set<int> *statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int currStmt = currAndTargetStmt.first;
    int targetStmt = currAndTargetStmt.second;
}

void AffectsBipExtractor::getAffectsBipNNUtil(pair<int, int> currAndTargetStmt, int originStmt,
                                              std::vector<std::pair<int, int>>& affectsPairs, std::unordered_set<int> *statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int currStmt = currAndTargetStmt.first;
    int targetStmt = currAndTargetStmt.second;

    string targetVar = sharedPkb.getAssignLHSVar(originStmt);
    vector<pair<int, int>> nextLines = sharedPkb.getNextNT(currStmt, NO_TYPE);
    if (statementsVisited->find(currStmt) != statementsVisited->end()) {
        if (nextLines.size() == 0) {
            return;
        }
        for (int j = 0; j < nextLines.size(); j++) {
            pair<int, int> nextPair = nextLines.at(j);
            int targetNext = nextPair.second;
            unordered_set<string> listOfVars{ targetVar };
            if (targetNext == targetStmt) {
                affectsPairs.push_back(make_pair(originStmt, targetNext));
                return;
            }
            if (isModifiedBip(targetNext, listOfVars)) {
                continue;
            }
            if (statementsVisited->find(targetNext) == statementsVisited->end()) {
                getAffectsBipNNUtil(make_pair(targetNext, originStmt), originStmt, affectsPairs, statementsVisited);
            }
        }
        return;
    }
    if (nextLines.size() == 0) {
        if (currStmt == originStmt) {
            nextLines = sharedPkb.getNextBipNT(currStmt, NO_TYPE);
        }
        if (nextLines.size() == 0) {
            return;
        }
    }
    statementsVisited->insert(currStmt);
    for (int i = 0; i < nextLines.size(); i++) {
        pair<int, int> currPair = nextLines.at(i);
        int currLine = currPair.first;
        int nextLine = currPair.second;
        if (nextLine == targetStmt) {
            affectsPairs.push_back(make_pair(originStmt, nextLine));
            return;
        }
        StatementTypes nextLineType = sharedPkb.getStatementType(nextLine);
        unordered_set<string> listOfVars{ targetVar };
        if (nextLineType == CALL) {
            vector<pair<int, int>> callNext = sharedPkb.getNextBipNT(nextLine, NO_TYPE);
            unordered_set<int>* isVisited = new unordered_set<int>();
            if (callNext.at(0).second == targetStmt) {
                affectsPairs.push_back(make_pair(originStmt, targetStmt));
                return;
            }
            if (isModifiedBip(callNext.at(0).second, listOfVars)) {
                continue;
            }
            getAffectsBipNNUtil(make_pair(callNext.at(0).second, targetStmt), originStmt, affectsPairs, isVisited);
        }

        if (isModifiedBip(nextLine, listOfVars)) {
            continue;
        }

        getAffectsBipNNUtil(make_pair(nextLine, targetStmt), originStmt, affectsPairs, statementsVisited);
    }
}

void AffectsBipExtractor::getAffectsBipNTUtil(std::pair<int, int> currStmtAndOriginalStmt, std::vector<std::pair<int, int>>& affectsPairs,
                         std::unordered_set<int>* statementsVisited, std::unordered_set<int>* resultsAdded) {

    PKB& sharedPkb = sharedPkb.getInstance();
    int currStmt = currStmtAndOriginalStmt.first;
    int originalStmt = currStmtAndOriginalStmt.second;
    string targetVar = sharedPkb.getAssignLHSVar(originalStmt);
    vector<pair<int, int>> nextLines = sharedPkb.getNextNT(currStmt, NO_TYPE);

    if (statementsVisited->find(currStmt) != statementsVisited->end()) {
        if (nextLines.size() == 0) {
            return;
        }
        for (int j = 0; j < nextLines.size(); j++) {
            pair<int, int> nextPair = nextLines.at(j);
            int targetNext = nextPair.second;

            unordered_set<string> listOfVars{ targetVar };
            addNTBipAffectsPairs(make_pair(originalStmt, targetNext), affectsPairs, resultsAdded);

            if (isModifiedBip(targetNext, listOfVars)) {
                continue;
            }
            if (statementsVisited->find(targetNext) == statementsVisited->end()) {
                getAffectsBipNTUtil(make_pair(targetNext, originalStmt), affectsPairs, statementsVisited, resultsAdded);
            }
        }
        return;
    }
    if (nextLines.size() == 0) {
        if (currStmt == originalStmt) {
            nextLines = sharedPkb.getNextBipNT(currStmt, NO_TYPE);
        }
        if (nextLines.size() == 0) {
            return;
        }
    }
    statementsVisited->insert(currStmt);
    for (int i = 0; i < nextLines.size(); i++) {
        pair<int, int> currPair = nextLines.at(i);
        int currLine = currPair.first;
        int nextLine = currPair.second;
        if (resultsAdded->find(nextLine) != resultsAdded->end()) {
            continue;
        }
        StatementTypes nextLineType = sharedPkb.getStatementType(nextLine);
        unordered_set<string> listOfVars{ targetVar };
        if (nextLineType == CALL) {
            vector<pair<int, int>> callNext = sharedPkb.getNextBipNT(nextLine, NO_TYPE);
            unordered_set<int>* isVisited = new unordered_set<int>();

            int c = callNext.at(0).second;
            addNTBipAffectsPairs(make_pair(c, nextLine), affectsPairs, resultsAdded);
            if (isModifiedBip(c, listOfVars)) {
                continue;
            }
            getAffectsBipNTUtil(make_pair(callNext.at(0).second, originalStmt), affectsPairs, isVisited, resultsAdded);
        }
        addNTBipAffectsPairs(make_pair(originalStmt, nextLine), affectsPairs, resultsAdded);

        if (isModifiedBip(nextLine, listOfVars)) {
            continue;
        }

        getAffectsBipNTUtil(make_pair(nextLine, originalStmt), affectsPairs, statementsVisited, resultsAdded);
    }
}


std::vector<std::pair<int, int>> AffectsBipExtractor::getAffectsBipNT(int stmt1, StatementTypes stmt2Type) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> affectsPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    unordered_set<int>* stmtAdded = new unordered_set<int>();

    getAffectsBipNTUtil(make_pair(stmt1, stmt1), affectsPairs, isVisited, stmtAdded);

    delete isVisited;
    delete stmtAdded;
    return affectsPairs;
}

std::vector<std::pair<int, int>> AffectsBipExtractor::getAffectsBipTN(StatementTypes stmt1Type, int stmt2) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> affectsPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    unordered_set<string> targetVars = sharedPkb.getAssignRHSVars(stmt2);
    if (targetVars.size() == 0) {
        return affectsPairs;
    }
    getAffectsBipTNUtil(make_pair(stmt2, stmt2), affectsPairs, targetVars, isVisited);

    delete isVisited;
    return affectsPairs;
}

void AffectsBipExtractor::getAffectsBipTNUtil(std::pair<int, int> currStmtAndOriginalStmt,
                                              std::vector<std::pair<int, int>> &affectsPairs,
                                              std::unordered_set<std::string> &targetVars,
                                              std::unordered_set<int> *statementsVisited) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int currStmt = currStmtAndOriginalStmt.first;
    int originalStmt = currStmtAndOriginalStmt.second;
    vector<pair<int, int>> nextLines = sharedPkb.getNextBipTN(NO_TYPE, currStmt);
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
                affectsPairs.push_back(pairToAdd);

                if (statementsVisited->find(prevLine) != statementsVisited->end()) {
                    continue;
                }
                fourth.erase(lhsVar);
                getAffectsBipTNUtil(make_pair(prevLine, originalStmt), affectsPairs, fourth, statementsVisited);
                statementsVisited->erase(prevLine);
            }
        }

        isReverseModifiedBip(prevLine, fourth);
        if (statementsVisited->find(prevLine) != statementsVisited->end()) {
            continue;
        }
        getAffectsBipTNUtil(make_pair(prevLine, originalStmt), affectsPairs, fourth, statementsVisited);
        statementsVisited->erase(prevLine);
    }

}

vector<pair<int, int>> AffectsBipExtractor::getAffectsBipTT(StatementTypes stmt1Type, StatementTypes stmt2Type) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, int>> affectsPairs;
    unordered_set<int>* isVisited = new unordered_set<int>();
    unordered_set<int>* listOfAssign = sharedPkb.getAssignList();

    for (const auto assignStmt : *listOfAssign) {
        string targetVar = sharedPkb.getAssignLHSVar(assignStmt);
        unordered_set<int>* stmtAdded = new unordered_set<int>();
        unordered_set<int>* isVisited = new unordered_set<int>();
        getAffectsBipNTUtil(make_pair(assignStmt, assignStmt), affectsPairs, isVisited, stmtAdded);

        delete isVisited;
        delete stmtAdded;
    }

    return affectsPairs;
}

bool AffectsBipExtractor::isModifiedBip(int stmtNum, unordered_set<string>& targetVars) {
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

bool AffectsBipExtractor::isReverseModifiedBip(int stmtNum, std::unordered_set<std::string> &targetVars) {
    PKB& sharedPkb = sharedPkb.getInstance();
    StatementTypes stmtType = sharedPkb.getStatementType(stmtNum);
    if (stmtType == READ) {
        string readVar = sharedPkb.getReadVar(stmtNum);
        if (targetVars.find(readVar) != targetVars.end()) {
            targetVars.erase(readVar);
        }
    }
    return true;
}