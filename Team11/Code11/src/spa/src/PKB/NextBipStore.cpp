//
// Created by Yen Peng Yip on 24/10/21.
//
#pragma once
#include "NextBipStore.h"
#include "Relationship.h"
#include "Relationship.cpp"
#include "Table.cpp"
#include "Definitions.h"

Relationship<int, int> NextBipStore::nextBip;
bool NextBipStore::isNextBipComputed = false;


NextBipStore::NextBipStore() {}

bool NextBipStore::getIsNextBipComputed() {
    return isNextBipComputed;
}

bool NextBipStore::setNextBipComputed() {
    return isNextBipComputed = true;
}

void NextBipStore::clear() {
    nextBip.clear();
}

bool NextBipStore::add(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return nextBip.add(stmt1, type1, stmt2, type2);
}

LIST_OF_INT_PAIRS NextBipStore::getNN(int stmt1, int stmt2) {
    return nextBip.getNN(stmt1, stmt2);
}
LIST_OF_INT_PAIRS NextBipStore::getTT(StatementTypes type1, StatementTypes type2) {
    return nextBip.getTT(type1, type2);
}
LIST_OF_INT_PAIRS NextBipStore::getNT(int stmt1, StatementTypes type2) {
    return nextBip.getNT(stmt1, type2);
}
LIST_OF_INT_PAIRS NextBipStore::getTN(StatementTypes type1, int stmt2) {
    return nextBip.getTN(type1, stmt2);
}
/*
bool NextBipCache::addStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    if (stmt1 <= 0 || stmt2 <= 0 || stmt2 <= stmt1 || type1 >= 0 || type2 >= 0) {
        return false;
    }
    return nextBipStar.add(stmt1, type1, stmt2, type2);
}
LIST_OF_INT_PAIRS NextBipCache::getStarNN(int stmt1, int stmt2) {
    return nextBipStar.getNN(stmt1, stmt2);
}
LIST_OF_INT_PAIRS NextBipCache::getStarTT(StatementTypes type1, StatementTypes type2) {
    return nextBipStar.getTT(type1, type2);
}
LIST_OF_INT_PAIRS NextBipCache::getStarNT(int stmt1, StatementTypes type2) {
    return nextBipStar.getNT(stmt1, type2);
}
LIST_OF_INT_PAIRS NextBipCache::getStarTN(StatementTypes type1, int stmt2) {
    return nextBipStar.getTN(type1, stmt2);
}*/
