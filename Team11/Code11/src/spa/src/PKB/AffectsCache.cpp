//
// Created by Yen Peng Yip on 31/10/21.
//
#pragma once
#include "AffectsCache.h"
#include "Relationship.h"
#include "Relationship.cpp"
#include "Table.cpp"
#include "Definitions.h"

Relationship<int, int> AffectsCache::affects;
Relationship<int, int> AffectsCache::affectsStar;

AffectsCache::AffectsCache() {}

void AffectsCache::clear() {
    affects.clear();
    affectsStar.clear();
    affectsNNCalled.clear();
    affectsTTCalled.clear();
    affectsNTCalled.clear();
    affectsTNCalled.clear();
    affectsStarNNCalled.clear();
    affectsStarTTCalled.clear();
    affectsStarNTCalled.clear();
    affectsStarTNCalled.clear();
}

bool AffectsCache::add(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return affects.add(stmt1, type1, stmt2, type2);
}

bool AffectsCache::addAffectsNNCalled(int stmt1, int stmt2) {
    affectsNNCalled.insert(make_pair(stmt1, stmt2));
    return true;
}

bool AffectsCache::isAffectsNNCalled(int stmt1, int stmt2) {
    if (affectsNNCalled.find(make_pair(stmt1, stmt2)) != affectsNNCalled.end()) {
        return true;
    }
    return false;
}

bool AffectsCache::addAffectsNTCalled(int stmt1, StatementTypes type2) {
    affectsNTCalled.insert(make_pair(stmt1, type2));
    return true;
}

bool AffectsCache::isAffectsNTCalled(int stmt1, StatementTypes type2) {
    if (affectsNTCalled.find(make_pair(stmt1, type2)) != affectsNTCalled.end()) {
        return true;
    }
    return false;
}

bool AffectsCache::addAffectsTTCalled(StatementTypes type1, StatementTypes type2) {
    affectsTTCalled.insert(make_pair(type1, type2));
    return true;
}

bool AffectsCache::isAffectsTTCalled(StatementTypes type1, StatementTypes type2) {
    if (affectsTTCalled.find(make_pair(type1, type2)) != affectsTTCalled.end()) {
        return true;
    }
    return false;
}

bool AffectsCache::addAffectsTNCalled(StatementTypes type1, int stmt2) {
    affectsTNCalled.insert(make_pair(type1, stmt2));
    return true;
}

bool AffectsCache::isAffectsTNCalled(StatementTypes type1, int stmt2) {
    if (affectsTNCalled.find(make_pair(type1, stmt2)) != affectsTNCalled.end()) {
        return true;
    }
    return false;
}

LIST_OF_INT_PAIRS AffectsCache::getNN(int stmt1, int stmt2) {
    LIST_OF_INT_PAIRS results = affects.getNN(stmt1, stmt2);
    return results;
}
LIST_OF_INT_PAIRS AffectsCache::getTT(StatementTypes type1, StatementTypes type2) {
    LIST_OF_INT_PAIRS results = affects.getTT(type1, type2);
    return results;
}
LIST_OF_INT_PAIRS AffectsCache::getNT(int stmt1, StatementTypes type2) {
    LIST_OF_INT_PAIRS results = affects.getNT(stmt1, type2);
    return results;
}
LIST_OF_INT_PAIRS AffectsCache::getTN(StatementTypes type1, int stmt2) {
    LIST_OF_INT_PAIRS results = affects.getTN(type1, stmt2);
    return results;
}

bool AffectsCache::addStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    if (stmt1 <= 0 || stmt2 <= 0 || stmt2 <= stmt1 || type1 >= 0 || type2 >= 0) {
        return false;
    }
    return affectsStar.add(stmt1, type1, stmt2, type2);
}

bool AffectsCache::addAffectsStarNNCalled(int stmt1, int stmt2) {
    affectsStarNNCalled.insert(make_pair(stmt1, stmt2));
    return true;
}

bool AffectsCache::isAffectsStarNNCalled(int stmt1, int stmt2) {
    if (affectsStarNNCalled.find(make_pair(stmt1, stmt2)) != affectsStarNNCalled.end()) {
        return true;
    }
    return false;
}

bool AffectsCache::addAffectsStarNTCalled(int stmt1, StatementTypes type2) {
    affectsStarNTCalled.insert(make_pair(stmt1, type2));
    return true;
}

bool AffectsCache::isAffectsStarNTCalled(int stmt1, StatementTypes type2) {
    if (affectsStarNTCalled.find(make_pair(stmt1, type2)) != affectsStarNTCalled.end()) {
        return true;
    }
    return false;
}

bool AffectsCache::addAffectsStarTTCalled(StatementTypes type1, StatementTypes type2) {
    affectsStarTTCalled.insert(make_pair(type1, type2));
    return true;
}

bool AffectsCache::isAffectsStarTTCalled(StatementTypes type1, StatementTypes type2) {
    if (affectsStarTTCalled.find(make_pair(type1, type2)) != affectsStarTTCalled.end()) {
        return true;
    }
    return false;
}

bool AffectsCache::addAffectsStarTNCalled(StatementTypes type1, int stmt2) {
    affectsStarTNCalled.insert(make_pair(type1, stmt2));
    return true;
}

bool AffectsCache::isAffectsStarTNCalled(StatementTypes type1, int stmt2) {
    if (affectsStarTNCalled.find(make_pair(type1, stmt2)) != affectsStarTNCalled.end()) {
        return true;
    }
    return false;
}

LIST_OF_INT_PAIRS AffectsCache::getStarNN(int stmt1, int stmt2) {
    LIST_OF_INT_PAIRS results = affectsStar.getNN(stmt1, stmt2);
    return results;
}
LIST_OF_INT_PAIRS AffectsCache::getStarTT(StatementTypes type1, StatementTypes type2) {
    LIST_OF_INT_PAIRS results = affectsStar.getTT(type1, type2);
    return results;
}
LIST_OF_INT_PAIRS AffectsCache::getStarNT(int stmt1, StatementTypes type2) {
    LIST_OF_INT_PAIRS results = affectsStar.getNT(stmt1, type2);
    return results;
}
LIST_OF_INT_PAIRS AffectsCache::getStarTN(StatementTypes type1, int stmt2) {
    LIST_OF_INT_PAIRS results = affectsStar.getTN(type1, stmt2);
    return results;
}
