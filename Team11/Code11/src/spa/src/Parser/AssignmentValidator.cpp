#include <stack>
#include <string>
#include <vector>
#include <iostream>
#include "Parser/AssignmentValidator.h"
#include "ParserException.h"
#include "ParserUtility.h"
using namespace std;

/**
 * Validates Expr based on SPA grammar
 * */
bool AssignmentValidator::isExpr(std::string expr)
{
    if(empty(expr))
    {
        return false;
    }
    char last = expr.back();
    if(expr[0] == '(' && last == ')')
    {
        if(ParserUtility::extractBracketString(expr) == expr.substr(1, expr.length()-2))
        {
            return isFactor(expr);
        }
    }
    if(expr.find('+') == string::npos && expr.find('-') == string::npos) //not an expr
    {
        return isTerm(expr);
    }
    int i = (int)expr.size()-1;
    while (i >= 0) {
        if(expr[i] == ')')
        {
            int openBrac = ParserUtility::extractBracketsBackwards(expr, i);
            i = openBrac;
            continue;
        } else if (expr[i] == '+' || expr[i] == '-')
        {
            break;
        } else
        {
            i--;
        }
    }
    if (i <= 0 ) //means the only +/- are in brackets
    {
        return isTerm(expr);
    } else {
        string expr1 = expr.substr(0, i);
        string term1 = expr.substr(i+1);
        if(isTerm(term1))
        {
            if(isExpr(expr1))
            {
                return true;
            }
        }
        return false;
    }
}

/**
 * Validates Term based on SPA grammar
 * */
bool AssignmentValidator::isTerm(std::string term)
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
            return isFactor(term);
        }
    }
    if(term.find('/') == string::npos && term.find('*') == string::npos && term.find('%') == string::npos) //not a term
    {
        return isFactor(term);
    }
    int i = (int)term.size()-1;
    while (i >= 0)
    {
        if(term[i] == ')')
        {
            int openBrac = ParserUtility::extractBracketsBackwards(term, i);
            i = openBrac;
            continue;
        } else if (term[i] == '/' || term[i] == '%' || term[i] == '*') {
            break;
        } else
        {
            i--;
        }
    }
    if (i <= 0)  //the /*% found are all in brackets
    {
        return isFactor(term);
    } else
    {
        string term1 = term.substr(0, i);
        string factor1 = term.substr(i+1);
        if(isFactor(factor1))
        {
            if(isTerm(term1))
            {
                return true;
            }
        }
        return false;
    }
}

/**
 * Validates Factor based on SPA grammar
 * */
bool AssignmentValidator::isFactor(std::string factor)
{
    if(empty(factor))
    {
        return false;
    }
    if (ParserUtility::isVariable(factor) || ParserUtility::isInteger(factor))
    {
        return true;
    } else
    {
        char last = factor.back();
        if(factor[0] == '(' && last == ')')
        {
            if(ParserUtility::extractBracketString(factor) == factor.substr(1, factor.length()-2))
            {
                string exprinbracs = factor.substr(1, factor.length()-2);
                return isExpr(exprinbracs);
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

bool AssignmentValidator::isValidTokens(std::vector<std::string> tokens)
{
    for (int i = 2; i < tokens.size()-1; i++ ) {
        //variable must be followed by operator
        if ((ParserUtility::isVariable(tokens[i]) || ParserUtility::isInteger(tokens[i])))
        {
            if(!ParserUtility::isOperator(tokens[i+1]) && tokens[i+1] != ")")
            {
                throw ParserException("Invalid syntax : variable not followed by operator");
            }
        }
        //operator cannot be followed by operator
        if (ParserUtility::isOperator(tokens[i]))
        {
            if (ParserUtility::isOperator(tokens[i+1]))
            {
                throw ParserException("Invalid syntax : operator followed by operator");
            }
        }
    }
    return true;
}
