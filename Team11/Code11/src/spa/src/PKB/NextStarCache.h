//
// Created by Yen Peng Yip on 28/10/21.
//

#ifndef AUTOTESTER_NEXTSTARCACHE_H
#define AUTOTESTER_NEXTSTARCACHE_H

#pragma once
#include <vector>
#include <unordered_set>
#include <utility>
#include "StatementTypes.h"
#include "Relationship.h"
#include "Definitions.h"
#include "Hasher.h"

class NextStarCache {
public:
    NextStarCache();
    virtual void clear();

    virtual bool addStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2);
    bool addNextStarNNCalled(int stmt1, int stmt2);
    bool isNextStarNNCalled(int stmt1, int stmt2);
    bool addNextStarTTCalled(StatementTypes type1);
    bool isNextStarTTCalled(StatementTypes type1);
    bool addNextStarNTCalled(int stmt1, StatementTypes type2);
    bool isNextStarNTCalled(int stmt1, StatementTypes type2);
    bool addNextStarTNCalled(StatementTypes type1, int stmt2);
    bool isNextStarTNCalled(StatementTypes type1, int stmt2);
    virtual LIST_OF_INT_PAIRS getStarNN(int stmt1, int stmt2);
    virtual LIST_OF_INT_PAIRS getStarTT(StatementTypes type1, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getStarNT(int stmt1, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getStarTN(StatementTypes type1, int stmt2);

private:
    static Relationship<int, int> nextStar;
    std::unordered_set<std::pair<int, int>, intIntHash> nextStarNNCalled;
    std::unordered_set<std::pair<StatementTypes, StatementTypes>, intIntHash> nextStarTTCalled;
    std::unordered_set<std::pair<int, StatementTypes>, intIntHash> nextStarNTCalled;
    std::unordered_set<std::pair<StatementTypes, int>, intIntHash> nextStarTNCalled;
};

#endif //AUTOTESTER_NEXTSTARCACHE_H
