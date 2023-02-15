#pragma once

#include <iostream>
#include <stdexcept>
#include "PKB.h"
#include "Table.h"
#include "DE/DesignExtractor.h"
#include "Store.h"
#include "Definitions.h"
#include "Cache.h"
#include <algorithm>

using namespace std;

//Initialize pointer to zero so that it can be initialized in first call to getInstance
PKB *PKB::instance = 0;

Store PKB::store;
Cache PKB::cache;

bool PKB::setStatementType(int statementNumber, StatementTypes stmtType) {
    return store.setStatementType(statementNumber, stmtType);
}

bool PKB::setNum(int num) {
    return store.setNum(num);
}

int PKB::getTotalNumberOfStatements() {
    return store.getTotalNumberOfStatements();
}

StatementTypes PKB::getStatementType(int statementNumber) {
    return store.getStatementType(statementNumber);
}

bool PKB::addIfStmtCV(int ifStmtNum, string var) {
    return store.addIfStmtCV(ifStmtNum, var);
}

bool PKB::addWhileStmtCV(int whileStmtNum, string var) {
    return store.addWhileStmtCV(whileStmtNum, var);
}

UNORDERED_LIST_OF_INT PKB::getIfStmtsWithCV(string var) {
    return store.getIfStmtsWithCV(var);
}

UNORDERED_LIST_OF_STR PKB::getAllCVInIfStmt(int stmtNum) {
    return store.getAllCVInIfStmt(stmtNum);
}

UNORDERED_LIST_OF_INT PKB::getWhileStmtsWithCV(string var) {
    return store.getWhileStmtsWithCV(var);
}

UNORDERED_LIST_OF_STR PKB::getAllCVInWhileStmt(int stmtNum) {
    return store.getAllCVInWhileStmt(stmtNum);
}

bool PKB::addAssignLHSVar(int stmtNum, string var) {
    return store.addAssignLHSVar(stmtNum, var);
}

bool PKB::addAssignRHSVar(int stmtNum, string var) {
    return store.addAssignRHSVar(stmtNum, var);
}

bool PKB::addAssignRHSConst(int stmtNum, int constant) {
    return store.addAssignRHSConst(stmtNum, constant);
}

bool PKB::addAssignPattern(int stmtNum, string pattern) {
    return store.addAssignPattern(stmtNum, pattern);
}

string PKB::getAssignPattern(int stmtNum) {
    return store.getAssignPattern(stmtNum);
}

bool PKB::addAssignSubExp(int stmtNum, string subExp) {
    return store.addAssignSubExp(stmtNum, subExp);
}

UNORDERED_LIST_OF_INT PKB::getSubExpStmtNums(string subExp) {
    return store.getSubExpStmtNums(subExp);
}

std::string PKB::getAssignLHSVar(int stmtNum) {
    return store.getAssignLHSVar(stmtNum);
}

UNORDERED_LIST_OF_INT PKB::getAssignRHSConst(int stmtNum) {
    return store.getAssignRHSConst(stmtNum);
}


UNORDERED_LIST_OF_STR PKB::getAssignRHSVars(int stmtNum) {
    return store.getAssignRHSVars(stmtNum);
}

UNORDERED_LIST_OF_INT *PKB::getAssignList() {
    return store.getAssignList();
}

LIST_OF_STR PKB::getAssignRHSVarsVector(int stmtNum) {
    return store.getAssignRHSVarsVector(stmtNum);
}

LIST_OF_INT PKB::getAssignRHSConstVector(int stmtNum) {
    return store.getAssignRHSConstVector(stmtNum);
}

/////////////////////// READ ///////////////////////

bool PKB::addReadStmt(int stmtNum, string var) {
    return store.addReadStmt(stmtNum, var);
}

UNORDERED_LIST_OF_INT *PKB::getReadStmtList() {
    return store.getReadStmtList();
}

std::string PKB::getReadVar(int stmtNum) {
    return store.getReadVar(stmtNum);
}

/////////////////////// PRINT ///////////////////////

bool PKB::addPrintStmt(int stmtNum, string var) {
    return store.addPrintStmt(stmtNum, var);
}

UNORDERED_LIST_OF_INT *PKB::getPrintStmtList() {
    return store.getPrintStmtList();
}

std::string PKB::getPrintVar(int stmtNum) {
    return store.getPrintVar(stmtNum);
}

