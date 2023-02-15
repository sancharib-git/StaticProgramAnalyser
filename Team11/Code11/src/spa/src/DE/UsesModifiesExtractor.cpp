#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "UsesModifiesExtractor.h"
#include "ParserException.h"
#include <unordered_map>
#include <utility>
#include "PKB/PKB.h"
#include "PKB/CallsStore.h"
#include "StatementTypes.h"

using namespace std;

bool UsesModifiesExtractor::extractUsesModifies() {
    return addUsesModifies();
}

bool UsesModifiesExtractor::addUsesModifies() {
    // 1) Obtain the list of procedures in SIMPLE
    PKB& sharedPkb = sharedPkb.getInstance();
    unordered_set<string>* allProcedures = sharedPkb.getProcList();
    // Unordered_set to act as call set for the procedure
    unordered_set<string> proceduresCalled;
    // 2) For each procedure, populate Uses(p,v), Uses(c,v), Modifies(p,v) and Modifies(c,v)
    for (const string& procedure : *allProcedures) {
        // ensure proceduresCalled is empty
        proceduresCalled.clear();
        addUsesModifiesUtilProcedure(procedure, proceduresCalled);
    }

    return true;
}

/*
    Adds statement s in procedure such that Uses(s,v) holds.
    Adds statement c in procedure such that Uses(c,v) holds.
    Adds statement s in procedure such that Modifies(s,v) holds.
    Adds statement c in procedure such that Modifies(c,v) holds.
*/
bool UsesModifiesExtractor::addUsesModifiesUtilProcedure(string procedure, unordered_set<string>& proceduresCalled) {
    // 2) For each procedures, iterate through the range of statements in that procedures to get Uses(s,v)
    PKB& sharedPkb = sharedPkb.getInstance();
    checkSemanticErrorForProcedures(proceduresCalled, procedure);
    // Forms call set of procedures call. Uses set to check if there are any duplicates (cyclic calls).
    proceduresCalled.insert(procedure);
    vector<int> procedureRange = sharedPkb.getProcedureStmtNums(procedure);

    for (int i = 0; i < procedureRange.size(); i++) {
        // check if statement number is a call statement
        int statementNumber = procedureRange[i];
        StatementTypes statementType = sharedPkb.getStatementType(statementNumber);
        if (statementType == CALL) {
            processCallUsesModifies(statementNumber, proceduresCalled);
            continue;
        }

        if (sharedPkb.getParentTN(NO_TYPE, statementNumber).size() != 0) {
            // current statement has a parent, it is within a while/if/else block
            // uses and modifies of this statement is accounted for when adding uses/modifies for parent statement
            continue;
        }
        addUsesVarProc(proceduresCalled, statementNumber);
        addModifiesVarProc(proceduresCalled, statementNumber);
    }
    return true;
}

bool UsesModifiesExtractor::processCallUsesModifies(int statementNumber, unordered_set<string>& proceduresCalled) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, string>> callStmtVector = sharedPkb.getCallsSST(statementNumber, NO_TYPE);
    pair<int, string> callStmt = callStmtVector[0];
    string procedureInCall = callStmt.second;
    addUsesModifiesUtilProcedure(procedureInCall, proceduresCalled);

    // Populate Uses(c,v) and Modifies(c,v). procedureInCall should have been populated completely
    addUsesUtilCalls(statementNumber, procedureInCall);
    addModifiesUtilCalls(statementNumber, procedureInCall);

    if (sharedPkb.getParentTN(NO_TYPE, statementNumber).size() != 0) {
        // current statement has a parent, call is within a while/if/else block
        addUsesCallsContainer(statementNumber);
        addModifiesCallsContainer(statementNumber);
    }

    // Removes the procedure that has been populated from the call set
    unordered_set<string>::iterator procedureInCallIterator = proceduresCalled.find(procedureInCall);
    proceduresCalled.erase(procedureInCallIterator);
    return true;
}

