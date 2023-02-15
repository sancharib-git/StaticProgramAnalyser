//
// Created by Yen Peng Yip on 3/10/21.
//

#ifndef AUTOTESTER_CALLSSTORE_H
#define AUTOTESTER_CALLSSTORE_H

#include <vector>
#include <string>
#include "StatementTypes.h"
#include "Relationship.h"
#include "Definitions.h"

class CallsStore {

public:
    CallsStore();
    void clear();

    bool addS(int stmtNum, StatementTypes type, string varUsed);
    LIST_OF_INTSTR_PAIRS getSSP(int stmt, string procCalled);
    LIST_OF_INTSTR_PAIRS getSTT(StatementTypes type1, StatementTypes type2);
    LIST_OF_INTSTR_PAIRS getSST(int stmt, StatementTypes type);
    LIST_OF_INTSTR_PAIRS getSTP(StatementTypes type, string procCalled);

    bool addP(string procName, StatementTypes type, string varUsed);
    LIST_OF_STR_PAIRS getPPP(string procName, string procCalled);
    LIST_OF_STR_PAIRS getPTT(StatementTypes type1, StatementTypes type2);
    LIST_OF_STR_PAIRS getPPT(string procName, StatementTypes type2);
    LIST_OF_STR_PAIRS getPTP(StatementTypes type1, string procCalled);

    bool addStar(string procName, string procCalled);
    LIST_OF_STR_PAIRS getStarNN(string procName, string procCalled);
    LIST_OF_STR_PAIRS getStarTT(StatementTypes type1, StatementTypes type2);
    LIST_OF_STR_PAIRS getStarNT(string procName, StatementTypes type2);
    LIST_OF_STR_PAIRS getStarTN(StatementTypes type1, string procCalled);
private:
    static Relationship<int, string> callsS;
    static Relationship<string, string> callsP;
    static Relationship<string, string> callsStar;
};

#endif //AUTOTESTER_CALLSSTORE_H
