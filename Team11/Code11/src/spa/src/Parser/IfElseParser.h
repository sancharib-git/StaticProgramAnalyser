#ifndef AUTOTESTER_IFELSEPARSER_H
#define AUTOTESTER_IFELSEPARSER_H

#include <utility>
#include "BlockStmtParser.h"
#include "StatementTypes.h"

class IfElseParser : public BlockStmtParser
{
public:
    IfElseParser() = default;
    static int parseIfElse(const std::string& ifElseString, int parent, StatementTypes parentType, StatementTypes blockType);
private:
    static void addIfElseInfoToPKB(int ifStmtNum, int parent, StatementTypes parentType, StatementTypes blockType);
    static pair<int, int> processIfBlock(const string& ifElseString);
    static void processElseBlock(const string& ifElseString, size_t lastBracePos, int ifStmtNum);
};


#endif //AUTOTESTER_IFELSEPARSER_H
