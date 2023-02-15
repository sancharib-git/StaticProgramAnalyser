#include <string>
#include <utility>
#include "AddToPKB.h"
#include "IfElseParser.h"
#include "Parser.h"
#include "Parser/CondExprParser.h"
#include "ParserException.h"
#include "ParserUtility.h"
#include "StmtLstParser.h"
#include "StatementTypes.h"

/**
 * Processes if and else blocks and adds necessary data to PKB
 * @param parent : stmt num of parent of this if block
 * @param blockType : block type of the parent (if/else/while)
 * */
int IfElseParser::parseIfElse(const std::string& ifElseString, int parent, StatementTypes parentType, StatementTypes blockType)
{
    pair<int, int> p = processIfBlock(ifElseString);
    int ifStmtNum = p.first;
    int lastBracePos = p.second;
    processElseBlock(ifElseString, (size_t)lastBracePos, ifStmtNum);
    addIfElseInfoToPKB(ifStmtNum, parent, parentType, blockType);
    return ifStmtNum;
}

pair<int, int> IfElseParser::processIfBlock(const string& ifElseString)
{
    CondExprParser condExprParser{};
    StmtLstParser stmtLstParser{};
    //first get the condition (in accordance with line num)
    size_t firstBrac = ifElseString.find('(');
    int lastBrac = ParserUtility::extractBracketsForwards(ifElseString);
    std::string condition = ifElseString.substr(firstBrac, lastBrac-firstBrac+1);
    size_t firstOpenBrace = ifElseString.find('{');
    if(firstOpenBrace == string::npos || firstBrac == string::npos || firstBrac > firstOpenBrace)
    {
        throw ParserException("first bracket/brace not found in ifElse");
    }
    size_t then = ifElseString.find("then", lastBrac);
    if(then == string::npos || then > firstOpenBrace)
    {
        throw ParserException("then not found");
    }

    //send to conditionparser
    condExprParser.parseCondition(condition, StatementTypes::IF);
    int ifStmtNum = Parser::lineNum;

    //second, get the stomach, note that there r 2 stomachs in ifelse
    int lastBracePos = ParserUtility::extractBraces(ifElseString);
    std::string ifStomach = ifElseString.substr(firstOpenBrace+1, lastBracePos-(firstOpenBrace+1)); //check if this works

    //send the stomach to stmtlst parser, since if stomach is stmt list
    stmtLstParser.parseStmtLst(ifStomach, ifStmtNum, StatementTypes::IF, StatementTypes::IF);
    return pair<int, int>(ifStmtNum, lastBracePos);
}

void IfElseParser::processElseBlock(const string& ifElseString, size_t lastBracePos, int ifStmtNum)
{
    StmtLstParser stmtLstParser1{}; //separate instance of StmtLstParser
    std::string elseSubstr = ifElseString.substr(lastBracePos+1);
    size_t elsePos = elseSubstr.find("else");
    if(elseSubstr.find_first_not_of(" \t\n\v\f\r") != elsePos || elsePos == string::npos)
    {
        throw ParserException("Invalid syntax, Else clause not found");
    }
    size_t elseFirstBrace = elseSubstr.find('{');
    int lastElseBrace = ParserUtility::extractBraces(elseSubstr);
    int len = lastElseBrace - (int)elseFirstBrace -1;
    std::string elseStomach = elseSubstr.substr(elseFirstBrace+1, len);
    //send the stomach to stmtlst parser
    stmtLstParser1.parseStmtLst(elseStomach, ifStmtNum, StatementTypes::IF, StatementTypes::ELSE);
}

/*
 * Adds all the necessary info from If-Else blocks into PKB
 * */
void IfElseParser::addIfElseInfoToPKB(int ifStmtNum, int parent, StatementTypes parentType, StatementTypes blockType)
{
    //add information to PKB
    AddToPKB::addIfStmt(ifStmtNum);
    if(parent!=0)
    {
        AddToPKB::addParentRelations(parent, ifStmtNum, parentType, StatementTypes::IF);
    }
    AddToPKB::setStatementType(ifStmtNum, IF, Parser::currentProcName);
    if(parentType == StatementTypes::WHILE)
    {
        //add all the numbers of the block into parent
        for (int i = ifStmtNum; i <= Parser::lineNum; i++)
        {
            AddToPKB::addWhileBlock(parent, i);
        }
    }
    if(parentType == StatementTypes::IF && blockType == IF)
    {
        for (int i = ifStmtNum; i <= Parser::lineNum; i++)
        {
            AddToPKB::addIfBlock(parent, i);
        }
    }
    if(parentType == StatementTypes::IF && blockType == ELSE)
    {
        for (int i = ifStmtNum; i <= Parser::lineNum; i++)
        {
            AddToPKB::addElseBlock(parent, i);
        }
    }
    if(parent!=0)
    {
        populateInnerAbstractions(ifStmtNum, parent, parentType);
    }
}
