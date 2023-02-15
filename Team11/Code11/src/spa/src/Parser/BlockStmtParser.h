#ifndef AUTOTESTER_BLOCKSTMTPARSER_H
#define AUTOTESTER_BLOCKSTMTPARSER_H
#include "StatementTypes.h"

class BlockStmtParser {
public:
    BlockStmtParser() = default;
    static void populateInnerAbstractions(int innerIfConditionNum, int conditionStmtNum, StatementTypes parentType);
};


#endif //AUTOTESTER_BLOCKSTMTPARSER_H
