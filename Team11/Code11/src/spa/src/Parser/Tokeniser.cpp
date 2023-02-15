#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Tokeniser.h"
#include "PKB/PKB.h"
#include "ParserException.h"
using namespace std;

/**
 * Splits stmt into lexical tokens
 * */
std::vector<std::string> Tokeniser::Tokenise (std::string Stmt)
{
    std::vector<std::string> v;
    istringstream ss(Stmt);
    std::string word;
    while (ss >> word)
    {
        v.push_back(word);
    }
    if (Stmt.find('=') != std::string::npos)
    {
        std::vector<std::string> v1;
        v1 = tokenizeAssignment(v);
        return v1;
    }
    return v;
}

/**
 * Gets type of statement based on tokens
 * */
StatementTypes Tokeniser::getStmtType(std::vector<std::string> tokens)
{
    if (tokens.size() < 2) {
        throw ParserException("Invalid syntax, Tokenizer");
    }
    string &op = tokens[1];
    string st = tokens[0];
    if (op == "=") //All assignment statements second token is "="
    {
        return ASSIGNMENT;
    } 
    else if (st == "print") 
    {
        return PRINT;
    }
    else if (st == "read") 
    {
        return READ;
    } else if (st == "call")
    {
        return CALL;
    }
    return NO_TYPE;
}

/**
 * Further tokenise any tokens which do not have whitespaces, but consist of multiple lexical tokens
 * */
std::vector<std::string> Tokeniser::tokenizeAssignment(vector<string> assignmentTokens)
{
    //for each token, check if there are operators, if have: split based on them
    std::vector<std::string> tokens;
    for (std::string tok : assignmentTokens)
    {
        if (tok.size() > 1 &&
        (tok.find('=') != std::string::npos
        || tok.find('+') != std::string::npos
        || tok.find('-') != std::string::npos
        || tok.find('*') != std::string::npos
        || tok.find('/') != std::string::npos
        || tok.find('%') != std::string::npos
        || tok.find('(') != std::string::npos
        || tok.find(')') != std::string::npos)) //an operator has been found
        {
            processToken(tok, &tokens);
        } else
        {
            tokens.push_back(tok); //no operators found in tok, add into list as is
        }
    }
    return tokens;
}

/**
 * Process each token by splitting based on operator
 * */
void Tokeniser::processToken(const string& tok, vector<string>* tokens)
{
    std::string token;
    //tokenise based on operators
    //loop through chars in tok
    for (char chr : tok)
    {
        if(chr == '+' || chr == '-' || chr == '*' || chr == '/' || chr == '=' || chr == '%' || chr == '(' || chr == ')') //once operator is hit
        {
            if(!token.empty())
            {
                tokens->push_back(token); //add the current token into list
                std::string s(1, chr);
                tokens->push_back(s);
                token = "";
            } else {
                std::string s(1, chr);
                tokens->push_back(s);
            }
        } else
        {
            token.push_back(chr); //append the chr into token
        }
    }
    tokens->push_back(token);
}