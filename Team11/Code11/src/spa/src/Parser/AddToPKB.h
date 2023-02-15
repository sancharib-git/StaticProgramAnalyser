#ifndef AUTOTESTER_ADDTOPKB_H
#define AUTOTESTER_ADDTOPKB_H
#include <string>
#include "StatementTypes.h"
using namespace std;

class AddToPKB {
public:
    AddToPKB() = default;
    static int addConstantToPKB(const std::string& s);
    static int addVariableToPKB(std::string s);
    static int addPatternToPKB(std::string RHS);
    static int addAssignRHSVar(int lineNum, std::string var);
    static int addAssignRHSConst(int lineNum, int constant);
    static int addUsesSRelation(int lineNum, StatementTypes type, std::string var);
    static int addModifiesSRelation(int lineNum, StatementTypes type, std::string var);
    static int addAssignLHSVar(int lineNum, std::string var);
    static int addIfStmt(int stmtNum);
    static int addWhileStmt(int stmtNum);
    static int addParentRelations(int stmtNum1, int stmtNum2, StatementTypes stmtType1, StatementTypes stmtType2);
    static int addFollowsRelations(int s1, int s2, StatementTypes stmtType1, StatementTypes stmtType2);
    static int addTotalLines(int lines);
    static int addProcedure(std::string name);
    static int addWhileBlock(int parentNum, int stmtNum);
    static int addIfBlock(int parentNum, int stmtNum);
    static int addElseBlock(int parentNum, int stmtNum);
    static int addPrintStatement(int stmtNum, std::string var);
    static int addReadStatement(int stmtNum, std::string var);
    static int setStatementType(int stmtNum, StatementTypes type, std::string procName);
    static int addCallStmt(std::string proc, int stmtNum, StatementTypes types);
    static int addIfControlVar(string var, int stmtNum);
    static int addWhileControlVar(string var, int stmtNum);
};


#endif //AUTOTESTER_ADDTOPKB_H
