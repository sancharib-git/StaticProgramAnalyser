#pragma once
#ifndef AUTOTESTER_TOKENISER_H
#define AUTOTESTER_TOKENISER_H
#include "PKB/PKB.h"

using namespace std;

using namespace std;

class Tokeniser {
public:
    Tokeniser() = default;
    StatementTypes getStmtType(std::vector<std::string> tokens);
    std::vector<std::string> Tokenise(std::string Stmt);
    static std::vector<std::string> tokenizeAssignment(std::vector<std::string> assignmentTokens);

private:
    static void processToken(const string& tok, vector<string>* tokens);
};

#endif //AUTOTESTER_TOKENISER_H