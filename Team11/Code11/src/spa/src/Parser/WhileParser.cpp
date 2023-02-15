#include <string>
#include <algorithm>
#include "AddToPKB.h"
#include "WhileParser.h"
#include "Parser.h"
#include "CondExprParser.h"
#include "StmtLstParser.h"
#include "StatementTypes.h"
#include "ParserException.h"
#include "ParserUtility.h"
using namespace std;

/**
 * Process condition and stmt lst, and add info to PKB
 * @param parent : stmt num of parent of this while block
 * @param blockType : block type of the parent (if/else/while)
 * */
int WhileParser::parseWhile(const std::string& whileBlock, int parent, StatementTypes parentType, StatementTypes blockType)
{
    CondExprParser condExprParser{};
    StmtLstParser stmtLstParser{};
    //get the condition (in accordance with line num)
    size_t firstOpenBrace = whileBlock.find('{');
    size_t firstBrac = whileBlock.find('(');
    if(firstBrac == std::string::npos)
    {
        throw ParserException("Invalid syntax, While statement, brackets error");
    }
    string condition = whileBlock.substr(firstBrac, firstOpenBrace-firstBrac);
    //send to conditionparser
    condExprParser.parseCondition(condition, StatementTypes::WHILE);
    int condLineNum = Parser::lineNum;
    //second, get the stomach
    int lastBracePos = ParserUtility::extractBraces(whileBlock);
    string whileStomach = whileBlock.substr(firstOpenBrace+1, lastBracePos-(firstOpenBrace+1));
    //send the stomach to stmtlst parser, since while stomach is stmt list
    stmtLstParser.parseStmtLst(whileStomach, Parser::lineNum, StatementTypes::WHILE, StatementTypes::WHILE);
    //add info to pkb
    addWhileInfoToPKB(condLineNum, parent, parentType, blockType);
    return condLineNum;
}

void WhileParser::addWhileInfoToPKB(int condLineNum, int parent, StatementTypes parentType, StatementTypes blockType)
{
    AddToPKB::addWhileStmt(condLineNum);
    AddToPKB::setStatementType(condLineNum, WHILE, Parser::currentProcName);
    if(parent!=0)
    {
        AddToPKB::addParentRelations(parent, condLineNum, parentType, StatementTypes::WHILE);
    }
    if(parentType == StatementTypes::WHILE) //add all the statement numbers of this while block into parent
    {
        for (int i = condLineNum; i <= Parser::lineNum; i++)
        {
            AddToPKB::addWhileBlock(parent, i);
        }
    }
    if(parentType == StatementTypes::IF && blockType == IF)
    {
        for (int i = condLineNum; i <= Parser::lineNum; i++)
        {
            AddToPKB::addIfBlock(parent, i);
        }
    }
    if(parentType == StatementTypes::IF && blockType == ELSE)
    {
        for (int i = condLineNum; i <= Parser::lineNum; i++)
        {
            AddToPKB::addElseBlock(parent, i);
        }
    }
    if(parent!=0)
    {
        populateInnerAbstractions(condLineNum, parent, parentType);
    }
}
