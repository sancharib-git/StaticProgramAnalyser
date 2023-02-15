#include <string>
#include <vector>
#include <stack>
#include "AddToPKB.h"
#include "AssignmentParser.h"
#include "Parser.h"
#include "Parser/AssignmentValidator.h"
#include "ParserException.h"
#include "ParserUtility.h"
#include "StatementTypes.h"
using namespace std;

int AssignmentParser::processLHS(vector<std::string> tokens, int parentNum, StatementTypes parentType)
{
    //LHS variable is the first token in the assignment statement, multiple variables cannot be declared in 1 statement.
    string var = tokens.at(0);
    //Syntax error checking of LHS variable and adding to PKB
    if(ParserUtility::isVariable(var))
    {
        AddToPKB::addVariableToPKB(var);
        AddToPKB::addAssignLHSVar(Parser::lineNum, var);
        AddToPKB::addModifiesSRelation(Parser::lineNum, ASSIGNMENT, var);
        if (parentNum != 0)
        {
            AddToPKB::addModifiesSRelation(parentNum, parentType, var);
        }
    } else
    {
        throw ParserException("Invalid Syntax of assignment statement");
    }
    return 0;
}

int AssignmentParser::processRHS(vector<std::string> tokens, int parentNum, StatementTypes parentType)
{
    AssignmentValidator assignmentValidator{};
    //get entire RHS string, validate variables or constants in RHS and add to PKB
    string RHS;
    for (int t = 2; t < tokens.size(); t++)
    {
        string currentToken = tokens[t];
        RHS += currentToken;
        if (ParserUtility::isVariable(currentToken))
        {
            AddToPKB::addAssignRHSVar(Parser::lineNum, currentToken);
            AddToPKB::addUsesSRelation(Parser::lineNum, ASSIGNMENT, currentToken);
            AddToPKB::addVariableToPKB(currentToken);
            if(parentNum != 0 )
            {
                AddToPKB::addUsesSRelation(parentNum, parentType, currentToken);
            }
        } else if (ParserUtility::isInteger(currentToken))
        {
            AddToPKB::addConstantToPKB(currentToken);
            AddToPKB::addAssignRHSConst(Parser::lineNum, std::stoi(currentToken));
        }
    }
    //syntax checking of RHS
    if(!(assignmentValidator.isExpr(RHS)))
    {
        throw ParserException("Invalid Syntax of assignment statement");
    }
    //add RHS to PKB
    AddToPKB::addPatternToPKB(RHS);
    return 0;
}

int AssignmentParser::parseAssignment(const vector<std::string>& tokens, int parentNum, StatementTypes parentType)
{
    PKB& sharedPkb = PKB::getInstance();
    AssignmentValidator::isValidTokens(tokens);
    processLHS(tokens, parentNum, parentType);
    processRHS(tokens, parentNum, parentType);
    return 0;
}
