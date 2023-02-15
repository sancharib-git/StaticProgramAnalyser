//
// Created by Yen Peng Yip on 21/10/21.
//

#ifndef AUTOTESTER_STORE_H
#define AUTOTESTER_STORE_H
#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "StatementTypes.h"
#include "FollowsStore.h"
#include "ModifiesStore.h"
#include "ParentStore.h"
#include "UsesStore.h"
#include "CallsStore.h"
#include "NextStore.h"
#include "NextBipStore.h"
#include "Definitions.h"
#include "Hasher.h"

using namespace std;

class Store {

private:
    int _totalNumOfStatements;

    UNORDERED_LIST_OF_INT *assignList;
    UNORDERED_LIST_OF_INT *ifList;
    UNORDERED_LIST_OF_INT *constList;
    UNORDERED_LIST_OF_INT *readStmtList;
    UNORDERED_LIST_OF_INT *printStmtList;
    UNORDERED_LIST_OF_INT *callStmtList;
    UNORDERED_LIST_OF_INT *whileList;
    UNORDERED_LIST_OF_STR *varList;
    UNORDERED_LIST_OF_STR *procedureList;

    std::unordered_map<int, string> *assignPatternMap;
    std::unordered_map<int, string> *assignLHSMap;
    std::unordered_map<int, string> *procTable;
    std::unordered_map<int, string> *readMap;
    std::unordered_map<int, string> *printMap;

    std::unordered_map<string, int> *mAssignLHSMap;
    std::unordered_map<string, int> *mProcTable;

    std::unordered_map<int, StatementTypes> *_statementsTable;
    std::unordered_map<int, UNORDERED_LIST_OF_STR> *assignRHSMap;
    std::unordered_map<int, UNORDERED_LIST_OF_INT> *assignConstMap;
    std::unordered_map<int, LIST_OF_STR> *assignRHSMapVector;
    std::unordered_map<int, LIST_OF_INT> *assignRHSConstMapVector;
    std::unordered_map<int, LIST_OF_INT> *ifBlockList;
    std::unordered_map<int, LIST_OF_INT> *elseBlockList;
    std::unordered_map<int, LIST_OF_INT> *whileBlockList;

    std::unordered_map<string, LIST_OF_INT> *procStmtMap;
    std::unordered_map<string, UNORDERED_LIST_OF_INT> *ifStmtCVMap;
    std::unordered_map<string, UNORDERED_LIST_OF_INT> *whileStmtCVMap;
    std::unordered_map<int, UNORDERED_LIST_OF_STR> *mIfStmtCVMap;
    std::unordered_map<int, UNORDERED_LIST_OF_STR> *mWhileStmtCVMap;
    std::unordered_map<string, UNORDERED_LIST_OF_INT> *assignSubExpMap;

    static bool isNextBipComputed;
    static FollowsStore followsStore;
    static ParentStore parentStore;
    static ModifiesStore modifiesStore;
    static UsesStore usesStore;
    static CallsStore callsStore;
    static NextStore nextStore;
    static NextBipStore nextBipStore;

public:
    Store() {
        _totalNumOfStatements = 0;
        callStmtList = new std::unordered_set<int>();
        _statementsTable = new unordered_map<int, StatementTypes>();
        procStmtMap = new std::unordered_map<string, std::vector<int> >();
        ifStmtCVMap = new unordered_map<string, unordered_set<int>>();
        whileStmtCVMap = new unordered_map<string, unordered_set<int>>();
        mIfStmtCVMap = new std::unordered_map<int, UNORDERED_LIST_OF_STR>();
        mWhileStmtCVMap = new std::unordered_map<int, UNORDERED_LIST_OF_STR>();
        mAssignLHSMap = new unordered_map<string, int>();
        assignLHSMap = new unordered_map<int, string>();
        assignRHSMap = new unordered_map<int, std::unordered_set<string>>();
        assignRHSMapVector = new unordered_map<int, vector<string>>();
        assignRHSConstMapVector = new unordered_map<int, vector<int>>();
        assignPatternMap = new std::unordered_map<int, string>();
        assignSubExpMap = new std::unordered_map<string, unordered_set<int>>();
        assignConstMap = new unordered_map<int, std::unordered_set<int>>();
        assignList = new std::unordered_set<int>();
        ifList = new std::unordered_set<int>();
        ifBlockList = new std::unordered_map<int, std::vector<int> >();
        elseBlockList = new std::unordered_map<int, std::vector<int> >();
        whileList = new std::unordered_set<int>();
        whileBlockList = new std::unordered_map<int, std::vector<int> >();
        procedureList = new std::unordered_set<std::string>();
        procTable = new std::unordered_map<int, string>();
        mProcTable = new std::unordered_map<string, int>();
        varList = new std::unordered_set<std::string>();
        constList = new std::unordered_set<int>();
        readMap = new std::unordered_map<int, string>();
        readStmtList = new std::unordered_set<int>();
        printMap = new std::unordered_map<int, string>();
        printStmtList = new std::unordered_set<int>();
    };

