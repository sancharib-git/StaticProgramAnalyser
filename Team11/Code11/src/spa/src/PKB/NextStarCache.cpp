//
// Created by Yen Peng Yip on 28/10/21.
//
#pragma once
#include "NextStarCache.h"
#include "Relationship.h"
#include "Relationship.cpp"
#include "Table.cpp"
#include "Definitions.h"

Relationship<int, int> NextStarCache::nextStar;
NextStarCache::NextStarCache() {}

void NextStarCache::clear() {
    nextStar.clear();
    nextStarNNCalled.clear();
    nextStarTTCalled.clear();
    nextStarNTCalled.clear();
    nextStarTNCalled.clear();
}

bool NextStarCache::addStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return nextStar.add(stmt1, type1, stmt2, type2);
}

bool NextStarCache::addNextStarNNCalled(int stmt1, int stmt2) {
    nextStarNNCalled.insert(make_pair(stmt1, stmt2));
    return true;
}

bool NextStarCache::isNextStarNNCalled(int stmt1, int stmt2) {
    if (nextStarNNCalled.find(make_pair(stmt1, stmt2)) != nextStarNNCalled.end()) {
        return true;
    }
    return false;
}

bool NextStarCache::addNextStarTTCalled(StatementTypes type1) {
    nextStarTTCalled.insert(make_pair(type1, NO_TYPE));
    return true;
}

bool NextStarCache::isNextStarTTCalled(StatementTypes type1) {
    if (nextStarTTCalled.find(make_pair(type1, NO_TYPE)) != nextStarTTCalled.end()) {
        return true;
    }
    return false;
}

bool NextStarCache::addNextStarNTCalled(int stmt1, StatementTypes type2) {
    nextStarNTCalled.insert(make_pair(stmt1, type2));
    return true;
}

bool NextStarCache::isNextStarNTCalled(int stmt1, StatementTypes type2) {
    if (nextStarNTCalled.find(make_pair(stmt1, type2)) != nextStarNTCalled.end()) {
        return true;
    }
    return false;
}

bool NextStarCache::addNextStarTNCalled(StatementTypes type1, int stmt2) {
    nextStarTNCalled.insert(make_pair(type1, stmt2));
    return true;
}

bool NextStarCache::isNextStarTNCalled(StatementTypes type1, int stmt2) {
    if (nextStarTNCalled.find(make_pair(type1, stmt2)) != nextStarTNCalled.end()) {
        return true;
    }
    return false;
}

LIST_OF_INT_PAIRS NextStarCache::getStarNN(int stmt1, int stmt2) {
    return nextStar.getNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS NextStarCache::getStarTT(StatementTypes type1, StatementTypes type2) {
    return nextStar.getTT(type1, type2);
}

LIST_OF_INT_PAIRS NextStarCache::getStarNT(int stmt1, StatementTypes type2) {
    return nextStar.getNT(stmt1, type2);
}

LIST_OF_INT_PAIRS NextStarCache::getStarTN(StatementTypes type1, int stmt2) {
    return nextStar.getTN(type1, stmt2);
}
