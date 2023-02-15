#include <stack>
#include <string>
#include "ParserUtility.h"
#include "ParserException.h"
using namespace std;

bool ParserUtility::isOperator(const string& c)
{
    return (c == "+" ||
            c == "-" ||
            c == "*" ||
            c == "/" ||
            c == "%" );
}

/**
 * Returns the position of the matching close bracket of the first open bracket found
 * @return int : position of matching close bracket
 * */
int ParserUtility::extractBracketsForwards(const string& str)
{
    int last_pos = 0; //to store position of the last '}'
    size_t init_pos; //to find {} only after this position, (find next { or }
    std::stack<char> myStack;
    size_t openBraceindex = str.find('(');
    size_t closeBraceindex = str.find(')');
    if (openBraceindex == std::string::npos && closeBraceindex == std::string::npos)
    {
        throw ParserException("Invalid syntax, brackets error");
    }
    size_t minValue = std::min<size_t>(openBraceindex, closeBraceindex); //get whichever is next
    if (minValue == closeBraceindex)  // if its }, pop from stack but since stack is currently empty, this throws error.
    {
        throw ParserException("Invalid syntax, brackets error");
    } else //if its { push into stack
    {
        myStack.push('(');
    }
    init_pos = minValue+1; //update initpos to current index + 1, so can find from after current index
    while(!myStack.empty())
    {
        if(init_pos > str.length())
        {
            throw ParserException("Invalid syntax, brackets error");
        }
        openBraceindex = str.find('(', init_pos);
        closeBraceindex = str.find(')', init_pos);
        if (openBraceindex == std::string::npos && closeBraceindex == std::string::npos)
        {
            throw ParserException("Invalid syntax, brackets error");
        }
        minValue = std::min<size_t>(openBraceindex, closeBraceindex);
        if (minValue == openBraceindex)
        {
            myStack.push('(');
            init_pos = minValue+1;
        } else
        {
            myStack.pop();
            init_pos = minValue+1;
            last_pos = (int)minValue;
        }
    }
    return last_pos;
}

/**
 * Gets position of the matching open bracket to the close bracket at position specified
 * @param from : position of close bracket
 * */
int ParserUtility::extractBracketsBackwards(const string& bracketString, int from)
{
    int last_pos = 0; //to store position of the last '('
    size_t init_pos; //to find () only after this position, find next ( or )
    std::stack<char> myStack;
    size_t openBraceindex = bracketString.rfind('(', from);
    size_t closeBraceindex = bracketString.rfind(')', from);
    size_t minValue = 0;
    if(openBraceindex != std::string::npos && closeBraceindex != std::string::npos)
    {
        minValue = std::max<size_t>(openBraceindex, closeBraceindex);
        if (minValue == openBraceindex)  // if its (, pop from stack but since stack is currently empty, this throws error.
        {
            throw ParserException("Invalid syntax : error in brackets");
        } else //if its ) push into stack
        {
            myStack.push(')');
        }
    }
    init_pos = minValue-1; //update initpos to current index - 1, so can rfind from after current index
    while(!myStack.empty())
    {
        if(init_pos < 0)
        {
            throw ParserException("Invalid syntax : error in brackets");
        }
        openBraceindex = bracketString.rfind('(', init_pos);
        closeBraceindex = bracketString.rfind(')', init_pos);
        if (openBraceindex == std::string::npos && closeBraceindex == std::string::npos)
        {
            throw ParserException("Invalid syntax : error in brackets");
        }
        if(openBraceindex != std::string::npos && closeBraceindex != std::string::npos)
        {
            minValue = std::max<size_t>(openBraceindex, closeBraceindex);
        } else
        {
            if(openBraceindex == std::string::npos)
            {
                minValue = closeBraceindex;
            } else
            {
                minValue = openBraceindex;
            }
        }
        if (minValue == closeBraceindex)
        {
            myStack.push(')');
            init_pos = minValue-1;
        } else
        {
            myStack.pop();
            init_pos = minValue-1;
            last_pos = (int)minValue;
        }
    }
    return last_pos;
}

/**
 * Extract contents of the first matching open and close brackets, in a string
 * @return string : contents of brackets
 * */
