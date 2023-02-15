//
// Created by Yen Peng Yip on 6/10/21.
//
#pragma once
#include "NextStore.h"
#include "Relationship.h"
#include "Relationship.cpp"
#include "Table.cpp"
#include "DE/DesignExtractor.h"
#include "Definitions.h"

Relationship<int, int> NextStore::next;

NextStore::NextStore() {}

bool NextStore::add(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return next.add(stmt1, type1, stmt2, type2);
}
LIST_OF_INT_PAIRS NextStore::getNN(int stmt1, int stmt2) {
    return next.getNN(stmt1, stmt2);
}
LIST_OF_INT_PAIRS NextStore::getTT(StatementTypes type1, StatementTypes type2) {
    return next.getTT(type1, type2);
}
LIST_OF_INT_PAIRS NextStore::getNT(int stmt1, StatementTypes type2) {
    return next.getNT(stmt1, type2);
}
LIST_OF_INT_PAIRS NextStore::getTN(StatementTypes type1, int stmt2) {
    return next.getTN(type1, stmt2);
}
void NextStore::clear() {
    next.clear();
}