bool UsesModifiesExtractor::checkSemanticErrorForProcedures(unordered_set<string>& proceduresCalled, string procedure) {
    PKB& sharedPkb = sharedPkb.getInstance();
    unordered_set<string>* allProcedures = sharedPkb.getProcList();
    if (allProcedures->find(procedure) == allProcedures->end()) {
        // throw exception, calling non-existing procedures
        throw ParserException("Invalid procedure called");
    }
    if (proceduresCalled.find(procedure) != proceduresCalled.end()) {
        // throw an exception here to be updated -> cyclic call detected
        throw ParserException("Recursive or Cyclic call detected");
    }
    return true;
}

/* Adds Uses(c,v) */
bool UsesModifiesExtractor::addUsesUtilCalls(int callStatementNumber, string procedureCalled) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<string, string>> usesVariables = sharedPkb.getUsesPPT(procedureCalled, NO_TYPE);
    for (const pair<string, string>& procedureVarPair : usesVariables) {
        string variableUsed = procedureVarPair.second;
        sharedPkb.addUsesS(callStatementNumber, CALL, variableUsed);
    }
    return true;
}

/* Adds Uses(p,v) */
bool UsesModifiesExtractor::addUsesVarProc(unordered_set<string>& proceduresCalled, int statementNumber) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, string>> allVariablesUsed = sharedPkb.getUsesSST(statementNumber, NO_TYPE);
    StatementTypes statementType = sharedPkb.getStatementType(statementNumber);
    for (const string& procedure : proceduresCalled) {
        for (const pair<int, string>& stmtVarPair : allVariablesUsed) {
            string varUsed = stmtVarPair.second;
            sharedPkb.addUsesP(procedure, statementType, varUsed);
        }
    }
    return true;
}

/* Adds Modifies(c,v) */
bool UsesModifiesExtractor::addModifiesUtilCalls(int callStatementNumber, string procedureCalled) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<string, string>> modifiesVariables = sharedPkb.getModifiesPPT(procedureCalled, NO_TYPE);
    for (const pair<string, string>& procedureVarPair : modifiesVariables) {
        string variableModified = procedureVarPair.second;
        sharedPkb.addModifiesS(callStatementNumber, CALL, variableModified);
    }
    return true;
}

/* Adds Modifies(p,v) */
bool UsesModifiesExtractor::addModifiesVarProc(unordered_set<string>& proceduresCalled, int statementNumber) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, string>> allVariablesModified = sharedPkb.getModifiesSST(statementNumber, NO_TYPE);
    StatementTypes statementType = sharedPkb.getStatementType(statementNumber);
    for (const string& procedure : proceduresCalled) {
        for (const pair<int, string>& stmtVarPair : allVariablesModified) {
            string varModified = stmtVarPair.second;
            sharedPkb.addModifiesP(procedure, statementType, varModified);
        }
    }
    return true;
}

bool UsesModifiesExtractor::addModifiesCallsContainer(int callStatementNumber) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, string>> allVariablesModified = sharedPkb.getModifiesSST(callStatementNumber, NO_TYPE);
    vector<pair<int, int>> allParentsStarOfCall = sharedPkb.getParentStarTN(NO_TYPE, callStatementNumber);
    for (const pair<int, int>& parent : allParentsStarOfCall) {
        int parentStmtNum = parent.first;
        StatementTypes parentType = sharedPkb.getStatementType(parentStmtNum);
        for (const pair<int, string>& variable : allVariablesModified) {
            string variableModified = variable.second;
            sharedPkb.addModifiesS(parentStmtNum, parentType, variableModified);
        }
    }
    return true;
}

bool UsesModifiesExtractor::addUsesCallsContainer(int callStatementNumber) {
    PKB& sharedPkb = sharedPkb.getInstance();
    vector<pair<int, string>> allVariablesUsed = sharedPkb.getUsesSST(callStatementNumber, NO_TYPE);
    vector<pair<int, int>> allParentsStarOfCall = sharedPkb.getParentStarTN(NO_TYPE, callStatementNumber);
    for (const pair<int, int>& parent : allParentsStarOfCall) {
        int parentStmtNum = parent.first;
        StatementTypes parentType = sharedPkb.getStatementType(parentStmtNum);
        for (const pair<int, string>& variable : allVariablesUsed) {
            string variableUsed = variable.second;
            sharedPkb.addUsesS(parentStmtNum, parentType, variableUsed);
        }
    }
    return true;
}