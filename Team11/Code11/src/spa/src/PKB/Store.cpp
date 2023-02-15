//
// Created by Yen Peng Yip on 21/10/21.
//
#pragma once
#include <unordered_set>
#include <stdexcept>
#include "Store.h"
#include "Hasher.h"

using namespace std;

FollowsStore Store::followsStore;
ParentStore Store::parentStore;
ModifiesStore Store::modifiesStore;
UsesStore Store::usesStore;
CallsStore Store::callsStore;
NextStore Store::nextStore;
NextBipStore Store::nextBipStore;

bool Store::addStmt(int stmtNum, StatementTypes type, std::string procName) {
    auto result = _statementsTable->find(stmtNum);
    if (result == _statementsTable->end()) {
        _totalNumOfStatements++;
        _statementsTable->insert({stmtNum, type});
        if (!procStmtMap->emplace(procName, std::vector<int>{stmtNum}).second) {
            procStmtMap->at(procName).push_back(stmtNum);
        }
        return true;
    } else {
        // already in table
        return false;
    }
}

bool Store::setStatementType(int statementNumber, StatementTypes stmtType) {
    auto result = _statementsTable->find(statementNumber);
    if (result == _statementsTable->end()) {
        _totalNumOfStatements++;
        _statementsTable->insert({statementNumber, stmtType});
        return true;
    } else {
        // already in tables
        return false;
    }
}

bool Store::setNum(int num) {
    _totalNumOfStatements = num;
    return true;
}

int Store::getTotalNumberOfStatements() {
    return _totalNumOfStatements;
}

StatementTypes Store::getStatementType(int statementNumber) {
    auto result = _statementsTable->find(statementNumber);
    if (result != _statementsTable->end()) {
        return result->second;
    } else {
        return StatementTypes::NO_TYPE;
    }
}

bool Store::addIfStmtCV(int ifStmtNum, string var) {
    if (ifStmtCVMap->find(var) != ifStmtCVMap->end()) {
        ifStmtCVMap->at(var).emplace(ifStmtNum);
    } else {
        ifStmtCVMap->emplace(var, std::unordered_set<int>{ifStmtNum});
    }
    if (mIfStmtCVMap->find(ifStmtNum) != mIfStmtCVMap->end()) {
        mIfStmtCVMap->at(ifStmtNum).emplace(var);
    } else {
        mIfStmtCVMap->emplace(ifStmtNum, std::unordered_set<string>{var});
    }
    return true;
}

bool Store::addWhileStmtCV(int whileStmtNum, string var) {
    if (whileStmtCVMap->find(var) != whileStmtCVMap->end()) {
        whileStmtCVMap->at(var).emplace(whileStmtNum);
    } else {
        whileStmtCVMap->emplace(var, std::unordered_set<int>{whileStmtNum});
    }
    if (mWhileStmtCVMap->find(whileStmtNum) != mWhileStmtCVMap->end()) {
        mWhileStmtCVMap->at(whileStmtNum).emplace(var);
    } else {
        mWhileStmtCVMap->emplace(whileStmtNum, std::unordered_set<string>{var});
    }
    return true;
}

unordered_set<int> Store::getIfStmtsWithCV(string var) {
    if (ifStmtCVMap->find(var) != ifStmtCVMap->end()) {
        return ifStmtCVMap->at(var);
    }
    return {};
}

unordered_set<string> Store::getAllCVInIfStmt(int stmtNum) {
    if (mIfStmtCVMap->find(stmtNum) != mIfStmtCVMap->end()) {
        return mIfStmtCVMap->at(stmtNum);
    }
    return {};
}

unordered_set<int> Store::getWhileStmtsWithCV(string var) {
    if (whileStmtCVMap->find(var) != whileStmtCVMap->end()) {
        return whileStmtCVMap->at(var);
    }
    return {};
}

unordered_set<string> Store::getAllCVInWhileStmt(int stmtNum) {
    if (mWhileStmtCVMap->find(stmtNum) != mWhileStmtCVMap->end()) {
        return mWhileStmtCVMap->at(stmtNum);
    }
    return {};
}

