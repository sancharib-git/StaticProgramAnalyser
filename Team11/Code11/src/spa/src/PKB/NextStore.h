//
// Created by Yen Peng Yip on 6/10/21.
//

#ifndef AUTOTESTER_NEXTSTORE_H
#define AUTOTESTER_NEXTSTORE_H

#include <vector>
#include <string>
#include "StatementTypes.h"
#include "Relationship.h"
#include "Definitions.h"

class NextStore {
public:
    NextStore();
    virtual void clear();

    virtual bool add(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getNN(int stmt1, int stmt2);
    virtual LIST_OF_INT_PAIRS getTT(StatementTypes type1, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getNT(int stmt1, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getTN(StatementTypes type1, int stmt2);
private:
    static Relationship<int, int> next;
};

#endif //AUTOTESTER_NEXTSTORE_H
