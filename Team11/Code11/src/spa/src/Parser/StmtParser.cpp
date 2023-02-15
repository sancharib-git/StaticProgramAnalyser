#include <utility>
#include "AddToPKB.h"
#include "AssignmentParser.h"
#include "CallParser.h"
#include "Parser.h"
#include "ParserException.h"
#include "PrintParser.h"
#include "ReadParser.h"
#include "StmtParser.h"
#include "Tokeniser.h"

using namespace std;

/**
 * Processes different types of stmts and adds info to PKB
 * */
StatementTypes StmtParser::parseStmt(std::string stmtString, int parent, StatementTypes parentType, StatementTypes blockType)
{
    Parser::lineNum++; //increment line num
    Tokeniser tokeniser{};
    vector<string> tokens = tokeniser.Tokenise(stmtString);
    StatementTypes stmtType = tokeniser.getStmtType(tokens); //get statement type of statement being parsed
    if (parentType == StatementTypes::WHILE) //add information to PKB
    {
        AddToPKB::addWhileBlock(parent, Parser::lineNum);
    } else if (parentType == StatementTypes::IF && blockType != ELSE)
    {
        AddToPKB::addIfBlock(parent, Parser::lineNum);
    } else if (parentType == StatementTypes::IF)
    {
        AddToPKB::addElseBlock(parent, Parser::lineNum);
    }
    //call respective parsers
    if (stmtType == StatementTypes::ASSIGNMENT)
    {
        processAssignment(tokens, parent, parentType);
        return StatementTypes::ASSIGNMENT;
    }
    else if (stmtType == StatementTypes::PRINT)
    {
        processPrint(stmtString, parent, parentType);
        return StatementTypes::PRINT;
    }
    else if (stmtType == StatementTypes::READ)
    {
        processRead(stmtString, parent, parentType);
        return StatementTypes::READ;
    }
    else if (stmtType == StatementTypes::CALL)
    {
        processCall(stmtString, parent, parentType);
        return StatementTypes::CALL;
    }
    else
    {
        throw ParserException("Invalid syntax, Statement");
    }
}

void StmtParser::processAssignment(vector<string> tokens, int parent, StatementTypes parentType)
{
    int stat = AssignmentParser::parseAssignment(tokens, parent, parentType);
    if(parent!=0)
    {
        AddToPKB::addParentRelations(parent, Parser::lineNum, parentType, StatementTypes::ASSIGNMENT);
    }
    AddToPKB::setStatementType(Parser::lineNum, ASSIGNMENT, Parser::currentProcName);
}

void StmtParser::processPrint(string stmtString,  int parent, StatementTypes parentType)
{
    string var = PrintParser::parsePrintStmt(move(stmtString));
    AddToPKB::addPrintStatement(Parser::lineNum, var);
    AddToPKB::addVariableToPKB(var);
    AddToPKB::addUsesSRelation(Parser::lineNum, PRINT, var);
    if(parent!=0)
    {
        AddToPKB::addParentRelations(parent, Parser::lineNum, parentType, StatementTypes::PRINT);
        AddToPKB::addUsesSRelation(parent, parentType, var);
    }
    AddToPKB::setStatementType(Parser::lineNum, PRINT, Parser::currentProcName);
}

void StmtParser::processRead(string stmtString, int parent, StatementTypes parentType)
{
    std::string var = ReadParser::parseReadStmt(std::move(stmtString));
    AddToPKB::addReadStatement(Parser::lineNum, var);
    AddToPKB::addVariableToPKB(var);
    AddToPKB::addModifiesSRelation(Parser::lineNum, READ, var);
    if(parent!=0)
    {
        AddToPKB::addParentRelations(parent, Parser::lineNum, parentType, StatementTypes::READ);
        AddToPKB::addModifiesSRelation(parent, parentType, var);
    }
    AddToPKB::setStatementType(Parser::lineNum, READ, Parser::currentProcName);
}

void StmtParser::processCall(string stmtString, int parent, StatementTypes parentType)
{
    std::string procName = CallParser::parseCallStmt(std::move(stmtString));
    AddToPKB::addCallStmt(procName, Parser::lineNum, CALL);
    if (parent != 0)
    {
        AddToPKB::addParentRelations(parent, Parser::lineNum, parentType, StatementTypes::CALL);
    }
    AddToPKB::setStatementType(Parser::lineNum, CALL, Parser::currentProcName);
}
