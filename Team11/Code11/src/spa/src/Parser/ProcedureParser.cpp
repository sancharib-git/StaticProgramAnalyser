#include <string>
#include <vector>
#include <sstream>
#include "AddToPKB.h"
#include "Parser.h"
#include "ParserException.h"
#include "ParserUtility.h"
#include "ProcedureParser.h"
#include "PKB/PKB.h"
#include "StmtLstParser.h"
#include "StatementTypes.h"

bool isValidParentheses(const std::string& procString)
{
    size_t openBraceindex = procString.find('{'); //index for first {
    size_t closeBraceindex = procString.rfind('}'); //index of last }
    if (openBraceindex == std::string::npos || closeBraceindex == std::string::npos)
    {
        return false;
    }
    if (openBraceindex > closeBraceindex)
    {
        return false;
    }
    return true;
}

std::string getProcDeclaration (const std::string& procString)
{
    size_t openBraceindex = procString.find('{');
    std::string declaration = procString.substr(0, openBraceindex);
    return declaration;
}

bool isValidProcName(const std::string& procString)
{
    PKB& sharedPkb = PKB::getInstance();
    std::string declaration = getProcDeclaration(procString);
    std::vector<std::string> tokens;
    istringstream ss(declaration);
    std::string word;
    while (ss >> word)
    {
        tokens.push_back(word);
    }

    if(tokens.size() != 2)
    {
        return false;
    }
    if(tokens[0] != "procedure")
    {
        return false;
    }
    if(!isalpha(tokens[1][0]))
    {
        return false;
    }
    for (char c:tokens[1])
    {
        if(!isalnum(c))
        {
            return false;
        }
    }
    //if PKB procedure list already has this procedure name, return false
    unordered_set<basic_string<char>>* procList = sharedPkb.getProcList();
    if(procList->find(tokens[1]) != procList->end())
    {
        return false;
    }
    AddToPKB::addProcedure(tokens[1]);
    Parser::currentProcName = tokens[1];
    return true;
}

/*
 * Extracts the contents between open and close braces of a procedure
 * */
std::string ProcedureParser::getProcedureStomach(const std::string& procString)
{
    size_t openBraceIndex = procString.find('{');
    size_t lastBrace = ParserUtility::extractBraces(procString);
    std::string stom = procString.substr(openBraceIndex+1, lastBrace-openBraceIndex-1);
    return stom;
}

void ProcedureParser::parseProcedure(const std::string& procString)
{
    PKB& sharedPkb = PKB::getInstance();
    //check syntax
    if (!isValidParentheses(procString))
    {
        throw ParserException("Invalid syntax, braces error in procedure");
    }
    if (!isValidProcName(procString))
    {
        throw ParserException("Invalid procedure naming and declaration");
    }
    //get stomach of procedure
    std::string procStomach = getProcedureStomach(procString);
    //call parse statement list on procedure stomach
    StmtLstParser stmtLstParser{};
    stmtLstParser.parseStmtLst(procStomach, 0, StatementTypes::NO_TYPE, StatementTypes::NO_TYPE);
    //total num stmts in pkb
    AddToPKB::addTotalLines(Parser::lineNum);
}