bool Store::addAssignLHSVar(int stmtNum, string var) {
    assignList->emplace(stmtNum);
    assignLHSMap->emplace(stmtNum, var);
    mAssignLHSMap->emplace(var, stmtNum);
    return true;
}

bool Store::addAssignRHSVar(int stmtNum, string var) {
    unordered_set<string> copyTable{getAssignRHSVars(stmtNum)};

    if (copyTable.find(var) == copyTable.end()) {
        vector<string> temp = getAssignRHSVarsVector(stmtNum);
        temp.push_back(var);
        assignRHSMapVector->insert_or_assign(stmtNum, temp);
    }
    if (!assignRHSMap->emplace(stmtNum, std::unordered_set<string>{var}).second) {
        assignRHSMap->at(stmtNum).emplace(var);
    }
    return true;
}

bool Store::addAssignRHSConst(int stmtNum, int constant) {
    unordered_set<int> copyTable{getAssignRHSConst(stmtNum)};
    if (copyTable.find(constant) == copyTable.end()) {
        vector<int> temp = getAssignRHSConstVector(stmtNum);
        temp.push_back(constant);
        assignRHSConstMapVector->emplace(stmtNum, temp);
    }
    if (!assignConstMap->emplace(stmtNum, std::unordered_set<int>{constant}).second) {
        assignConstMap->at(stmtNum).emplace(constant);
    }
    return true;
}

bool Store::addAssignPattern(int stmtNum, string pattern) {
    assignPatternMap->emplace(stmtNum, pattern);
    return true;
}

string Store::getAssignPattern(int stmtNum) {
    if (assignPatternMap->find(stmtNum) != assignPatternMap->end()) {
        return assignPatternMap->at(stmtNum);
    }
    return "";
}

bool Store::addAssignSubExp(int stmtNum, string subExp) {
    if (!assignSubExpMap->emplace(subExp, std::unordered_set<int>{stmtNum}).second) {
        assignSubExpMap->at(subExp).emplace(stmtNum);
        return true;
    } else {
        assignSubExpMap->emplace(subExp, std::unordered_set<int>{stmtNum});
        return true;
    }
}

unordered_set<int> Store::getSubExpStmtNums(string subExp) {
    if (assignSubExpMap->find(subExp) != assignSubExpMap->end()) {
        return assignSubExpMap->at(subExp);
    }
    return {};
}

std::string Store::getAssignLHSVar(int stmtNum) {
    if (assignLHSMap->find(stmtNum) != assignLHSMap->end()) {
        return assignLHSMap->at(stmtNum);
    }
    return "";
}

unordered_set<int> Store::getAssignRHSConst(int stmtNum) {
    if (assignConstMap->find(stmtNum) != assignConstMap->end()) {
        return assignConstMap->at(stmtNum);
    }
    return {};
}


std::unordered_set<string> Store::getAssignRHSVars(int stmtNum) {
    if (assignRHSMap->find(stmtNum) != assignRHSMap->end()) {
        return assignRHSMap->at(stmtNum);
    }
    return {};
}

std::unordered_set<int> *Store::getAssignList() {
    return assignList;
}

std::vector<string> Store::getAssignRHSVarsVector(int stmtNum) {
    if (assignRHSMapVector->find(stmtNum) != assignRHSMapVector->end()) {
        return assignRHSMapVector->at(stmtNum);
    }
    return {};
}

std::vector<int> Store::getAssignRHSConstVector(int stmtNum) {
    if (assignRHSConstMapVector->find(stmtNum) != assignRHSConstMapVector->end()) {
        return assignRHSConstMapVector->at(stmtNum);
    }
    return {};
}

/////////////////////// READ ///////////////////////

bool Store::addReadStmt(int stmtNum, string var) {
    // if already added, return false
    if (!readStmtList->emplace(stmtNum).second) {
        return false;
    }
    readStmtList->emplace(stmtNum);
    readMap->emplace(stmtNum, var);
    return true;
}

std::unordered_set<int> *Store::getReadStmtList() {
    return readStmtList;
}

std::string Store::getReadVar(int stmtNum) {
    if (readMap->find(stmtNum) != readMap->end()) {
        return readMap->at(stmtNum);
    }
    return "";
}

/////////////////////// PRINT ///////////////////////

