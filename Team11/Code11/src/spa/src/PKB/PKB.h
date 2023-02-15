#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include "Table.h"
#include "Hasher.h"
#include "Table.cpp"
#include "StatementTypes.h"
#include "Relationship.h"
#include "Relationship.cpp"
#include "Store.h"
#include "Definitions.h"
#include "Cache.h"

using namespace std;

class PKB {

private:
    static PKB *instance;
    static Store store;
    static Cache cache;

public:
    PKB() {
        static Store store;
        static Cache cache;
    };

    // for client to access PKB
    static PKB& getInstance() {
        if (!(instance)) {
            instance = new PKB();
        }
        return *instance;
    };

    /////////////////////// ASSIGN ///////////////////////

    bool addIfStmtCV(int ifStmtNum, string var);

    bool addWhileStmtCV(int whileStmtNum, string var);

    // get the list of if condition statements that consists of the specified variable
    UNORDERED_LIST_OF_INT getIfStmtsWithCV(string var);

    // get the list of control variables present in the specified if condition statement
    UNORDERED_LIST_OF_STR getAllCVInIfStmt(int stmtNum);

    // get the list of while condition statements that consists of the specified variable
    UNORDERED_LIST_OF_INT getWhileStmtsWithCV(string var);

    // get the list of control variables present in the specified while condition statement
    UNORDERED_LIST_OF_STR getAllCVInWhileStmt(int stmtNum);

    bool addAssignLHSVar(int stmtNum, string var);

    bool addAssignRHSVar(int stmtNum, string var);

    bool addAssignRHSConst(int stmtNum, int constant);

    bool addAssignPattern(int stmtNum, string pattern);

    std::string getAssignPattern(int stmtNum);

    std::string getAssignLHSVar(int stmtNum);

    UNORDERED_LIST_OF_STR getAssignRHSVars(int stmtNum);

    UNORDERED_LIST_OF_INT getAssignRHSConst(int stmtNum);

    LIST_OF_STR getAssignRHSVarsVector(int stmtNum);

    LIST_OF_INT getAssignRHSConstVector(int stmtNum);

    // get list of all assign stmt nums
    UNORDERED_LIST_OF_INT* getAssignList();

    bool addAssignSubExp(int stmtNum, string subExp);

    UNORDERED_LIST_OF_INT getSubExpStmtNums(string subExp);

    /////////////////////// READ & PRINT ///////////////////////

    // add read stmt num to readStmtList and readMap
    bool addReadStmt(int stmtNum, string var);

    // add print stmt num to printStmtList and printMap
    bool addPrintStmt(int stmtNum, string var);

    // get variable in read statement
    std::string getReadVar(int stmtNum);

    // get list of all read stmt nums
    UNORDERED_LIST_OF_INT* getReadStmtList();

    // get variable in print statement
    std::string getPrintVar(int stmtNum);

    // get list of all print stmt nums
    UNORDERED_LIST_OF_INT* getPrintStmtList();

    /////////////////////// IF / ELSE / WHILE ///////////////////////

    // add if stmt num to ifList
    bool addIfStmt(int stmtNum);

    // add blockStmtNum (in ascending order) to list of values under key ifStmtNum
    bool addIfBlockStmt(int ifStmtNum, int blockStmtNum);

    // add blockStmtNum (in ascending order) to list of values under key ifStmtNum
    bool addElseBlockStmt(int ifStmtNum, int blockStmtNum);

    // add while stmt num to whileList
    bool addWhileStmt(int stmtNum);

    // add blockStmtNum (in ascending order) to list of values under key whileStmtNum
    bool addWhileBlockStmt(int whileStmtNum, int blockStmtNum);

    // get list of all if stmt nums
    UNORDERED_LIST_OF_INT* getIfList();

    // get list of all if block stmt nums
    LIST_OF_INT getIfBlockList(int ifStmtNum);

    // get list of all else block stmt nums
    LIST_OF_INT getElseBlockList(int ifStmtNum);

    // get list of all while stmt nums
    UNORDERED_LIST_OF_INT* getWhileList();

    // get list of all while block stmt nums
    LIST_OF_INT getWhileBlockList(int whileStmtNum);

    /////////////////////// STMT / PROC / VAR / CONST ///////////////////////

    bool addStmt(int stmtNum, StatementTypes type, std::string getorName);

    // add proc name to procedureList
    bool addProcedure(std::string procName);

    int getProcDigit(string procName);

    string getProcName(int procDigit);

    LIST_OF_INT getProcedureStmtNums(string procName);

    // add var to varTable and returns index
    bool addVar(std::string var);

    // add constant to constTable and returns index
    bool addConst(int constant);

