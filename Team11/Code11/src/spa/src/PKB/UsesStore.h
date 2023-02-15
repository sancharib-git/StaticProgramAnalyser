//
// Created by Yen Peng Yip on 3/10/21.
//

#ifndef AUTOTESTER_USESSTORE_H
#define AUTOTESTER_USESSTORE_H

#include <vector>
#include <string>
#include "StatementTypes.h"
#include "Relationship.h"
#include "Definitions.h"

class UsesStore {

public:
    UsesStore();
    virtual void clear();

    virtual bool addS(int stmtNum, StatementTypes type, string varUsed);
    virtual LIST_OF_INTSTR_PAIRS getSSV(int stmt, string var);
    virtual LIST_OF_INTSTR_PAIRS getSTT(StatementTypes type1, StatementTypes type2);
    virtual LIST_OF_INTSTR_PAIRS getSST(int stmt, StatementTypes type);
    virtual LIST_OF_INTSTR_PAIRS getSTV(StatementTypes type, string var);

    virtual bool addP(string procName, StatementTypes type, string varUsed);
    virtual LIST_OF_STR_PAIRS getPPV(string procName, string var);
    virtual LIST_OF_STR_PAIRS getPTT(StatementTypes type1, StatementTypes type2);
    virtual LIST_OF_STR_PAIRS getPPT(string procName, StatementTypes type2);
    virtual LIST_OF_STR_PAIRS getPTV(StatementTypes type1, string var);

private:
    static Relationship<int, string> usesS;
    static Relationship<string, string> usesP;
};

#endif //AUTOTESTER_USESSTORE_H