bool Store::addPrintStmt(int stmtNum, string var) {
    // if already added, return false
    if (!printStmtList->emplace(stmtNum).second)
    {
        return false;
    }
    printStmtList->emplace(stmtNum);
    printMap->emplace(stmtNum, var);
    return true;
}

std::unordered_set<int> *Store::getPrintStmtList() {
    return printStmtList;
}

std::string Store::getPrintVar(int stmtNum) {
    if (printMap->find(stmtNum) != printMap->end()) {
        return printMap->at(stmtNum);
    }
    return "";
}

/////////////////////// IF ///////////////////////

bool Store::addIfStmt(int stmtNum) {
    ifList->insert(stmtNum);
    return ifList->count(stmtNum);
}

std::unordered_set<int> *Store::getIfList() {
    return ifList;
}

bool Store::addIfBlockStmt(int ifStmtNum, int blockStmtNum) {
    if (ifBlockList->find(ifStmtNum) != ifBlockList->end()) {
        ifBlockList->at(ifStmtNum).push_back(blockStmtNum);
        return true;
    } else {
        std::vector<int> block;
        block.push_back(blockStmtNum);
        ifBlockList->insert(std::make_pair(ifStmtNum, block));
        return true;
    }
}

std::vector<int> Store::getIfBlockList(int ifStmtNum) {
    if (ifBlockList->find(ifStmtNum) != ifBlockList->end()) {
        return ifBlockList->at(ifStmtNum);
    }
    return {};
}

bool Store::addElseBlockStmt(int ifStmtNum, int blockStmtNum) {
    if (elseBlockList->find(ifStmtNum) != elseBlockList->end()) {
        elseBlockList->at(ifStmtNum).push_back(blockStmtNum);
        return true;
    } else {
        std::vector<int> block;
        block.push_back(blockStmtNum);
        elseBlockList->insert(std::make_pair(ifStmtNum, block));
        return true;
    }
}

std::vector<int> Store::getElseBlockList(int ifStmtNum) {
    if (elseBlockList->find(ifStmtNum) != elseBlockList->end()) {
        return elseBlockList->at(ifStmtNum);
    }
    return {};
}

/////////////////////// WHILE ///////////////////////

bool Store::addWhileStmt(int stmtNum) {
    whileList->insert(stmtNum);
    return whileList->count(stmtNum);
}

std::unordered_set<int> *Store::getWhileList() {
    return whileList;
}

bool Store::addWhileBlockStmt(int whileStmtNum, int blockStmtNum) {
    if (whileBlockList->find(whileStmtNum) != whileBlockList->end()) {
        whileBlockList->at(whileStmtNum).push_back(blockStmtNum);
        return true;
    } else {
        std::vector<int> block;
        block.push_back(blockStmtNum);
        whileBlockList->insert(std::make_pair(whileStmtNum, block));
        return true;
    }
}

std::vector<int> Store::getWhileBlockList(int whileStmtNum) {
    if (whileBlockList->find(whileStmtNum) != whileBlockList->end()) {
        return whileBlockList->at(whileStmtNum);
    }
    return {};
}

/////////////////////// PROCEDURE ///////////////////////

bool Store::addProcedure(std::string procName) {
    if (procedureList->find(procName) == procedureList->end()) {
        procedureList->insert(procName);
        procTable->emplace(procedureList->size(), procName);
        mProcTable->emplace(procName, procedureList->size());
        return true;
    }
    return false;
}

int Store::getProcDigit(string procName) {
    if (mProcTable->find(procName) != mProcTable->end()) {
        return mProcTable->at(procName);
    }
    return -1;
}

string Store::getProcName(int procDigit) {
    if (procTable->find(procDigit) != procTable->end()) {
        return procTable->at(procDigit);
    }
    return "";
}

std::unordered_set<std::string> *Store::getProcList() {
    return procedureList;
}

std::vector<int> Store::getProcedureStmtNums(string procName) {
    if (procStmtMap->find(procName) != procStmtMap->end()) {
        return procStmtMap->at(procName);
    }
    return {};
}

/////////////////////// VARIABLE ///////////////////////

bool Store::addVar(string var) {
    return varList->emplace(var).second;
}

