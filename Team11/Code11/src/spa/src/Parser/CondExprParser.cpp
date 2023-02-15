#include <algorithm>
#include <string>
#include <sstream>
#include "AddToPKB.h"
#include "CondExprParser.h"
#include "Parser.h"
#include "ParserException.h"
#include "ParserUtility.h"

/**
 * Validates condexpr based on SPA grammar
 * */
bool CondExprParser::isCondExpr(const std::string& condString, StatementTypes parent) //param shld not be in brackets
{
    if(empty(condString))
    {
        return false;
    }
    if(condString.find("!(") == std::string::npos &&
    condString.find("&&") == std::string::npos &&
    condString.find("||") == std::string::npos) //HAS to be relexpr if none of these can be found
    {
        return isRelExpr(condString, parent);
    }

    size_t notnot = condString.find("!(");
    if(notnot == 0)
    {
        //first get left brackstring
        std::string leftBrackString = ParserUtility::extractBracketString(condString);
        if(condString.length() - leftBrackString.length() > 3) //extracted leftbrackstring should only be 3 characters smaller than condstring: !,( and )
        {
            return false;
        }
        return isCondExpr(leftBrackString, parent);
    } else //if theres no ! at the front, MUST be a && or ||. so extract the 2 sides
    {
        //first get left brackstring
        int lastPosOfLeftBracks = ParserUtility::extractBracketsForwards(condString);
        std::string leftBrackString = ParserUtility::extractBracketString(condString);
        //then update condstring
        std::string newCondString = condString.substr(lastPosOfLeftBracks+1);
        if(newCondString.find("&&") == 0 || newCondString.find("||") == 0)
        {
            std::string inbracsright = ParserUtility::extractBracketString(newCondString);
            return (isCondExpr(inbracsright, parent) && isCondExpr(leftBrackString, parent));
        } else //&& or || not found at the start DESPITE cutting off the left brackets of condString
        {
            return false;
        }
    }
}

bool CondExprParser::isRelExpr(const std::string& relString, StatementTypes parent)
{
    if(empty(relString))
    {
        return false;
    }
    size_t bigger = relString.find('>');
    size_t bigeq = relString.find(">=");
    size_t smaller = relString.find('<');
    size_t smalleq = relString.find("<=");
    size_t eq = relString.find("==");
    size_t noteq = relString.find("!=");
    if(bigeq != std::string::npos)
    {
        std::string left = relString.substr(0, bigeq);
        std::string right = relString.substr(bigeq+2);
        return (isRelFactor(right, parent) && isRelFactor(left, parent));
    } else if(bigger != std::string::npos)
    {
        std::string left = relString.substr(0, bigger);
        std::string right = relString.substr(bigger+1);
        return (isRelFactor(right, parent) && isRelFactor(left, parent));
    } else if(smalleq != std::string::npos)
    {
        std::string left = relString.substr(0, smalleq);
        std::string right = relString.substr(smalleq+2);
        return (isRelFactor(right, parent) && isRelFactor(left, parent));
    } else if(smaller != std::string::npos)
    {
        std::string left = relString.substr(0, smaller);
        std::string right = relString.substr(smaller+1);
        return (isRelFactor(right, parent) && isRelFactor(left, parent));
    } else if(eq != std::string::npos)
    {
        std::string left = relString.substr(0, eq);
        std::string right = relString.substr(eq+2);
        return (isRelFactor(right, parent) && isRelFactor(left, parent));
    } else if(noteq != std::string::npos)
    {
        std::string left = relString.substr(0, noteq);
        std::string right = relString.substr(noteq+2);
        return (isRelFactor(right, parent) && isRelFactor(left, parent));
    }
    return false;
}

bool CondExprParser::isRelFactor(const std::string& relFactor, StatementTypes parent )
{
    if(empty(relFactor))
    {
        return false;
    }
    if (relFactor.find('(') != std::string::npos)
    {
        return isExpr(relFactor, parent);
    }
    if (ParserUtility::isVariable(relFactor) || ParserUtility::isInteger(relFactor))
    {
        if(ParserUtility::isVariable(relFactor))
        {
            AddToPKB::addVariableToPKB(relFactor);
            AddToPKB::addUsesSRelation(Parser::lineNum, parent, relFactor);
            if(parent == StatementTypes::IF)
            {
                AddToPKB::addIfControlVar(relFactor, Parser::lineNum);
            } else if(parent == StatementTypes::WHILE)
            {
                AddToPKB::addWhileControlVar(relFactor, Parser::lineNum);
            }
        } else
        {
            AddToPKB::addConstantToPKB(relFactor);
        }
        return true;
    } else if (isExpr(relFactor, parent))
    {
        return true;
    }
    return false;
}