    bool addStmt(int stmtNum, StatementTypes type, std::string procName);

    StatementTypes getStatementType(int statementNumber);

    bool setStatementType(int statementNumber, StatementTypes stmtType);

    bool setNum(int num);

    int getTotalNumberOfStatements();

    bool addIfStmtCV(int ifStmtNum, string var);

    bool addWhileStmtCV(int whileStmtNum, string var);

    UNORDERED_LIST_OF_INT getIfStmtsWithCV(string var);

    UNORDERED_LIST_OF_STR getAllCVInIfStmt(int stmtNum);

    UNORDERED_LIST_OF_INT getWhileStmtsWithCV(string var);

    UNORDERED_LIST_OF_STR getAllCVInWhileStmt(int stmtNum);

    bool addAssignLHSVar(int stmtNum, string var);

    bool addAssignRHSVar(int stmtNum, string var);

    bool addAssignRHSConst(int stmtNum, int constant);

    bool addAssignPattern(int stmtNum, string pattern);

    string getAssignPattern(int stmtNum);

    bool addAssignSubExp(int stmtNum, string subExp);

    UNORDERED_LIST_OF_INT getSubExpStmtNums(string subExp);

    std::string getAssignLHSVar(int stmtNum);

    UNORDERED_LIST_OF_INT getAssignRHSConst(int stmtNum);

    UNORDERED_LIST_OF_STR getAssignRHSVars(int stmtNum);

    UNORDERED_LIST_OF_INT *getAssignList();

    LIST_OF_STR getAssignRHSVarsVector(int stmtNum);

    LIST_OF_INT getAssignRHSConstVector(int stmtNum);

    bool addReadStmt(int stmtNum, string var);

    UNORDERED_LIST_OF_INT *getReadStmtList();

    std::string getReadVar(int stmtNum);

    bool addPrintStmt(int stmtNum, string var);

    UNORDERED_LIST_OF_INT *getPrintStmtList();

    std::string getPrintVar(int stmtNum);

    bool addIfStmt(int stmtNum);

    UNORDERED_LIST_OF_INT *getIfList();

    bool addIfBlockStmt(int ifStmtNum, int blockStmtNum);

    LIST_OF_INT getIfBlockList(int ifStmtNum);

    bool addElseBlockStmt(int ifStmtNum, int blockStmtNum);

    LIST_OF_INT getElseBlockList(int ifStmtNum);

    bool addWhileStmt(int stmtNum);

    UNORDERED_LIST_OF_INT *getWhileList();

    bool addWhileBlockStmt(int whileStmtNum, int blockStmtNum);

    LIST_OF_INT getWhileBlockList(int whileStmtNum);

    bool addProcedure(std::string procName);

    int getProcDigit(string procName);

    string getProcName(int procDigit);

    std::unordered_set<std::string> *getProcList();

    LIST_OF_INT getProcedureStmtNums(string procName);

    bool addVar(string var);

    int getTotalNumOfVars();

    UNORDERED_LIST_OF_STR *getAllVars();

    bool addConst(int constant);

    int getTotalNumOfConsts();

    UNORDERED_LIST_OF_INT *getAllConsts();

    /////////////////////// FOLLOWS ///////////////////////

    bool addFollows(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2);

    LIST_OF_INT_PAIRS getFollowsNN(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getFollowsTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_INT_PAIRS getFollowsNT(int stmt1, StatementTypes type2);

    LIST_OF_INT_PAIRS getFollowsTN(StatementTypes type1, int stmt2);

    /////////////////////// FOLLOWS STAR ///////////////////////

    bool addFollowsStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2);