string ParserUtility::extractBracketString(const string& str)
{
    int last_pos = 0; //to store position of the last ')'
    size_t init_pos; //to find () only after this position, find next ( or )
    std::stack<char> myStack;
    size_t openBraceindex = str.find('(');
    size_t firstOpen = openBraceindex;
    size_t closeBraceindex = str.find(')');
    if (openBraceindex == std::string::npos && closeBraceindex == std::string::npos)
    {
        throw ParserException("Invalid syntax : error in brackets");
    }
    size_t minValue = std::min<size_t>(openBraceindex, closeBraceindex); //get whichever is next
    if (minValue == closeBraceindex)  // if its ), pop from stack but since stack is currently empty, this throws error.
    {
        throw ParserException("Invalid syntax : error in brackets");
    } else //if its ( push into stack
    {
        myStack.push('(');
    }
    init_pos = minValue+1; //update initpos to current index + 1, so can find from after current index
    while(!myStack.empty())
    {
        if(init_pos > str.length())
        {
            throw ParserException("Invalid syntax : error in brackets");
        }
        openBraceindex = str.find('(', init_pos);
        closeBraceindex = str.find(')', init_pos);
        if (openBraceindex == std::string::npos && closeBraceindex == std::string::npos)
        {
            throw ParserException("Invalid syntax : error in brackets");
        }
        minValue = std::min<size_t>(openBraceindex, closeBraceindex);
        if (minValue == openBraceindex)
        {
            myStack.push('(');
            init_pos = minValue+1;
        } else
        {
            myStack.pop();
            init_pos = minValue+1;
            last_pos = (int)minValue;
        }
    }
    if(last_pos < str.length()- 1
    && str.find_first_not_of("()", last_pos) == std::string::npos) //the remaining chars are all ( or ) despite empty stack
    {
        throw ParserException("Invalid syntax : error in brackets");
    }
    if(last_pos-(firstOpen+1) == 0) {
        throw ParserException("Invalid syntax : error in brackets");
    }
    return str.substr(firstOpen+1, last_pos-(firstOpen+1));
}

/**
 * Returns the position of the close brace matching the first open brace found
 * */
int ParserUtility::extractBraces(const string& code)
{
    int last_pos = 0; //to store position of the last '}'
    size_t init_pos; //to find {} only after this position, (find next { or }
    std::stack<char> myStack;
    size_t openBraceindex = code.find('{');
    size_t closeBraceindex = code.find('}');
    if (openBraceindex == std::string::npos)
    {
        throw ParserException("Invalid syntax, braces error");
    }
    size_t minValue = std::min<size_t>(openBraceindex, closeBraceindex); //get whichever is next
    if (minValue == closeBraceindex) // if its }, pop from stack but since stack is currently empty, this throws error.
    {
        throw ParserException("Invalid syntax, braces error");
    } else //if its { push into stack
    {
        myStack.push('{');
    }
    init_pos = minValue+1; //update initpos to current index + 1, so can find from after current index

    while(!myStack.empty())
    {
        if(init_pos > code.length())
        {
            throw ParserException("Invalid syntax : error in braces");
        }
        openBraceindex = code.find('{', init_pos);
        closeBraceindex = code.find('}', init_pos);
        if (openBraceindex == std::string::npos && closeBraceindex == std::string::npos)
        {
            throw ParserException("Invalid syntax, braces error");
        }
        minValue = std::min<size_t>(openBraceindex, closeBraceindex);
        if (minValue == openBraceindex)
        {
            myStack.push('{');
            init_pos = minValue+1;
        } else
        {
            myStack.pop();
            init_pos = minValue+1;
            last_pos = (int)minValue;
        }
    }
    return last_pos;
}

/**
 * Gets position of matching close brace of the open brace at position from
 * @param from : position of open brace
 * */
int ParserUtility::extractBracesFrom(const string& code, size_t from)
{
    int last_pos = 0; //to store position of the last '}'
    size_t init_pos; //to find {} only after this position, (find next { or }
    std::stack<char> myStack;
    size_t openBraceindex = code.find('{', from);
    size_t closeBraceindex = code.find('}', from);
    if (openBraceindex == std::string::npos)
    {
        throw ParserException("Invalid syntax, braces error");
    }
    size_t minValue = std::min<size_t>(openBraceindex, closeBraceindex); //get whichever is next
    if (minValue == closeBraceindex) // if its }, pop from stack but since stack is currently empty, this throws error.
    {
        throw ParserException("Invalid syntax, braces error");
    } else //if its { push into stack
    {
        myStack.push('{');
    }
    init_pos = minValue+1; //update initpos to current index + 1, so can find from after current index

    while(!myStack.empty())
    {
        if(init_pos > code.length())
        {
            throw ParserException("Invalid syntax : error in braces");
        }
        openBraceindex = code.find('{', init_pos);
        closeBraceindex = code.find('}', init_pos);
        if (openBraceindex == std::string::npos && closeBraceindex == std::string::npos)
        {
            throw ParserException("Invalid syntax, braces error");
        }
        minValue = std::min<size_t>(openBraceindex, closeBraceindex);
        if (minValue == openBraceindex)
        {
            myStack.push('{');
            init_pos = minValue+1;
        } else
        {
            myStack.pop();
            init_pos = minValue+1;
            last_pos = (int)minValue;
        }
    }
    return last_pos;
}

bool ParserUtility::isInteger(string s)
{
    if(s.length() > 1 && s[0] == '0')
    {
        throw ParserException("Invalid number");
    }
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
    {
        return false;
    }
    char * p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
}

bool ParserUtility::isVariable(string s)
{
    if (!isalpha(s[0]))
    {
        return false;
    }
    for (int i = 0; i < s.length(); i++)
    {
        if (!isalnum(s[i]))
        {
            return false;
        }
    }
    return true;
}