/**
 * Parses expr based on spa grammar and adds necessary info to PKB
 * @param parent : parent of this conditional stmt
 * */
bool CondExprParser::isExpr(std::string expr, StatementTypes parent)
{
    char last = expr.back();
    if(expr[0] == '(' && last == ')')
    {
        if(ParserUtility::extractBracketString(expr) == expr.substr(1, expr.length()-2))
        {
            return isFactor(expr, parent);
        }
    }
    if(expr.find('+') == string::npos && expr.find('-') == string::npos) { //move
        return isTerm(expr, parent);
    }
    int i = (int)expr.size()-1;
    while (i >= 0) {
        if(expr[i] == ')')
        {
            int openBrac = ParserUtility::extractBracketsBackwards(expr, i);
            i = openBrac;
            continue;
        } else if (expr[i] == '+' || expr[i] == '-') {
            break;
        } else
        {
            i--;
        }
    }

    if (i <= 0 ) //means the only +/- are in brackets
    {
        return isTerm(expr, parent);
    } else {
        string expr1 = expr.substr(0, i);
        string term1 = expr.substr(i+1);
        if(isTerm(term1, parent))
        {
            if(isExpr(expr1, parent))
            {
                return true;
            }
        }
        return false;
    }
}

/**
 * Parses factor based on spa grammar and adds necessary info to PKB
 * @param parent : parent of this conditional stmt
 * */
bool CondExprParser::isFactor(std::string factor, StatementTypes parent)
{
    if(empty(factor)) {
        return false;
    } else if (ParserUtility::isVariable(factor) || ParserUtility::isInteger(factor))
    {
        if(ParserUtility::isVariable(factor)) {
            AddToPKB::addVariableToPKB(factor);
            AddToPKB::addUsesSRelation(Parser::lineNum, parent, factor);
            if(parent == StatementTypes::IF)
            {
                AddToPKB::addIfControlVar(factor, Parser::lineNum);
            } else if(parent == StatementTypes::WHILE)
            {
                AddToPKB::addWhileControlVar(factor, Parser::lineNum);
            }
        } else
        {
            AddToPKB::addConstantToPKB(factor);
        }
        return true;
    } else
    {
        char last = factor.back();
        if(factor[0] == '(' && last == ')')
        {
            if(ParserUtility::extractBracketString(factor) == factor.substr(1, factor.length()-2))
            {
                string exprinbracs = factor.substr(1, factor.length()-2);
                return isExpr(exprinbracs, parent);
            } else
            {
                return false;
            }
        } else
        {
            return false;
        }
    }
}

/**
 * Parses term based on spa grammar and adds necessary info to PKB
 * @param parent : parent of this conditional stmt
 * */
bool CondExprParser::isTerm(std::string term, StatementTypes parent)
{
    if(empty(term))
    {
        return false;
    }
    char last = term.back();
    if(term[0] == '(' && last == ')')
    {
        if(ParserUtility::extractBracketString(term) == term.substr(1, term.length()-2))
        {
            return isFactor(term, parent);
        }
    }
    if(term.find('/') == string::npos && term.find('*') == string::npos && term.find('%') == string::npos)
    {
        return isFactor(term, parent);
    }
    int i = (int)term.size()-1;
    while (i >= 0) {
        if(term[i] == ')') {
            int openBrac = ParserUtility::extractBracketsBackwards(term, i);
            i = openBrac;
            continue;
        } else if (term[i] == '/' || term[i] == '%' || term[i] == '*') {
            break;
        } else {
            i--;
        }
    }
    if (i <= 0)  //the /*% found are all in brackets
    {
        return isFactor(term, parent);
    } else {
        string term1 = term.substr(0, i);
        string factor1 = term.substr(i+1);
        if(isFactor(factor1, parent))
        {
            if(isTerm(term1, parent))
            {
                return true;
            }
        }
        return false;
    }
}

void CondExprParser::parseCondition(const std::string& condString, StatementTypes parent) {
    //ATTN LINE NUM FOR condition changed here
    Parser::lineNum++;
    //strip all whitespaces
    std::string res; //this res is the whitespace stripped condition string
    std::istringstream ss(condString);
    std::string word;
    while (ss >> word)
    {
        res += word;
    }
    bool isValidCondExpr = checkConditionSyntax(res, parent);
    if (!isValidCondExpr) {
        throw ParserException("Invalid syntax, conditional statement");
    }
}

bool CondExprParser::checkConditionSyntax(const std::string& condString, StatementTypes parent) {
    std::string condExpr = ParserUtility::extractBracketString(condString);
    return isCondExpr(condExpr, parent);
}
