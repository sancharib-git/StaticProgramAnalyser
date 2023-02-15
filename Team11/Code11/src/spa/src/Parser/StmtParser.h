#ifndef AUTOTESTER_STMTPARSER_H
#define AUTOTESTER_STMTPARSER_H


#include "StatementTypes.h"

class StmtParser {
public:
    StmtParser() = default;
    static StatementTypes parseStmt(std::string stmtString, int parent, StatementTypes parentType, StatementTypes blockType);
private:
    static void processAssignment(vector<string> tokens, int parent, StatementTypes parentType);
    static void processRead(string stmtString, int parent, StatementTypes parentType);
    static void processPrint(string stmtString,  int parent, StatementTypes parentType);
    static void processCall(string stmtString, int parent, StatementTypes parentType);
};


#endif //AUTOTESTER_STMTPARSER_H
