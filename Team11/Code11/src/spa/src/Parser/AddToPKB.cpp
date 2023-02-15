#include <utility>
#include "AddToPKB.h"
#include "Parser.h"
#include "ParserException.h"
#include "TreeProcessor.h"
using namespace std;

int AddToPKB::addConstantToPKB(const string& s)
{
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addConst(stoi(s));
    } catch (exception& e)
    {
        throw ParserException("Could not add Constant to PKB");
    }
    return 0;
}

int AddToPKB::addVariableToPKB(string s)
{
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addVar(move(s));
    } catch (exception& e)
    {
        throw ParserException("Could not add variable to PKB");
    }
    return 0;
}

int AddToPKB::addPatternToPKB (string RHS)
{
    PKB& sharedPkb = PKB::getInstance();
    try {
        TreeProcessor tp = TreeProcessor();
        BinaryTree* tree = tp.buildASTFromSubexpr(RHS);
        string processed = tp.treeInString(*tree);
        sharedPkb.addAssignPattern(Parser::lineNum, move(processed));
    } catch (exception& e)
    {
        throw ParserException("Could not add pattern to PKB");
    }
    return 0;
}

int AddToPKB::addAssignRHSVar(int lineNum, string var) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addAssignRHSVar(lineNum, move(var));
    } catch (exception& e)
    {
        throw ParserException("Could not add RHS var to PKB");
    }
    return 0;
}

int AddToPKB::addAssignRHSConst(int lineNum, int constant) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addAssignRHSConst(lineNum, constant);
    } catch (exception& e)
    {
        throw ParserException("Could not add RHS const to PKB");
    }
    return 0;
}

int AddToPKB::addAssignLHSVar(int lineNum, string var) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addAssignLHSVar(lineNum, move(var));
    } catch (exception& e)
    {
        throw ParserException("Could not add LHS var to PKB");
    }
    return 0;
}

int AddToPKB::addUsesSRelation(int lineNum, StatementTypes type, string var) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addUsesS(lineNum, type, move(var));
    } catch (exception& e)
    {
        throw ParserException("Could not add UsesS relation to PKB");
    }
    return 0;
}

int AddToPKB::addModifiesSRelation(int lineNum, StatementTypes type, string var) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addModifiesS(lineNum, type, move(var));
    } catch (exception& e)
    {
        throw ParserException("Could not add ModifiesS relation to PKB");
    }
    return 0;
}

int AddToPKB::addIfStmt(int stmtNum) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addIfStmt(stmtNum);
    } catch (exception& e)
    {
        throw ParserException("Could not add if stmt relation to PKB");
    }
    return 0;
}

int AddToPKB::addParentRelations(int stmtNum1, int stmtNum2, StatementTypes stmtType1, StatementTypes stmtType2) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addParent(stmtNum1, stmtType1, stmtNum2, stmtType2);
    } catch (exception& e)
    {
        throw ParserException("Could not add Parent to PKB");
    }
    return 0;
}

int AddToPKB::addFollowsRelations(int s1, int s2, StatementTypes stmtType1, StatementTypes stmtType2) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addFollows(s1, stmtType1, s2, stmtType2);
    } catch (exception& e)
    {
        throw ParserException("Could not add Follows to PKB");
    }
    return 0;
}

int AddToPKB::addTotalLines(int lines) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.setNum(lines);
    } catch (exception& e)
    {
        throw ParserException("Could not set total number of lines to PKB");
    }
    return 0;
}

int AddToPKB::addWhileBlock(int parentNum, int stmtNum) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addWhileBlockStmt(parentNum, stmtNum);
    } catch (exception& e)
    {
        throw ParserException("Could not add while block stmt to PKB");
    }
    return 0;
}

int AddToPKB::addIfBlock(int parentNum, int stmtNum) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addIfBlockStmt(parentNum, stmtNum);
    } catch (exception& e)
    {
        throw ParserException("Could not add if block stmt to PKB");
    }
    return 0;
}

int AddToPKB::addElseBlock(int parentNum, int stmtNum) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addElseBlockStmt(parentNum, stmtNum);
    } catch (exception& e)
    {
        throw ParserException("Could not add else block stmt to PKB");
    }
    return 0;
}

int AddToPKB::addPrintStatement(int stmtNum, string var) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addPrintStmt(stmtNum, move(var));
    } catch (exception& e)
    {
        throw ParserException("Could not add print stmt to PKB");
    }
    return 0;
}

int AddToPKB::addProcedure(string name) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addProcedure(move(name));
    } catch (exception& e)
    {
        throw ParserException("Could not add procedure to PKB");
    }
    return 0;
}

int AddToPKB::setStatementType(int stmtNum, StatementTypes type, string procName) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addStmt(stmtNum, type, move(procName));
    } catch (exception& e)
    {
        throw ParserException("Could not set stmt type to PKB");
    }
    return 0;
}

int AddToPKB::addReadStatement(int stmtNum, string var) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addReadStmt(stmtNum, move(var));
    } catch (exception& e)
    {
        throw ParserException("Could not add read stmt to PKB");
    }
    return 0;
}

int AddToPKB::addWhileStmt(int stmtNum) {
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addWhileStmt(stmtNum);
    } catch (exception& e)
    {
        throw ParserException("Could not add while stmt to PKB");
    }
    return 0;
}

int AddToPKB::addCallStmt(string proc, int stmtNum, StatementTypes types)
{
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addCalls(Parser::currentProcName, types, move(proc), stmtNum);
    } catch (exception& e)
    {
        throw ParserException("Could not add call stmt to PKB");
    }
    return 0;
}

int AddToPKB::addWhileControlVar(string var, int stmtNum)
{
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addWhileStmtCV(stmtNum, move(var));
    } catch (exception& e)
    {
        throw ParserException("Could not add while control var to PKB");
    }
    return 0;
}

int AddToPKB::addIfControlVar(string var, int stmtNum)
{
    PKB& sharedPkb = PKB::getInstance();
    try {
        sharedPkb.addIfStmtCV(stmtNum, move(var));
    } catch (exception& e)
    {
        throw ParserException("Could not add if control var to PKB");
    }
    return 0;
}

