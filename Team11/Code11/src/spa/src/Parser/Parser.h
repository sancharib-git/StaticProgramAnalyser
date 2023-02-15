#pragma once

using namespace std;

#include "Tokeniser.h"
#include "Parser/AssignmentParser.h"

class Parser {
public:
    static int lineNum;
    Parser() = default;
    static std::string fileToString(const string& filename);
    static int Parse(std::string fileStr);
    static std::string currentProcName;
};
