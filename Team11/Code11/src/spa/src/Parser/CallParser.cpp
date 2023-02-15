#include "CallParser.h"
#include "ParserException.h"

bool checkCallProcedure(std::string proc)
{
    if (!isalpha(proc[0]))
    {
        return false;
    }
    for (int i = 0; i < proc.length(); i++)
    {
        if (!isalnum(proc[i]))
        {
            return false;
        }
    }
    return true;
}

/*
* Checks if call statement is valid
*/
bool CallParser::checkCallStmt(vector<std::string> tokens)
{
    if (tokens[0] == "call" && checkCallProcedure(tokens[1]) && tokens.size() == 2)
    {
        return true;
    }
    return false;
}


/*
* Returns the variable after parsing the print statement
*/
std::string CallParser::parseCallStmt(std::string stringToParse)
{
    try
    {
        Tokeniser tokeniser{};
        vector<std::string> tokens = tokeniser.Tokenise(stringToParse);
        if (tokens.size() != 2)
        {
            throw ParserException("Invalid syntax, call statement");
        }

        if (!checkCallStmt(tokens))
        {
            throw ParserException("Invalid syntax, call statement");
        }
        else
        {
            string proc = tokens[1];
            return proc;
        }
    }
    catch (exception e)
    {
        throw ParserException("Invalid syntax, call statement");
    }
}

