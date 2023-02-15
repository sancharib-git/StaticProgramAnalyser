//
// Created by Yen Peng Yip on 3/10/21.
//
#pragma once
#include "Relationship.h"
#include "Relationship.cpp"
#include "Table.cpp"
#include "FollowsStore.h"
#include "Definitions.h"

Relationship<int, int> FollowsStore::follows;
Relationship<int, int> FollowsStore::followsStar;

FollowsStore::FollowsStore() {}

void FollowsStore::clear() {
    follows.clear();
    followsStar.clear();
}

bool FollowsStore::add(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    if (stmt1 <= 0 || stmt2 <= 0 || stmt2 <= stmt1 || type1 >= 0 || type2 >= 0) {
        return false;
    }
    return follows.add(stmt1, type1, stmt2, type2);
}

LIST_OF_INT_PAIRS FollowsStore::getNN(int stmt1, int stmt2) {
    return follows.getNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS FollowsStore::getTT(StatementTypes type1, StatementTypes type2) {
    return follows.getTT(type1, type2);
}

LIST_OF_INT_PAIRS FollowsStore::getNT(int stmt1, StatementTypes type2) {
    return follows.getNT(stmt1, type2);
}

LIST_OF_INT_PAIRS FollowsStore::getTN(StatementTypes type1, int stmt2) {
    return follows.getTN(type1, stmt2);
}

bool FollowsStore::addStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    if (stmt1 <= 0 || stmt2 <= 0 || stmt2 <= stmt1 || type1 >= 0 || type2 >= 0) {
        return false;
    }
    return followsStar.add(stmt1, type1, stmt2, type2);
}


LIST_OF_INT_PAIRS FollowsStore::getStarNN(int stmt1, int stmt2) {
    return followsStar.getNN(stmt1, stmt2);
}

LIST_OF_INT_PAIRS FollowsStore::getStarTT(StatementTypes type1, StatementTypes type2) {
    return followsStar.getTT(type1, type2);
}

LIST_OF_INT_PAIRS FollowsStore::getStarNT(int stmt1, StatementTypes type2) {
    return followsStar.getNT(stmt1, type2);
}

LIST_OF_INT_PAIRS FollowsStore::getStarTN(StatementTypes type1, int stmt2) {
    return followsStar.getTN(type1, stmt2);
}