int Store::getTotalNumOfVars() {
    return varList->size();
}

unordered_set<string> *Store::getAllVars() {
    return varList;
}

/////////////////////// CONSTANT ///////////////////////

bool Store::addConst(int constant) {
    return constList->emplace(constant).second;
}

int Store::getTotalNumOfConsts() {
    return constList->size();
}

unordered_set<int> *Store::getAllConsts() {
    return constList;
}

///////////////////////// FOLLOW ///////////////////////

bool Store::addFollows(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return followsStore.add(stmt1, type1, stmt2, type2);
}

vector<std::pair<int, int> > Store::getFollowsNN(int stmt1, int stmt2) {
    return followsStore.getNN(stmt1, stmt2);
}

vector<std::pair<int, int> > Store::getFollowsTT(StatementTypes type1, StatementTypes type2) {
    return followsStore.getTT(type1, type2);
}

vector<std::pair<int, int> > Store::getFollowsNT(int stmt1, StatementTypes type2) {
    return followsStore.getNT(stmt1, type2);
}

vector<std::pair<int, int> > Store::getFollowsTN(StatementTypes type1, int stmt2) {
    return followsStore.getTN(type1, stmt2);
}

///////////////////////// FOLLOW STAR ///////////////////////

bool Store::addFollowsStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return followsStore.addStar(stmt1, type1, stmt2, type2);
}

vector<pair<int, int> > Store::getFollowsStarNN(int stmt1, int stmt2) {
    return followsStore.getStarNN(stmt1, stmt2);
}

vector<pair<int, int> > Store::getFollowsStarTT(StatementTypes type1, StatementTypes type2) {
    return followsStore.getStarTT(type1, type2);
}

vector<pair<int, int> > Store::getFollowsStarNT(int stmt1, StatementTypes type2) {
    return followsStore.getStarNT(stmt1, type2);
}

vector<pair<int, int> > Store::getFollowsStarTN(StatementTypes type1, int stmt2) {
    return followsStore.getStarTN(type1, stmt2);
}

/////////////////////// PARENT ///////////////////////

bool Store::addParent(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return parentStore.add(stmt1, type1, stmt2, type2);
}

vector<std::pair<int, int> > Store::getParentNN(int stmt1, int stmt2) {
    return parentStore.getNN(stmt1, stmt2);
}

vector<std::pair<int, int> > Store::getParentTT(StatementTypes type1, StatementTypes type2) {
    return parentStore.getTT(type1, type2);
}

vector<std::pair<int, int> > Store::getParentNT(int stmt1, StatementTypes type2) {
    return parentStore.getNT(stmt1, type2);
}

vector<std::pair<int, int> > Store::getParentTN(StatementTypes type1, int stmt2) {
    return parentStore.getTN(type1, stmt2);
}

/////////////////////// PARENT STAR ///////////////////////

bool Store::addParentStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return parentStore.addStar(stmt1, type1, stmt2, type2);
}

vector<std::pair<int, int> > Store::getParentStarNN(int stmt1, int stmt2) {
    return parentStore.getStarNN(stmt1, stmt2);
}

vector<std::pair<int, int> > Store::getParentStarTT(StatementTypes type1, StatementTypes type2) {
    return parentStore.getStarTT(type1, type2);
}

vector<std::pair<int, int> > Store::getParentStarNT(int stmt1, StatementTypes type2) {
    return parentStore.getStarNT(stmt1, type2);
}

vector<std::pair<int, int> > Store::getParentStarTN(StatementTypes type1, int stmt2) {
    return parentStore.getStarTN(type1, stmt2);
}

/////////////// USES STATEMENT API ///////////////

bool Store::addUsesS(int stmtNum, StatementTypes type, std::string varUsed) {
    return usesStore.addS(stmtNum, type, varUsed);
}

vector<std::pair<int, string> > Store::getUsesSSV(int stmt1, string var2) {
    return usesStore.getSSV(stmt1, var2);
}

vector<std::pair<int, string> > Store::getUsesSTT(StatementTypes type1, StatementTypes type2) {
    return usesStore.getSTT(type1, type2);
}

vector<std::pair<int, string> > Store::getUsesSST(int stmt1, StatementTypes type2) {
    return usesStore.getSST(stmt1, type2);
}