/////////////////////// IF ///////////////////////

bool PKB::addIfStmt(int stmtNum) {
    return store.addIfStmt(stmtNum);
}

UNORDERED_LIST_OF_INT *PKB::getIfList() {
    return store.getIfList();
}

bool PKB::addIfBlockStmt(int ifStmtNum, int blockStmtNum) {
    return store.addIfBlockStmt(ifStmtNum, blockStmtNum);
}

LIST_OF_INT PKB::getIfBlockList(int ifStmtNum) {
    return store.getIfBlockList(ifStmtNum);
}

bool PKB::addElseBlockStmt(int ifStmtNum, int blockStmtNum) {
    return store.addElseBlockStmt(ifStmtNum, blockStmtNum);
}

LIST_OF_INT PKB::getElseBlockList(int ifStmtNum) {
    return store.getElseBlockList(ifStmtNum);
}

/////////////////////// WHILE ///////////////////////

bool PKB::addWhileStmt(int stmtNum) {
    return store.addWhileStmt(stmtNum);
}

UNORDERED_LIST_OF_INT *PKB::getWhileList() {
    return store.getWhileList();
}

bool PKB::addWhileBlockStmt(int whileStmtNum, int blockStmtNum) {
    return store.addWhileBlockStmt(whileStmtNum, blockStmtNum);
}

LIST_OF_INT PKB::getWhileBlockList(int whileStmtNum) {
    return store.getWhileBlockList(whileStmtNum);
}

/////////////////////// PROCEDURE ///////////////////////

bool PKB::addProcedure(std::string procName) {
    return store.addProcedure(procName);
}

int PKB::getProcDigit(string procName) {
    return store.getProcDigit(procName);
}

string PKB::getProcName(int procDigit) {
    return store.getProcName(procDigit);
}

UNORDERED_LIST_OF_STR *PKB::getProcList() {
    return store.getProcList();
}

LIST_OF_INT PKB::getProcedureStmtNums(string procName) {
    LIST_OF_INT procStmtNums = store.getProcedureStmtNums(procName);
    sort(procStmtNums.begin(), procStmtNums.end());
    return procStmtNums;
}

/////////////////////// STATEMENT ///////////////////////

bool PKB::addStmt(int stmtNum, StatementTypes type, std::string procName) {
    return store.addStmt(stmtNum, type, procName);
}

/////////////////////// VARIABLE ///////////////////////

bool PKB::addVar(string var) {
    return store.addVar(var);
}

int PKB::getTotalNumOfVars() {
    return store.getTotalNumOfVars();
}

UNORDERED_LIST_OF_STR *PKB::getAllVars() {
    return store.getAllVars();
}

/////////////////////// CONSTANT ///////////////////////

bool PKB::addConst(int constant) {
    return store.addConst(constant);
}

int PKB::getTotalNumOfConsts() {
    return store.getTotalNumOfConsts();
}

UNORDERED_LIST_OF_INT *PKB::getAllConsts() {
    return store.getAllConsts();
}

///////////////////////// FOLLOW ///////////////////////

bool PKB::addFollows(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return store.addFollows(stmt1, type1, stmt2, type2);
}

LIST_OF_INT_PAIRS PKB::getFollowsNN(int stmt1, int stmt2) {
    return store.getFollowsNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getFollowsTT(StatementTypes type1, StatementTypes type2) {
    return store.getFollowsTT(type1, type2);
}

LIST_OF_INT_PAIRS PKB::getFollowsNT(int stmt1, StatementTypes type2) {
    return store.getFollowsNT(stmt1, type2);
}

LIST_OF_INT_PAIRS PKB::getFollowsTN(StatementTypes type1, int stmt2) {
    return store.getFollowsTN(type1, stmt2);
}

///////////////////////// FOLLOW STAR ///////////////////////

bool PKB::addFollowsStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return store.addFollowsStar(stmt1, type1, stmt2, type2);
}

