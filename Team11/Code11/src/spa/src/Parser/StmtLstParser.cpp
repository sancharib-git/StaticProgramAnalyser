#include <algorithm>
#include <fstream>
#include <string>
#include <utility>
#include "AddToPKB.h"
#include "IfElseParser.h"
#include "ParserException.h"
#include "ParserUtility.h"
#include "StmtLstParser.h"
#include "StmtParser.h"
#include "WhileParser.h"
using namespace std;

/**
 * Parses Statement list
 * Non-static method
 * */
void StmtLstParser::parseStmtLst(std::string stomach, int parent, StatementTypes parentType, StatementTypes blockType)
{
    //find indexes of the different types of statements (read/assign/print) and (if/else,while)
    size_t semicolIndex = stomach.find(';');
    size_t whileIndex = stomach.find("while");
    size_t ifIndex = stomach.find("if");
    if (semicolIndex == string::npos && whileIndex == string::npos && ifIndex == string::npos)
    {
        throw ParserException("Invalid syntax in statement list");
    }
    //checking if ifindex and whileindex are valid
    ifIndex = checkIfIndexValidity(stomach, ifIndex);
    whileIndex = checkWhileIndexValidity(stomach, whileIndex);

    while(semicolIndex != string::npos || whileIndex != string::npos || ifIndex != string::npos) //loop through all statements
    {
        ifIndex = checkIfIndexValidityInsideLoop(stomach, ifIndex);
        whileIndex = checkWhileIndexValidityInsideLoop(stomach, whileIndex);
        size_t minValue = min<size_t>(semicolIndex, whileIndex);
        minValue = min<size_t>(minValue, ifIndex); //find which statement is nearest (to be parsed next)
        if (minValue == semicolIndex) //parse statement
        {
            processStmt(stomach, semicolIndex, parent, parentType, blockType);
            stomach = stomach.substr(semicolIndex+1);
        } else if (minValue == whileIndex) //parse while
        {
            if(stomach.find_first_not_of(" \t\n\v\f\r") != stomach.find("while"))
            {
                throw ParserException("Invalid characters found before while");
            }
            int lastPos = processWhileBlock(stomach, whileIndex, parent, parentType, blockType);
            stomach = stomach.substr(lastPos+1);
        } else //parse if
        {
            if(stomach.find_first_not_of(" \t\n\v\f\r") != stomach.find("if"))
            {
                throw ParserException("Invalid characters found before if");
            }
            int lastPos = processIfElseBlock(stomach, ifIndex, parent, parentType, blockType);
            stomach = stomach.substr(lastPos+1);
        }
        semicolIndex = stomach.find(';');
        whileIndex = stomach.find("while");
        ifIndex = stomach.find("if");
    }
    if(!empty(stomach))
    {
        if(stomach.find_first_not_of(" \t\n\v\f\r") != std::string::npos) //non-space string
        {
            throw ParserException("Invalid characters found in statement list");
        }
    }
    //add follows rln for all stmts in this statement list
    addFollowsStmtLst();
}

/**
 * check if whileindex is valid, e.g. while  (...) - valid, worthwhile = 4; - invalid
 * */
size_t StmtLstParser::checkWhileIndexValidity(string stomach, size_t whileIndex)
{
    if (whileIndex != std::string::npos) {
        if(stomach[whileIndex+5] == '(') {
            whileIndex = whileIndex;
        } else if (isspace(stomach.at(whileIndex+5))) {
            int i = (int)whileIndex+5;
            while (isspace(stomach.at(i))) {
                i++;
            }
            if(stomach[i] != '(') {
                whileIndex = std::string::npos;
            }
        } else {
            whileIndex = std::string::npos;
        }
    }
    return whileIndex;

}

/**
 * check if ifindex is valid, e.g. if  (...) - valid, iffy = 4; - invalid
 * */
size_t StmtLstParser::checkIfIndexValidity(string stomach, size_t ifIndex) {
    if (ifIndex != std::string::npos) {
        if(stomach[ifIndex+2] == '(') {
            ifIndex = ifIndex;
        } else if (isspace(stomach.at(ifIndex+2))) {
            int i = (int)ifIndex+2;
            while (isspace(stomach.at(i))) {
                i++;
            }
            if(stomach[i] != '(') {
                ifIndex = std::string::npos;
            }
        } else {
            ifIndex = std::string::npos;
        }
    }
    return ifIndex;
}

