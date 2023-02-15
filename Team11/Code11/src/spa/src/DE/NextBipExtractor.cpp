#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "NextBipExtractor.h"
#include <unordered_map>
#include <utility>
#include "PKB/PKB.h"
#include "StatementTypes.h"

using namespace std;

// static method to interface with PKB to extractNextBip
bool NextBipExtractor::extractNextBip() {
    NextBipExtractor* process = new NextBipExtractor();
    bool isSuccess = process->extractNextBipUtil();
    delete process;
    return isSuccess;
}

// will extractNextBip for entire procedures, before retrieving from cache
bool NextBipExtractor::extractNextBipUtil() {
    PKB& sharedPkb = sharedPkb.getInstance();
    unordered_set<string>* allProcedures = sharedPkb.getProcList();
    unordered_map<string, vector<int>> processedProcedures;
    vector<int> callStatementsStack;
    for (const string& procedure : *allProcedures) {
        if (processedProcedures.find(procedure) != processedProcedures.end()) {
            continue;
        }
        extractNextBipProcedureUtil(procedure, processedProcedures, callStatementsStack);
    }
    return true;
}

bool NextBipExtractor::extractNextBipProcedureUtil(string procedure, std::unordered_map<std::string, std::vector<int>>& procedureProcessed, vector<int>& callStatementsStack) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<int> procStmtNums = sharedPkb.getProcedureStmtNums(procedure);
    vector<int> lastStatementsInProc;
    for (int i = 0; i < procStmtNums.size(); i++) {
        int stmt = procStmtNums[i];
        if (i == 0 && !callStatementsStack.empty()) {
            sharedPkb.addNextBip(callStatementsStack.back(), stmt);
        }
        pair<string, int> procedureStmtPair = make_pair(procedure, stmt);
        bool isLastStmt = extractNextBipProcedureStatementsUtil(procedureStmtPair, procedureProcessed, callStatementsStack);
        if (isLastStmt) {
            lastStatementsInProc.push_back(stmt);
        }
    }
    // populate last statements of procedure into procedureProcessed, also to add in nextBip back to call statements
    processNextBipLastStatements(callStatementsStack, lastStatementsInProc);
    procedureProcessed.insert(make_pair(procedure, lastStatementsInProc));
    return true;
}

bool NextBipExtractor::processNextBipLastStatements(vector<int>& callStatementsStack, vector<int>& lastStatements) {
    if (callStatementsStack.empty()) {
        return true;
    }
    PKB& sharedPkb = sharedPkb.getInstance();
    for (int i = (callStatementsStack.size() - 1); i >= 0; i--) {
        vector<pair<int, int>> nextStmt = sharedPkb.getNextNT(callStatementsStack[i], NO_TYPE);
        if (!nextStmt.empty()) {
            processNextBipLastStatementsUtil(lastStatements, nextStmt[0].second);
            return true;
        }
    }
    return true;
}

bool NextBipExtractor::processNextBipLastStatementsUtil(vector<int>& lastStatements, int callNext) {
    PKB& sharedPkb = sharedPkb.getInstance();
    for (int stmt : lastStatements) {
        sharedPkb.addNextBip(stmt, callNext);
    }
    return true;
}

// returns true if statement is the last statement in the procedure
bool NextBipExtractor::extractNextBipProcedureStatementsUtil(std::pair<std::string, int> procedureStmtPair, std::unordered_map<std::string, std::vector<int>>& procedureProcessed, std::vector<int>& callStatementsStack) {
    // Last statement if 1) it is while and has only 1 next 2) no next for other statements
    // caution about call -> recurse into the procedure
    PKB& sharedPkb = sharedPkb.getInstance();
    string procedure = procedureStmtPair.first;
    int stmt = procedureStmtPair.second;
    StatementTypes stmtType = sharedPkb.getStatementType(stmt);
    vector<pair<int, int>>  nextPairs = sharedPkb.getNextNT(stmt, NO_TYPE);
    // check if it is a call statement
    if ((stmtType == CALL)) {
        string procedureCalled = sharedPkb.getCallsSST(stmt, NO_TYPE)[0].second;
        if (procedureProcessed.find(procedureCalled) != procedureProcessed.end()) {
            pair<int, string> callPair = make_pair(stmt, procedureCalled);
            callStatementsStack.push_back(stmt);
            processCallToProcessedProcedure(procedureProcessed, callPair,
                callStatementsStack);
            callStatementsStack.pop_back();
            return (nextPairs.empty());
        }
        // go into the procedure
        callStatementsStack.push_back(stmt);
        extractNextBipProcedureUtil(procedureCalled, procedureProcessed, callStatementsStack);
        callStatementsStack.pop_back();
        return (nextPairs.empty());
    }

    for (int i = 0; i < nextPairs.size(); i++) {
        sharedPkb.addNextBip(nextPairs[i].first, nextPairs[i].second);
    }
    if ((stmtType == WHILE) && (nextPairs.size() < 2)) {
        return true;
    }
    else if (nextPairs.size() == 0) {
        return true;
    }
    return false;
}

bool NextBipExtractor::processCallToProcessedProcedure(unordered_map<string, vector<int>>& procedureProcessed, pair<int, string>& stmtProcedurePair, std::vector<int>& callStatementsStack) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int firstProcStmt = sharedPkb.getProcedureStmtNums(stmtProcedurePair.second).front();
    sharedPkb.addNextBip(stmtProcedurePair.first, firstProcStmt);
    vector<int> lastStatements = procedureProcessed.find(stmtProcedurePair.second)->second;
    processNextBipLastStatements(callStatementsStack, lastStatements);
    return true;
}