LIST_OF_INT_PAIRS PKB::getFollowsStarNN(int stmt1, int stmt2) {
    return store.getFollowsStarNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getFollowsStarTT(StatementTypes type1, StatementTypes type2) {
    return store.getFollowsStarTT(type1, type2);
}

LIST_OF_INT_PAIRS PKB::getFollowsStarNT(int stmt1, StatementTypes type2) {
    return store.getFollowsStarNT(stmt1, type2);
}

LIST_OF_INT_PAIRS PKB::getFollowsStarTN(StatementTypes type1, int stmt2) {
    return store.getFollowsStarTN(type1, stmt2);
}

/////////////////////// PARENT ///////////////////////

bool PKB::addParent(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return store.addParent(stmt1, type1, stmt2, type2);
}

LIST_OF_INT_PAIRS PKB::getParentNN(int stmt1, int stmt2) {
    return store.getParentNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getParentTT(StatementTypes type1, StatementTypes type2) {
    return store.getParentTT(type1, type2);
}

LIST_OF_INT_PAIRS PKB::getParentNT(int stmt1, StatementTypes type2) {
    return store.getParentNT(stmt1, type2);
}

LIST_OF_INT_PAIRS PKB::getParentTN(StatementTypes type1, int stmt2) {
    return store.getParentTN(type1, stmt2);
}

/////////////////////// PARENT STAR ///////////////////////

bool PKB::addParentStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return store.addParentStar(stmt1, type1, stmt2, type2);
}

LIST_OF_INT_PAIRS PKB::getParentStarNN(int stmt1, int stmt2) {
    return store.getParentStarNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getParentStarTT(StatementTypes type1, StatementTypes type2) {
    return store.getParentStarTT(type1, type2);
}

LIST_OF_INT_PAIRS PKB::getParentStarNT(int stmt1, StatementTypes type2) {
    return store.getParentStarNT(stmt1, type2);
}

LIST_OF_INT_PAIRS PKB::getParentStarTN(StatementTypes type1, int stmt2) {
    return store.getParentStarTN(type1, stmt2);
}

/////////////// USES STATEMENT API ///////////////

bool PKB::addUsesS(int stmtNum, StatementTypes type, std::string varUsed) {
    return store.addUsesS(stmtNum, type, varUsed);
}

LIST_OF_INTSTR_PAIRS PKB::getUsesSSV(int stmt1, string var2) {
    return store.getUsesSSV(stmt1, var2);
}

LIST_OF_INTSTR_PAIRS PKB::getUsesSTT(StatementTypes type1, StatementTypes type2) {
    return store.getUsesSTT(type1, type2);
}

LIST_OF_INTSTR_PAIRS PKB::getUsesSST(int stmt1, StatementTypes type2) {
    return store.getUsesSST(stmt1, type2);
}

LIST_OF_INTSTR_PAIRS PKB::getUsesSTV(StatementTypes type1, string var2) {
    return store.getUsesSTV(type1, var2);
}

/////////////// USES PROCEDURE API ///////////////

bool PKB::addUsesP(string procName, StatementTypes type, std::string varUsed) {
    return store.addUsesP(procName, type, varUsed);
}

LIST_OF_STR_PAIRS PKB::getUsesPPV(string proc1, string var2) {
    return store.getUsesPPV(proc1, var2);
}

LIST_OF_STR_PAIRS PKB::getUsesPTT(StatementTypes type1, StatementTypes type2) {
    return store.getUsesPTT(type1, type2);
}

LIST_OF_STR_PAIRS PKB::getUsesPPT(string proc1, StatementTypes type2) {
    return store.getUsesPPT(proc1, type2);
}

LIST_OF_STR_PAIRS PKB::getUsesPTV(StatementTypes type1, string var2) {
    return store.getUsesPTV(type1, var2);
}

/////////////// MODIFIES STATEMENT API ///////////////

bool PKB::addModifiesS(int stmtNum, StatementTypes type, std::string varModified) {
    return store.addModifiesS(stmtNum, type, varModified);
}

LIST_OF_INTSTR_PAIRS PKB::getModifiesSSV(int stmt1, string var2) {
    return store.getModifiesSSV(stmt1, var2);
}

LIST_OF_INTSTR_PAIRS PKB::getModifiesSTT(StatementTypes type1, StatementTypes type2) {
    return store.getModifiesSTT(type1, type2);
}

LIST_OF_INTSTR_PAIRS PKB::getModifiesSST(int stmt1, StatementTypes type2) {
    return store.getModifiesSST(stmt1, type2);
}

LIST_OF_INTSTR_PAIRS PKB::getModifiesSTV(StatementTypes type1, string var2) {
    return store.getModifiesSTV(type1, var2);
}

/////////////// MODIFIES PROCEDURE API ///////////////

bool PKB::addModifiesP(string procName, StatementTypes type, std::string varModified) {
    return store.addModifiesP(procName, type, varModified);
}

LIST_OF_STR_PAIRS PKB::getModifiesPPV(string proc1, string var2) {
    return store.getModifiesPPV(proc1, var2);
}

LIST_OF_STR_PAIRS PKB::getModifiesPTT(StatementTypes type1, StatementTypes type2) {
    return store.getModifiesPTT(type1, type2);
}

LIST_OF_STR_PAIRS PKB::getModifiesPPT(string proc1, StatementTypes type2) {
    return store.getModifiesPPT(proc1, type2);
}

LIST_OF_STR_PAIRS PKB::getModifiesPTV(StatementTypes type1, string var2) {
    return store.getModifiesPTV(type1, var2);
}

/////////////////////// CALL API ///////////////////////

bool PKB::addCalls(string procName, StatementTypes type, string procCalled, int stmtNum) {
    return store.addCalls(procName, type, procCalled, stmtNum);
}

LIST_OF_INTSTR_PAIRS PKB::getCallsSSP(int stmtNum, string procCalled) {
    return store.getCallsSSP(stmtNum, procCalled);
}

LIST_OF_INTSTR_PAIRS PKB::getCallsSTT(StatementTypes type1, StatementTypes type2) {
    return store.getCallsSTT(type1, type2);
}

LIST_OF_INTSTR_PAIRS PKB::getCallsSST(int stmtNum, StatementTypes type2) {
    return store.getCallsSST(stmtNum, type2);
}

LIST_OF_INTSTR_PAIRS PKB::getCallsSTP(StatementTypes type1, string procCalled) {
    return store.getCallsSTP(type1, procCalled);
}

LIST_OF_STR_PAIRS PKB::getCallsPPP(string procName, string procCalled) {
    return store.getCallsPPP(procName, procCalled);
}

LIST_OF_STR_PAIRS PKB::getCallsPTT(StatementTypes type1, StatementTypes type2) {
    return store.getCallsPTT(type1, type2);
}

LIST_OF_STR_PAIRS PKB::getCallsPPT(string procName, StatementTypes type2) {
    return store.getCallsPPT(procName, type2);
}

LIST_OF_STR_PAIRS PKB::getCallsPTP(StatementTypes type1, string procCalled) {
    return store.getCallsPTP(type1, procCalled);
}

UNORDERED_LIST_OF_INT *PKB::getCallStmtList() {
    return store.getCallStmtList();
}

/////////////////////// CALL STAR API ///////////////////////

bool PKB::addCallsStar(string procName, string procCalled) {
    return store.addCallsStar(procName, procCalled);
}

LIST_OF_STR_PAIRS PKB::getCallsStarNN(string procName, string procCalled) {
    return store.getCallsStarNN(procName, procCalled);
}

LIST_OF_STR_PAIRS PKB::getCallsStarTT(StatementTypes type1, StatementTypes type2) {
    return store.getCallsStarTT(type1, type2);
}

LIST_OF_STR_PAIRS PKB::getCallsStarNT(string procName, StatementTypes type2) {
    return store.getCallsStarNT(procName, type2);
}

LIST_OF_STR_PAIRS PKB::getCallsStarTN(StatementTypes type1, string procCalled) {
    return store.getCallsStarTN(type1, procCalled);
}

/////////////////////// NEXT API ///////////////////////

bool PKB::addNext(int stmt1, int stmt2) {
    return store.addNext(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getNextNN(int stmt1, int stmt2) {
    return store.getNextNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getNextTT(StatementTypes type1, StatementTypes type2) {
    return store.getNextTT(type1, type2);
}

LIST_OF_INT_PAIRS PKB::getNextNT(int stmt1, StatementTypes type2) {
    return store.getNextNT(stmt1, type2);
}

LIST_OF_INT_PAIRS PKB::getNextTN(StatementTypes type1, int stmt2) {
    return store.getNextTN(type1, stmt2);
}

/////////////////////// NEXT STAR API ///////////////////////

bool PKB::addNextStar(int stmt1, int stmt2) {
    return cache.addNextStar(stmt1, getStatementType(stmt1), stmt2, getStatementType(stmt2));
}

LIST_OF_INT_PAIRS PKB::getNextStarNN(int stmt1, int stmt2) {
    if (!cache.isNextStarNNCalled(stmt1, stmt2)) {
        cache.addNextStarNNCalled(stmt1, stmt2);
        return DesignExtractor::getNextStarNN(stmt1, stmt2);
    }
    return cache.getNextStarNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getNextStarTT(StatementTypes type1, StatementTypes type2) {
    if (!cache.isNextStarTTCalled(type1)) {
        cache.addNextStarTTCalled(type1);
        DesignExtractor::getNextStarTT(type1, type2);
//        return DesignExtractor::getNextStarTT(type1, type2);
    }
    return cache.getNextStarTT(type1, type2);
}

bool PKB::getNextStarTTBoolean(StatementTypes type1, StatementTypes type2) {
    // Next* used as a boolean clause, does not need exact results
    if (!cache.getNextStarTT(type1, type2).empty()) {
        return true;
    }
    DesignExtractor::setNextStarTTBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getNextStarTT(type1, type2);
    return !results.empty();
}

LIST_OF_INT_PAIRS PKB::getNextStarNT(int stmt1, StatementTypes type2) {
    if (!cache.isNextStarNTCalled(stmt1, type2)) {
        cache.addNextStarNTCalled(stmt1, type2);
        DesignExtractor::getNextStarNT(stmt1, type2);
    }
    return cache.getNextStarNT(stmt1, type2);
}

bool PKB::getNextStarNTBoolean(int stmt1, StatementTypes type2) {
    // Next* used as a boolean clause, does not need exact results
    if (!cache.getNextStarNT(stmt1, type2).empty()) {
        return true;
    }
    DesignExtractor::setNextStarNTBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getNextStarNT(stmt1, type2);
    return !results.empty();
}

LIST_OF_INT_PAIRS PKB::getNextStarTN(StatementTypes type1, int stmt2) {
    if (!cache.isNextStarTNCalled(type1, stmt2)) {
        cache.addNextStarTNCalled(type1, stmt2);
        DesignExtractor::getNextStarTN(type1, stmt2);
    }
    return cache.getNextStarTN(type1, stmt2);
}

bool PKB::getNextStarTNBoolean(StatementTypes type1, int stmt2) {
    // Next* used as a boolean clause, does not need exact results
    if (!cache.getNextStarTN(type1, stmt2).empty()) {
        return true;
    }
    DesignExtractor::setNextStarTNBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getNextStarTN(type1, stmt2);
    return !results.empty();
}

/////////////////////// NEXTBIP ///////////////////////

bool PKB::addNextBip(int stmt1, int stmt2) {
    return store.addNextBip(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getNextBipNN(int stmt1, int stmt2) {
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }
    return store.getNextBipNN(stmt1, stmt2);
}
LIST_OF_INT_PAIRS PKB::getNextBipTT(StatementTypes type1, StatementTypes type2) {
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }
    return store.getNextBipTT(type1, type2);
}
LIST_OF_INT_PAIRS PKB::getNextBipNT(int stmt1, StatementTypes type2) {
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }
    return store.getNextBipNT(stmt1, type2);
}
LIST_OF_INT_PAIRS PKB::getNextBipTN(StatementTypes type1, int stmt2) {
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }
    return store.getNextBipTN(type1, stmt2);
}

/////////////////////// NEXTBIP STAR ///////////////////////

bool PKB::addNextBipStar(int stmt1, int stmt2) {
    return cache.addNextBipStar(stmt1, getStatementType(stmt1), stmt2, getStatementType(stmt2));
}

LIST_OF_INT_PAIRS PKB::getNextBipStarNN(int stmt1, int stmt2) {
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }
    if (!cache.isNextBipStarNNCalled(stmt1, stmt2)) {
        cache.addNextBipStarNNCalled(stmt1, stmt2);
        DesignExtractor::getNextBipStarNN(stmt1, stmt2);
    }
    return cache.getNextBipStarNN(stmt1, stmt2);
}
LIST_OF_INT_PAIRS PKB::getNextBipStarTT(StatementTypes type1, StatementTypes type2) {
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }
    if (!cache.isNextBipStarTTCalled(type1)) {
        cache.addNextBipStarTTCalled(type1);
        DesignExtractor::getNextBipStarTT(type1, type2);
    }
    return cache.getNextBipStarTT(type1, type2);
}
LIST_OF_INT_PAIRS PKB::getNextBipStarNT(int stmt1, StatementTypes type2) {
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }
    if (!cache.isNextBipStarNTCalled(stmt1, type2)) {
        cache.addNextBipStarNTCalled(stmt1, type2);
        DesignExtractor::getNextBipStarNT(stmt1, type2);
    }

    return cache.getNextBipStarNT(stmt1, type2);
}
LIST_OF_INT_PAIRS PKB::getNextBipStarTN(StatementTypes type1, int stmt2) {
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }

    if (!cache.isNextBipStarTNCalled(type1, stmt2)) {
        cache.addNextBipStarTNCalled(type1, stmt2);
        DesignExtractor::getNextBipStarTN(type1, stmt2);
    }


    return cache.getNextBipStarTN(type1, stmt2);
}