size_t StmtLstParser::checkWhileIndexValidityInsideLoop(string stomach, size_t whileIndex)
{
    if (whileIndex != std::string::npos) {
        if(stomach[whileIndex+5] == '(') {
            whileIndex = whileIndex;
        } else if (isspace(stomach.at(whileIndex+5))) {
            int i = (int)whileIndex+5;
            while (isspace(stomach.at(i))) {
                i++;
            }
            if(stomach[i] != '(') {
                whileIndex = SIZE_MAX;
            }
        } else {
            whileIndex = SIZE_MAX;
        }
    }
    return whileIndex;
}

size_t StmtLstParser::checkIfIndexValidityInsideLoop(string stomach, size_t ifIndex) {
    if (ifIndex != std::string::npos) {
        if(stomach[ifIndex+2] == '(') {
            ifIndex = ifIndex;
        } else if (isspace(stomach.at(ifIndex+2))) {
            int i = (int)ifIndex+2;
            while (isspace(stomach.at(i))) {
                i++;
            }
            if(stomach[i] != '(') {
                ifIndex = SIZE_MAX;
            }
        } else {
            ifIndex = SIZE_MAX;
        }
    }
    return ifIndex;
}

/**
 * Add follows relations of this stmt lst to PKB
 * */
void StmtLstParser::addFollowsStmtLst()
{
    //add follows relations from the list - Follows (s1, s2) holds if they are in the same statement list (stmtLst),
    // and s2 appears immediately after s1
    for(int i = 0; i < followsStatements.size()-1; ++i)
    {
        AddToPKB::addFollowsRelations(followsStatements[i].second, followsStatements[i+1].second,
                                      followsStatements[i].first, followsStatements[i+1].first);
    }
}

void StmtLstParser::processStmt(const string& stomach, size_t semicolIndex, int parent,
                                StatementTypes parentType, StatementTypes blockType)
{
    StmtParser stmtParser{};
    //get the statement
    std::string stmt = stomach.substr(0, semicolIndex);
    //send to parse statement
    StatementTypes stmtType = stmtParser.parseStmt(stmt, parent, parentType, blockType);
    //add follows relation
    std::pair<StatementTypes, int> stmtDeets(stmtType, Parser::lineNum);
    followsStatements.push_back(stmtDeets);

}

int StmtLstParser::processWhileBlock(const string& stomach, size_t whileIndex, int parent,
                                     StatementTypes parentType, StatementTypes blockType)
{
    WhileParser whileParser{};
    //get the entire while block using getblock function
    int lastPos = ParserUtility::extractBraces(stomach); //changed the param from code to stomach
    std::string whileBlock = stomach.substr(whileIndex, lastPos+1);
    //send to while parser
    int innerCondNum = whileParser.parseWhile(whileBlock, parent, parentType, blockType);
    //add follows
    std::pair<StatementTypes, int> stmtDeets(StatementTypes::WHILE, innerCondNum);
    followsStatements.push_back(stmtDeets);
    return lastPos;
}

int StmtLstParser::processIfElseBlock(string stomach, size_t ifIndex, int parent,
                                      StatementTypes parentType, StatementTypes blockType)
{
    int ifLastPos = ParserUtility::extractBraces(stomach); //changed param from code to stomach
    std::string ifBlock = stomach.substr(ifIndex, ifLastPos-ifIndex+1);
    int elseLastPos = ParserUtility::extractBracesFrom(stomach, ifLastPos+1); //extract else block
    std::string elseBlock = stomach.substr(ifLastPos+1, elseLastPos-(ifLastPos+1)+1);
    std::string ifelseblock = ifBlock+elseBlock;
    //send to if parser
    int innerCondNum = IfElseParser::parseIfElse(ifelseblock, parent, parentType, blockType);
    //add follows
    std::pair<StatementTypes, int> stmtDeets(StatementTypes::IF, innerCondNum);
    followsStatements.push_back(stmtDeets);
    return elseLastPos;
}