    // get list of all procedure names
    UNORDERED_LIST_OF_STR* getProcList();

    int getTotalNumOfVars();

    UNORDERED_LIST_OF_STR* getAllVars();

    int getTotalNumOfConsts();

    UNORDERED_LIST_OF_INT* getAllConsts();

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

    std::unordered_set<int>* getCallStmtList();

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

    /////////////////////// NEXT STAR ///////////////////////

    bool addNextStar(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getNextStarNN(int stmt1, int stmt2);
    LIST_OF_INT_PAIRS getNextStarTT(StatementTypes type1, StatementTypes type2);
    LIST_OF_INT_PAIRS getNextStarNT(int stmt1, StatementTypes type2);
    LIST_OF_INT_PAIRS getNextStarTN(StatementTypes type1, int stmt2);

    bool getNextStarTTBoolean(StatementTypes type1, StatementTypes type2);
    bool getNextStarNTBoolean(int stmt1, StatementTypes type2);
    bool getNextStarTNBoolean(StatementTypes type1, int stmt2);

    /////////////////////// NEXTBIP ///////////////////////

    bool addNextBip(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getNextBipNN(int stmt1, int stmt2);
    LIST_OF_INT_PAIRS getNextBipTT(StatementTypes type1, StatementTypes type2);
    LIST_OF_INT_PAIRS getNextBipNT(int stmt1, StatementTypes type2);
    LIST_OF_INT_PAIRS getNextBipTN(StatementTypes type1, int stmt2);

    /////////////////////// NEXTBIP STAR ///////////////////////

    bool addNextBipStar(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getNextBipStarNN(int stmt1, int stmt2);
    LIST_OF_INT_PAIRS getNextBipStarTT(StatementTypes type1, StatementTypes type2);
    LIST_OF_INT_PAIRS getNextBipStarNT(int stmt1, StatementTypes type2);
    LIST_OF_INT_PAIRS getNextBipStarTN(StatementTypes type1, int stmt2);

    bool getNextBipStarTTBoolean(StatementTypes type1, StatementTypes type2);
    bool getNextBipStarNTBoolean(int stmt1, StatementTypes type2);
    bool getNextBipStarTNBoolean(StatementTypes type1, int stmt2);

    /////////////////////// AFFECTS ///////////////////////

    bool addAffects(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getAffectsNN(int stmt1, int stmt2);
    LIST_OF_INT_PAIRS getAffectsTT(StatementTypes type1, StatementTypes type2);
    LIST_OF_INT_PAIRS getAffectsNT(int stmt1, StatementTypes type2);
    LIST_OF_INT_PAIRS getAffectsTN(StatementTypes type1, int stmt2);

    bool getAffectsTTBoolean(StatementTypes type1, StatementTypes type2);
    bool getAffectsNTBoolean(int stmt1, StatementTypes type2);
    bool getAffectsTNBoolean(StatementTypes type1, int stmt2);

    /////////////////////// AFFECTSSTAR ///////////////////////

    bool addAffectsStar(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getAffectsStarNN(int stmt1, int stmt2);
    LIST_OF_INT_PAIRS getAffectsStarTT(StatementTypes type1, StatementTypes type2);
    LIST_OF_INT_PAIRS getAffectsStarNT(int stmt1, StatementTypes type2);
    LIST_OF_INT_PAIRS getAffectsStarTN(StatementTypes type1, int stmt2);

    bool getAffectsStarTTBoolean(StatementTypes type1, StatementTypes type2);
    bool getAffectsStarNTBoolean(int stmt1, StatementTypes type2);
    bool getAffectsStarTNBoolean(StatementTypes type1, int stmt2);

    /////////////////////// AFFECTSBIP ///////////////////////

    bool addAffectsBip(int stmt1, int stmt2);

    LIST_OF_INT_PAIRS getAffectsBipNN(int stmt1, int stmt2);
    LIST_OF_INT_PAIRS getAffectsBipTT(StatementTypes type1, StatementTypes type2);
    LIST_OF_INT_PAIRS getAffectsBipNT(int stmt1, StatementTypes type2);
    LIST_OF_INT_PAIRS getAffectsBipTN(StatementTypes type1, int stmt2);

    /////////////// EXTRA ///////////////
    void print(vector<std::pair<int, int> > const &s);
    void print(vector<std::pair<int, string> > const &s);

    ///////////////////STATEMENT TYPES ADDERS AND GETTERS///////////////////

    static StatementTypes getStatementType(int statementNumber);

    bool setStatementType(int statementNumber, StatementTypes stmtType);

    int getTotalNumberOfStatements();

    bool setNum(int num);

    /////////////// CLEAR ///////////////

    void clear();
    void clearCache();
};