bool PKB::getNextBipStarTNBoolean(StatementTypes type1, int stmt2) {
    // NextBip* used as a boolean clause, does not need exact results
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }
    if (!cache.getNextBipStarTN(type1, stmt2).empty()) {
        return true;
    }
    DesignExtractor::setNextBipStarTNBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getNextBipStarTN(type1, stmt2);
    return !results.empty();
}

bool PKB::getNextBipStarTTBoolean(StatementTypes type1, StatementTypes type2) {
    // NextBip* used as a boolean clause, does not need exact results
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }
    if (!cache.getNextBipStarTT(type1, type2).empty()) {
        return true;
    }
    DesignExtractor::setNextBipStarTTBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getNextBipStarTT(type1, type2);
    return !results.empty();
}

bool PKB::getNextBipStarNTBoolean(int stmt1, StatementTypes type2) {
    // NextBip* used as a boolean clause, does not need exact results
    if (!store.getIsNextBipComputed()) {
        DesignExtractor::extractNextBip();
        store.setNextBipComputed();
    }
    if (!cache.getNextBipStarNT(stmt1, type2).empty()) {
        return true;
    }
    DesignExtractor::setNextBipStarNTBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getNextBipStarNT(stmt1, type2);
    return !results.empty();
}

/////////////////////// AFFECTS ///////////////////////