    LIST_OF_INT_PAIRS getFollowsStarNN(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getFollowsStarTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_INT_PAIRS getFollowsStarNT(int stmt1, StatementTypes type2);

    LIST_OF_INT_PAIRS getFollowsStarTN(StatementTypes type1, int stmt2);

    /////////////////////// PARENT ///////////////////////

    bool addParent(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2);

    LIST_OF_INT_PAIRS getParentNN(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getParentTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_INT_PAIRS getParentNT(int stmt1, StatementTypes type2);

    LIST_OF_INT_PAIRS getParentTN(StatementTypes type1, int stmt2);

    /////////////////////// PARENT STAR ///////////////////////

    bool addParentStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2);

    LIST_OF_INT_PAIRS getParentStarNN(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getParentStarTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_INT_PAIRS getParentStarNT(int stmt1, StatementTypes type2);

    LIST_OF_INT_PAIRS getParentStarTN(StatementTypes type1, int stmt2);

    /////////////////////// USES ///////////////////////

    bool addUsesS(int stmtNum, StatementTypes type, std::string varUsed);

    bool addUsesP(string procName, StatementTypes type, std::string varUsed);

    LIST_OF_INTSTR_PAIRS getUsesSSV(int stmt, string var);

    LIST_OF_INTSTR_PAIRS getUsesSTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_INTSTR_PAIRS getUsesSST(int stmt, StatementTypes type);

    LIST_OF_INTSTR_PAIRS getUsesSTV(StatementTypes type, string var);

    LIST_OF_STR_PAIRS getUsesPPV(string procName, string var);

    LIST_OF_STR_PAIRS getUsesPTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_STR_PAIRS getUsesPPT(string procName, StatementTypes type2);

    LIST_OF_STR_PAIRS getUsesPTV(StatementTypes type1, string var);

    /////////////////////// MODIFIES ///////////////////////

    bool addModifiesS(int stmtNum, StatementTypes type, std::string varModified);

    bool addModifiesP(string procName, StatementTypes type, std::string varModified);

    LIST_OF_INTSTR_PAIRS getModifiesSSV(int stmt1, string var);

    LIST_OF_INTSTR_PAIRS getModifiesSTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_INTSTR_PAIRS getModifiesSST(int stmt1, StatementTypes type2);

    LIST_OF_INTSTR_PAIRS getModifiesSTV(StatementTypes type1, string var);

    LIST_OF_STR_PAIRS getModifiesPPV(string procName, string var);

    LIST_OF_STR_PAIRS getModifiesPTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_STR_PAIRS getModifiesPPT(string procName, StatementTypes type2);

    LIST_OF_STR_PAIRS getModifiesPTV(StatementTypes type1, string var);

    /////////////////////// CALLS ///////////////////////

    bool addCalls(string procName, StatementTypes type, string procCalled, int stmtNum);

    LIST_OF_INTSTR_PAIRS getCallsSSP(int stmtNum, string procCalled);

    LIST_OF_INTSTR_PAIRS getCallsSTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_INTSTR_PAIRS getCallsSST(int stmtNum, StatementTypes type2);

    LIST_OF_INTSTR_PAIRS getCallsSTP(StatementTypes type1, string procCalled);

    LIST_OF_STR_PAIRS getCallsPPP(string procName, string procCalled);

    LIST_OF_STR_PAIRS getCallsPTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_STR_PAIRS getCallsPPT(string procName, StatementTypes type2);

    LIST_OF_STR_PAIRS getCallsPTP(StatementTypes type1, string procCalled);

    std::unordered_set<int> *getCallStmtList();

    /////////////////////// CALLS STAR ///////////////////////

    bool addCallsStar(string procName, string procCalled);

    LIST_OF_STR_PAIRS getCallsStarNN(string procName, string procCalled);

    LIST_OF_STR_PAIRS getCallsStarTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_STR_PAIRS getCallsStarNT(string procName, StatementTypes type2);

    LIST_OF_STR_PAIRS getCallsStarTN(StatementTypes type1, string procCalled);

    /////////////////////// NEXT ///////////////////////

    bool addNext(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getNextNN(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getNextTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_INT_PAIRS getNextNT(int stmt1, StatementTypes type2);

    LIST_OF_INT_PAIRS getNextTN(StatementTypes type1, int stmt2);

    /////////////////////// NEXTBIP ///////////////////////

    bool addNextBip(int stmt1, int stmt2);

    bool getIsNextBipComputed();

    bool setNextBipComputed();

    LIST_OF_INT_PAIRS getNextBipNN(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getNextBipTT(StatementTypes type1, StatementTypes type2);

    LIST_OF_INT_PAIRS getNextBipNT(int stmt1, StatementTypes type2);

    LIST_OF_INT_PAIRS getNextBipTN(StatementTypes type1, int stmt2);

    /////////////// CLEAR ///////////////

    void clear();
};

#endif //AUTOTESTER_STORE_H
