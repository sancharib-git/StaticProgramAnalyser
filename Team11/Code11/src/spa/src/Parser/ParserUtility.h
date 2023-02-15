#ifndef AUTOTESTER_PARSERUTILITY_H
#define AUTOTESTER_PARSERUTILITY_H
using namespace std;

class ParserUtility {
public:
    ParserUtility() = default;
    static bool isOperator(const string& c);
    static int extractBracketsBackwards(const string& bracketString, int from);
    static int extractBracketsForwards(const string& s);
    static string extractBracketString(const string& brackets);
    static bool isInteger(string s);
    static bool isVariable(string s);
    static int extractBraces(const string& code);
    static int extractBracesFrom(const string &code, size_t from);
};


#endif //AUTOTESTER_PARSERUTILITY_H
