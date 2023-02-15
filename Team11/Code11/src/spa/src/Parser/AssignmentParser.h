#pragma once
using namespace std;

#ifndef AUTOTESTER_ASSIGNMENTPARSER_H
#define AUTOTESTER_ASSIGNMENTPARSER_H
#include "StatementTypes.h"
#include <vector>


class AssignmentParser {
public:
    AssignmentParser() = default;
    static int parseAssignment(const std::vector<string>& tokens, int parentNum, StatementTypes parentType);
private:
    static int processLHS(vector<string> tokens, int parentNum, StatementTypes parentType);
    static int processRHS(vector<string> tokens, int parentNum, StatementTypes parentType);
};

#endif //AUTOTESTER_ASSIGNMENTPARSER_H