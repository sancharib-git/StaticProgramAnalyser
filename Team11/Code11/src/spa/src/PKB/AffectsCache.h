//
// Created by Yen Peng Yip on 31/10/21.
//

#ifndef AUTOTESTER_AFFECTSCACHE_H
#define AUTOTESTER_AFFECTSCACHE_H

#pragma once
#include <vector>
#include "StatementTypes.h"
#include "Relationship.h"
#include "Definitions.h"
#include "Hasher.h"

class AffectsCache {
public:
    AffectsCache();
    virtual void clear();

    virtual bool add(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2);
    bool addAffectsNNCalled(int stmt1, int stmt2);
    bool isAffectsNNCalled(int stmt1, int stmt2);
    bool addAffectsNTCalled(int stmt1, StatementTypes type2);
    bool isAffectsNTCalled(int stmt1, StatementTypes type2);
    bool addAffectsTTCalled(StatementTypes type1, StatementTypes type2);
    bool isAffectsTTCalled(StatementTypes type1, StatementTypes type2);
    bool addAffectsTNCalled(StatementTypes type1, int stmt2);
    bool isAffectsTNCalled(StatementTypes type1, int stmt2);
    virtual LIST_OF_INT_PAIRS getNN(int stmt1, int stmt2);
    virtual LIST_OF_INT_PAIRS getTT(StatementTypes type1, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getNT(int stmt1, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getTN(StatementTypes type1, int stmt2);

    virtual bool addStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2);
    bool addAffectsStarNNCalled(int stmt1, int stmt2);
    bool isAffectsStarNNCalled(int stmt1, int stmt2);
    bool addAffectsStarNTCalled(int stmt1, StatementTypes type2);
    bool isAffectsStarNTCalled(int stmt1, StatementTypes type2);
    bool addAffectsStarTTCalled(StatementTypes type1, StatementTypes type2);
    bool isAffectsStarTTCalled(StatementTypes type1, StatementTypes type2);
    bool addAffectsStarTNCalled(StatementTypes type1, int stmt2);
    bool isAffectsStarTNCalled(StatementTypes type1, int stmt2);
    virtual LIST_OF_INT_PAIRS getStarNN(int stmt1, int stmt2);
    virtual LIST_OF_INT_PAIRS getStarTT(StatementTypes type1, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getStarNT(int stmt1, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getStarTN(StatementTypes type1, int stmt2);

private:
    static Relationship<int, int> affects;
    static Relationship<int, int> affectsStar;
    std::unordered_set<std::pair<int, int>, intIntHash> affectsNNCalled;
    std::unordered_set<std::pair<StatementTypes, StatementTypes>, intIntHash> affectsTTCalled;
    std::unordered_set<std::pair<int, StatementTypes>, intIntHash> affectsNTCalled;
    std::unordered_set<std::pair<StatementTypes, int>, intIntHash> affectsTNCalled;
    std::unordered_set<std::pair<int, int>, intIntHash> affectsStarNNCalled;
    std::unordered_set<std::pair<StatementTypes, StatementTypes>, intIntHash> affectsStarTTCalled;
    std::unordered_set<std::pair<int, StatementTypes>, intIntHash> affectsStarNTCalled;
    std::unordered_set<std::pair<StatementTypes, int>, intIntHash> affectsStarTNCalled;
};

#endif //AUTOTESTER_AFFECTSCACHE_H