vector<std::pair<int, string> > Store::getUsesSTV(StatementTypes type1, string var2) {
    return usesStore.getSTV(type1, var2);
}

/////////////// USES PROCEDURE API ///////////////

bool Store::addUsesP(string procName, StatementTypes type, std::string varUsed) {
    return usesStore.addP(procName, type, varUsed);
}

vector<std::pair<string, string> > Store::getUsesPPV(string proc1, string var2) {
    return usesStore.getPPV(proc1, var2);
}

vector<std::pair<string, string> > Store::getUsesPTT(StatementTypes type1, StatementTypes type2) {
    return usesStore.getPTT(type1, type2);
}

vector<std::pair<string, string> > Store::getUsesPPT(string proc1, StatementTypes type2) {
    return usesStore.getPPT(proc1, type2);
}

vector<std::pair<string, string> > Store::getUsesPTV(StatementTypes type1, string var2) {
    return usesStore.getPTV(type1, var2);
}

/////////////// MODIFIES STATEMENT API ///////////////

bool Store::addModifiesS(int stmtNum, StatementTypes type, std::string varModified) {
    return modifiesStore.addS(stmtNum, type, varModified);
}

vector<std::pair<int, string> > Store::getModifiesSSV(int stmt1, string var2) {
    return modifiesStore.getSSV(stmt1, var2);
}

vector<std::pair<int, string> > Store::getModifiesSTT(StatementTypes type1, StatementTypes type2) {
    return modifiesStore.getSTT(type1, type2);
}

vector<std::pair<int, string> > Store::getModifiesSST(int stmt1, StatementTypes type2) {
    return modifiesStore.getSST(stmt1, type2);
}

vector<std::pair<int, string> > Store::getModifiesSTV(StatementTypes type1, string var2) {
    return modifiesStore.getSTV(type1, var2);
}

/////////////// MODIFIES PROCEDURE API ///////////////

bool Store::addModifiesP(string procName, StatementTypes type, std::string varModified) {
    return modifiesStore.addP(procName, type, varModified);
}

vector<std::pair<string, string> > Store::getModifiesPPV(string proc1, string var2) {
    return modifiesStore.getPPV(proc1, var2);
}

vector<std::pair<string, string> > Store::getModifiesPTT(StatementTypes type1, StatementTypes type2) {
    return modifiesStore.getPTT(type1, type2);
}

vector<std::pair<string, string> > Store::getModifiesPPT(string proc1, StatementTypes type2) {
    return modifiesStore.getPPT(proc1, type2);
}

vector<std::pair<string, string> > Store::getModifiesPTV(StatementTypes type1, string var2) {
    return modifiesStore.getPTV(type1, var2);
}

/////////////////////// CALL API ///////////////////////

bool Store::addCalls(string procName, StatementTypes type, string procCalled, int stmtNum) {
    if (!callStmtList->emplace(stmtNum).second) {
        return false;
    }
    callStmtList->emplace(stmtNum);
    return callsStore.addS(stmtNum, type, procCalled)
           && callsStore.addP(procName, type, procCalled);
}

vector<std::pair<int, string> > Store::getCallsSSP(int stmtNum, string procCalled) {
    return callsStore.getSSP(stmtNum, procCalled);
}

vector<std::pair<int, string> > Store::getCallsSTT(StatementTypes type1, StatementTypes type2) {
    return callsStore.getSTT(type1, type2);
}

vector<std::pair<int, string> > Store::getCallsSST(int stmtNum, StatementTypes type2) {
    return callsStore.getSST(stmtNum, type2);
}

vector<std::pair<int, string> > Store::getCallsSTP(StatementTypes type1, string procCalled) {
    return callsStore.getSTP(type1, procCalled);
}

vector<std::pair<string, string> > Store::getCallsPPP(string procName, string procCalled) {
    return callsStore.getPPP(procName, procCalled);
}

vector<std::pair<string, string> > Store::getCallsPTT(StatementTypes type1, StatementTypes type2) {
    return callsStore.getPTT(type1, type2);
}

vector<std::pair<string, string> > Store::getCallsPPT(string procName, StatementTypes type2) {
    return callsStore.getPPT(procName, type2);
}

