//
// // Created by Toh Yu Ting on 7/11/21.
//
#pragma once
#include "NextBipStarCache.h"
#include "Relationship.h"
#include "Relationship.cpp"
#include "Table.cpp"
#include "Definitions.h"

Relationship<int, int> NextBipStarCache::nextBipStar;
NextBipStarCache::NextBipStarCache() {}

void NextBipStarCache::clear() {
    nextBipStar.clear();
    nextBipStarNNCalled.clear();
    nextBipStarTTCalled.clear();
    nextBipStarNTCalled.clear();
    nextBipStarTNCalled.clear();
}

bool NextBipStarCache::addBipStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return nextBipStar.add(stmt1, type1, stmt2, type2);
}

bool NextBipStarCache::addNextBipStarNNCalled(int stmt1, int stmt2) {
    nextBipStarNNCalled.insert(make_pair(stmt1, stmt2));
    return true;
}

bool NextBipStarCache::isNextBipStarNNCalled(int stmt1, int stmt2) {
    if (nextBipStarNNCalled.find(make_pair(stmt1, stmt2)) != nextBipStarNNCalled.end()) {
        return true;
    }
    return false;
}

bool NextBipStarCache::addNextBipStarTTCalled(StatementTypes type1) {
    nextBipStarTTCalled.insert(make_pair(type1, NO_TYPE));
    return true;
}

bool NextBipStarCache::isNextBipStarTTCalled(StatementTypes type1) {
    if (nextBipStarTTCalled.find(make_pair(type1, NO_TYPE)) != nextBipStarTTCalled.end()) {
        return true;
    }
    return false;
}

bool NextBipStarCache::addNextBipStarNTCalled(int stmt1, StatementTypes type2) {
    nextBipStarNTCalled.insert(make_pair(stmt1, type2));
    return true;
}

bool NextBipStarCache::isNextBipStarNTCalled(int stmt1, StatementTypes type2) {
    if (nextBipStarNTCalled.find(make_pair(stmt1, type2)) != nextBipStarNTCalled.end()) {
        return true;
    }
    return false;
}

bool NextBipStarCache::addNextBipStarTNCalled(StatementTypes type1, int stmt2) {
    nextBipStarTNCalled.insert(make_pair(type1, stmt2));
    return true;
}

bool NextBipStarCache::isNextBipStarTNCalled(StatementTypes type1, int stmt2) {
    if (nextBipStarTNCalled.find(make_pair(type1, stmt2)) != nextBipStarTNCalled.end()) {
        return true;
    }
    return false;
}

LIST_OF_INT_PAIRS NextBipStarCache::getBipStarNN(int stmt1, int stmt2) {
    return nextBipStar.getNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS NextBipStarCache::getBipStarTT(StatementTypes type1, StatementTypes type2) {
    return nextBipStar.getTT(type1, type2);
}

LIST_OF_INT_PAIRS NextBipStarCache::getBipStarNT(int stmt1, StatementTypes type2) {
    return nextBipStar.getNT(stmt1, type2);
}

LIST_OF_INT_PAIRS NextBipStarCache::getBipStarTN(StatementTypes type1, int stmt2) {
    return nextBipStar.getTN(type1, stmt2);
}
