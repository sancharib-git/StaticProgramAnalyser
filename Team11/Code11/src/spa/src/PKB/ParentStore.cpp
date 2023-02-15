//
// Created by Yen Peng Yip on 3/10/21.
//

#pragma once
#include "Relationship.h"
#include "Relationship.cpp"
#include "Table.cpp"
#include "ParentStore.h"

Relationship<int, int> ParentStore::parent;
Relationship<int, int> ParentStore::parentStar;

ParentStore::ParentStore() {}

void ParentStore::clear() {
    parent.clear();
    parentStar.clear();
}

bool ParentStore::add(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    // invalid follow stmts
    if (stmt1 <= 0 || stmt2 <= 0 || stmt2 <= stmt1 || type1 >= 0 || type2 >= 0) {
        return false;
    }
    return parent.add(stmt1, type1, stmt2, type2);
}

std::vector<std::pair<int, int> > ParentStore::getNN(int stmt1, int stmt2) {
    return parent.getNN(stmt1, stmt2);
}
std::vector<std::pair<int, int> > ParentStore::getTT(StatementTypes type1, StatementTypes type2) {
    return parent.getTT(type1, type2);
}
std::vector<std::pair<int, int> > ParentStore::getNT(int stmt1, StatementTypes type2) {
    return parent.getNT(stmt1, type2);
}
std::vector<std::pair<int, int> > ParentStore::getTN(StatementTypes type1, int stmt2) {
    return parent.getTN(type1, stmt2);
}

bool ParentStore::addStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    if (stmt1 <= 0 || stmt2 <= 0 || stmt2 <= stmt1 || type1 >= 0 || type2 >= 0) {
        return false;
    }
    return parentStar.add(stmt1, type1, stmt2, type2);
}

std::vector<std::pair<int, int> > ParentStore::getStarNN(int stmt1, int stmt2) {
    return parentStar.getNN(stmt1, stmt2);
}

std::vector<std::pair<int, int> > ParentStore::getStarTT(StatementTypes type1, StatementTypes type2) {
    return parentStar.getTT(type1, type2);
}

std::vector<std::pair<int, int> > ParentStore::getStarNT(int stmt1, StatementTypes type2) {
    return parentStar.getNT(stmt1, type2);
}

std::vector<std::pair<int, int> > ParentStore::getStarTN(StatementTypes type1, int stmt2) {
    return parentStar.getTN(type1, stmt2);
}
