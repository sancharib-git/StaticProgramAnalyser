#ifndef AUTOTESTER_STMTLSTPARSER_H
#define AUTOTESTER_STMTLSTPARSER_H
#include "StatementTypes.h"
#include <vector>

class StmtLstParser {
public:
    std::vector<std::pair<StatementTypes, int>> followsStatements;
    StmtLstParser() = default; //put as prev line num
    void parseStmtLst(std::string stomach, int parent, StatementTypes parentType, StatementTypes blockType);


private:
    static size_t checkWhileIndexValidity(string stomach, size_t whileIndex);
    static size_t checkIfIndexValidity(string stomach, size_t ifIndex);
    static size_t checkWhileIndexValidityInsideLoop(string stomach, size_t whileIndex);
    static size_t checkIfIndexValidityInsideLoop(string stomach, size_t ifIndex);
    void processStmt(const string& stomach, size_t semicolIndex, int parent, StatementTypes parentType, StatementTypes blockType);
    int processWhileBlock(const string& stomach, size_t whileIndex, int parent, StatementTypes parentType, StatementTypes blockType);
    int processIfElseBlock(string stomach, size_t ifIndex, int parent, StatementTypes parentType, StatementTypes blockType);
    void addFollowsStmtLst();
};


#endif //AUTOTESTER_STMTLSTPARSER_H