bool PKB::addAffects(int stmt1, int stmt2) {
    return cache.addAffects(stmt1, getStatementType(stmt1), stmt2, getStatementType(stmt2));
}

LIST_OF_INT_PAIRS PKB::getAffectsNN(int stmt1, int stmt2) {
    if (!cache.isAffectsNNCalled(stmt1, stmt2)) {
        cache.addAffectsNNCalled(stmt1, stmt2);
        return DesignExtractor::getAffectsNN(stmt1, stmt2);
    }
    return cache.getAffectsNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getAffectsNT(int stmt1, StatementTypes type2) {
    if (!cache.isAffectsNTCalled(stmt1, type2)) {
        cache.addAffectsNTCalled(stmt1, type2);
        return DesignExtractor::getAffectsNT(stmt1, type2);
    }
    return cache.getAffectsNT(stmt1, type2);

}

LIST_OF_INT_PAIRS PKB::getAffectsTT(StatementTypes type1, StatementTypes type2) {
    if (!cache.isAffectsTTCalled(type1, type2)) {
        cache.addAffectsTTCalled(type1, type2);
        return DesignExtractor::getAffectsTT(type1, type2);
    }
    return cache.getAffectsNT(type1, type2);
}

LIST_OF_INT_PAIRS PKB::getAffectsTN(StatementTypes type1, int stmt2) {
    if (!cache.isAffectsTNCalled(type1, stmt2)) {
        cache.addAffectsTNCalled(type1, stmt2);
        return DesignExtractor::getAffectsTN(type1, stmt2);
    }
    return cache.getAffectsTN(type1, stmt2);
}

bool PKB::getAffectsTTBoolean(StatementTypes type1, StatementTypes type2) {
    // Affects used as a boolean clause, does not need exact results
    if (!cache.getAffectsTT(type1, type2).empty()) {
        return true;
    }
    DesignExtractor::setAffectsTTBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getAffectsTT(type1, type2);
    return !results.empty();
}

bool PKB::getAffectsNTBoolean(int stmt1, StatementTypes type2) {
    if (!cache.getAffectsNT(stmt1, type2).empty()) {
        return true;
    }
    DesignExtractor::setAffectsNTBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getAffectsNT(stmt1, type2);
    return !results.empty();
}

bool PKB::getAffectsTNBoolean(StatementTypes type1, int stmt2) {
    if (!cache.getAffectsTN(type1, stmt2).empty()) {
        return true;
    }
    DesignExtractor::setAffectsTNBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getAffectsTN(type1, stmt2);
    return !results.empty();
}

/////////////////////// AFFECTSSTAR ///////////////////////

bool PKB::addAffectsStar(int stmt1, int stmt2){
    return cache.addAffectsStar(stmt1, getStatementType(stmt1), stmt2, getStatementType(stmt2));
}

LIST_OF_INT_PAIRS PKB::getAffectsStarNN(int stmt1, int stmt2) {
    if (!cache.isAffectsStarNNCalled(stmt1, stmt2)) {
        cache.addAffectsStarNNCalled(stmt1, stmt2);
        return DesignExtractor::getAffectsStarNN(stmt1, stmt2);;
    }
    return cache.getAffectsStarNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getAffectsStarTT(StatementTypes type1, StatementTypes type2) {
    if (!cache.isAffectsStarTTCalled(type1,type2)) {
        cache.addAffectsStarTTCalled(type1,type2);
        return DesignExtractor::getAffectsStarTT(type1, type2);
    }
    return cache.getAffectsStarTT(type1, type2);
}

LIST_OF_INT_PAIRS PKB::getAffectsStarNT(int stmt1, StatementTypes type2) {
    if (!cache.isAffectsStarNTCalled(stmt1, type2)) {
        cache.addAffectsStarNTCalled(stmt1, type2);
        return DesignExtractor::getAffectsStarNT(stmt1, type2);
    }
    return cache.getAffectsStarNT(stmt1, type2);
}

LIST_OF_INT_PAIRS PKB::getAffectsStarTN(StatementTypes type1, int stmt2) {
    if (!cache.isAffectsStarTNCalled(type1, stmt2)) {
        cache.addAffectsStarTNCalled(type1, stmt2);
        return DesignExtractor::getAffectsStarTN(type1, stmt2);
    }
    return cache.getAffectsStarTN(type1, stmt2);
}

bool PKB::getAffectsStarTTBoolean(StatementTypes type1, StatementTypes type2) {
    // Affects* used as a boolean clause, does not need exact results
    if (!cache.getAffectsStarTT(type1, type2).empty()) {
        return true;
    }
    DesignExtractor::setAffectsStarTTBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getAffectsStarTT(type1, type2);
    return !results.empty();
}

bool PKB::getAffectsStarNTBoolean(int stmt1, StatementTypes type2) {
    if (!cache.getAffectsStarNT(stmt1, type2).empty()) {
        return true;
    }
    DesignExtractor::setAffectsStarNTBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getAffectsStarNT(stmt1, type2);
    return !results.empty();
}

bool PKB::getAffectsStarTNBoolean(StatementTypes type1, int stmt2) {
    if (!cache.getAffectsStarTN(type1, stmt2).empty()) {
        return true;
    }
    DesignExtractor::setAffectsStarTNBoolean(true);
    vector<pair<int, int>> results = DesignExtractor::getAffectsStarTN(type1, stmt2);
    return !results.empty();
}

/////////////////////// AFFECTSBIP ///////////////////////

bool PKB::addAffectsBip(int stmt1, int stmt2) {
    return true;
}

LIST_OF_INT_PAIRS PKB::getAffectsBipNN(int stmt1, int stmt2) {
    return DesignExtractor::getAffectsBipNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getAffectsBipNT(int stmt1,StatementTypes type2) {
    return DesignExtractor::getAffectsBipNT(stmt1, type2);
}

LIST_OF_INT_PAIRS PKB::getAffectsBipTN(StatementTypes type1, int stmt2) {
    return DesignExtractor::getAffectsBipTN(type1, stmt2);
}

LIST_OF_INT_PAIRS PKB::getAffectsBipTT(StatementTypes type1, StatementTypes type2) {
    return DesignExtractor::getAffectsBipTT(type1, type2);
}

/////////////// CLEAR ///////////////

void PKB::clear() {
    store.clear();
}

void PKB::clearCache() {
    cache.clear();
}
