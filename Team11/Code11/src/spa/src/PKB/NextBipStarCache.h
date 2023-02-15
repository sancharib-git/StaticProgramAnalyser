//
// Created by Toh Yu Ting on 7/11/21.
//

#pragma once
#include <vector>
#include <unordered_set>
#include <utility>
#include "StatementTypes.h"
#include "Relationship.h"
#include "Definitions.h"
#include "Hasher.h"

class NextBipStarCache {
public:
    NextBipStarCache();
    virtual void clear();

    virtual bool addBipStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2);
    bool addNextBipStarNNCalled(int stmt1, int stmt2);
    bool isNextBipStarNNCalled(int stmt1, int stmt2);
    bool addNextBipStarTTCalled(StatementTypes type1);
    bool isNextBipStarTTCalled(StatementTypes type1);
    bool addNextBipStarNTCalled(int stmt1, StatementTypes type2);
    bool isNextBipStarNTCalled(int stmt1, StatementTypes type2);
    bool addNextBipStarTNCalled(StatementTypes type1, int stmt2);
    bool isNextBipStarTNCalled(StatementTypes type1, int stmt2);
    virtual LIST_OF_INT_PAIRS getBipStarNN(int stmt1, int stmt2);
    virtual LIST_OF_INT_PAIRS getBipStarTT(StatementTypes type1, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getBipStarNT(int stmt1, StatementTypes type2);
    virtual LIST_OF_INT_PAIRS getBipStarTN(StatementTypes type1, int stmt2);

private:
    static Relationship<int, int> nextBipStar;
    std::unordered_set<std::pair<int, int>, intIntHash> nextBipStarNNCalled;
    std::unordered_set<std::pair<StatementTypes, StatementTypes>, intIntHash> nextBipStarTTCalled;
    std::unordered_set<std::pair<int, StatementTypes>, intIntHash> nextBipStarNTCalled;
    std::unordered_set<std::pair<StatementTypes, int>, intIntHash> nextBipStarTNCalled;
};
