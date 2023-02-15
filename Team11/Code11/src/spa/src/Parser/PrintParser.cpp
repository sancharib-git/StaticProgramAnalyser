#include "PrintParser.h"
#include "ParserException.h"

bool checkPrintVariable(std::string var) 
{
    if (!isalpha(var[0]))
    {
        return false;
    }
    for (int i = 0; i < var.length(); i++) 
    {
        if (!isalnum(var[i]))
        {
            return false;
        }
    }
    return true;
}

/*
* Returns the variable after parsing the print statement
*/
std::string PrintParser::parsePrintStmt(std::string stringToParse) 
{
    try
    {
        Tokeniser tokeniser{};
        vector<std::string> tokens = tokeniser.Tokenise(stringToParse);
        if (tokens.size() != 2)
        {
            throw ParserException("Invalid syntax, Print statement");
        }

        if (!checkPrintStmt(tokens))
        {
            throw ParserException("Invalid syntax, Print statement");
        }
        else
        {
            string var = tokens[1];
            return var;
        }
    }
    catch (exception e)
    {
        throw ParserException("Invalid syntax, Print statement");
    }
}

/*
* Checks if print statement is valid
*/
bool PrintParser::checkPrintStmt(vector<std::string> tokens) 
{
    if (tokens[0] == "print" && checkPrintVariable(tokens[1]) && tokens.size() == 2) 
    {
        return true;
    }
    return false;
}
