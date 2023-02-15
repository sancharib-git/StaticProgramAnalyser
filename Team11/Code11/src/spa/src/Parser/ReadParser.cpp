#include "ReadParser.h"
#include "ParserException.h"

bool checkReadVariable(std::string var) 
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
* Returns the variable after parsing the read statement
*/
std::string ReadParser::parseReadStmt(std::string stringToParse) 
{
    try
    {
        Tokeniser tokeniser{};
        vector<std::string> tokens = tokeniser.Tokenise(stringToParse);
        if (tokens.size() != 2)
        {
            throw ParserException("Invalid syntax, Read statement");
        }

        if (!checkReadStmt(tokens))
        {
            throw ParserException("Invalid syntax, Read statement");
        }
        else
        {
            string var = tokens[1];
            return var;
        }
    }
    catch (exception e)
    {
        throw ParserException("Invalid syntax, Read statement");
    }
}

/*
* Checks if read statement is valid
*/
bool ReadParser::checkReadStmt(vector<std::string> tokens) 
{
    if (tokens[0] == "read" && checkReadVariable(tokens[1]) && tokens.size() == 2) 
    {
        return true;
    }
    return false;
}