vector<std::pair<string, string> > Store::getCallsPTP(StatementTypes type1, string procCalled) {
    return callsStore.getPTP(type1, procCalled);
}

std::unordered_set<int> *Store::getCallStmtList() {
    return callStmtList;
}

/////////////////////// CALL STAR API ///////////////////////

bool Store::addCallsStar(string procName, string procCalled) {
    return callsStore.addStar(procName, procCalled);
}

vector<std::pair<string, string> > Store::getCallsStarNN(string procName, string procCalled) {
    return callsStore.getStarNN(procName, procCalled);
}

vector<std::pair<string, string> > Store::getCallsStarTT(StatementTypes type1, StatementTypes type2) {
    return callsStore.getStarTT(type1, type2);
}

vector<std::pair<string, string> > Store::getCallsStarNT(string procName, StatementTypes type2) {
    return callsStore.getStarNT(procName, type2);
}

vector<std::pair<string, string> > Store::getCallsStarTN(StatementTypes type1, string procCalled) {
    return callsStore.getStarTN(type1, procCalled);
}

/////////////////////// NEXT API ///////////////////////

bool Store::addNext(int stmt1, int stmt2) {
    return nextStore.add(stmt1, getStatementType(stmt1), stmt2, getStatementType(stmt2));
}

vector<std::pair<int, int> > Store::getNextNN(int stmt1, int stmt2) {
    return nextStore.getNN(stmt1, stmt2);
}

vector<std::pair<int, int> > Store::getNextTT(StatementTypes type1, StatementTypes type2) {
    return nextStore.getTT(type1, type2);
}

vector<std::pair<int, int> > Store::getNextNT(int stmt1, StatementTypes type2) {
    return nextStore.getNT(stmt1, type2);
}

vector<std::pair<int, int> > Store::getNextTN(StatementTypes type1, int stmt2) {
    return nextStore.getTN(type1, stmt2);
}

/////////////////////// NEXTBIP API ///////////////////////

bool Store::getIsNextBipComputed() {
    return NextBipStore::getIsNextBipComputed();
}

bool Store::setNextBipComputed() {
    return NextBipStore::setNextBipComputed();
}

bool Store::addNextBip(int stmt1, int stmt2) {
    return nextBipStore.add(stmt1, getStatementType(stmt1), stmt2, getStatementType(stmt2));
}

vector<std::pair<int, int> > Store::getNextBipNN(int stmt1, int stmt2) {
    return nextBipStore.getNN(stmt1, stmt2);
}

vector<std::pair<int, int> > Store::getNextBipTT(StatementTypes type1, StatementTypes type2) {
    return nextBipStore.getTT(type1, type2);
}

vector<std::pair<int, int> > Store::getNextBipNT(int stmt1, StatementTypes type2) {
    return nextBipStore.getNT(stmt1, type2);
}

vector<std::pair<int, int> > Store::getNextBipTN(StatementTypes type1, int stmt2) {
    return nextBipStore.getTN(type1, stmt2);
}

/////////////// CLEAR ///////////////
void Store::clear() {
    _totalNumOfStatements = 0;
    _statementsTable->clear();
    ifStmtCVMap->clear();
    mIfStmtCVMap->clear();
    whileStmtCVMap->clear();
    mWhileStmtCVMap->clear();
    mAssignLHSMap->clear();
    assignLHSMap->clear();
    assignRHSMap->clear();
    assignList->clear();

    assignPatternMap->clear();
    assignSubExpMap->clear();

    assignRHSConstMapVector->clear();
    assignRHSMapVector->clear();
    assignConstMap->clear();

    ifList->clear();
    ifBlockList->clear();
    elseBlockList->clear();
    whileList->clear();
    whileBlockList->clear();

    procStmtMap->clear();
    procedureList->clear();
    procTable->clear();
    mProcTable->clear();
    varList->clear();
    constList->clear();

    readMap->clear();
    readStmtList->clear();

    printMap->clear();
    printStmtList->clear();
    callStmtList->clear();

    followsStore.clear();
    parentStore.clear();
    modifiesStore.clear();
    usesStore.clear();
    callsStore.clear();
    nextStore.clear();
}
