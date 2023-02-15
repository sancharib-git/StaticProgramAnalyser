//
// Created by Yen Peng Yip on 24/10/21.
//
#pragma once
#include "Cache.h"

using namespace std;

NextBipStarCache Cache::nextBipStarCache;
NextStarCache Cache::nextStarCache;
AffectsCache Cache::affectsCache;


//////////////////////NEXTBIP STAR///////////////////////////////////////

bool Cache::addNextBipStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return nextBipStarCache.addBipStar(stmt1, type1, stmt2, type2);
}

bool Cache::addNextBipStarNNCalled(int stmt1, int stmt2) {
    return nextBipStarCache.addNextBipStarNNCalled(stmt1, stmt2);
}
bool Cache::isNextBipStarNNCalled(int stmt1, int stmt2) {
    return nextBipStarCache.isNextBipStarNNCalled(stmt1, stmt2);
}

// populates and checks by (type1, _)
bool Cache::addNextBipStarTTCalled(StatementTypes type1) {
    return nextBipStarCache.addNextBipStarTTCalled(type1);
}
bool Cache::isNextBipStarTTCalled(StatementTypes type1) {
    return nextBipStarCache.isNextBipStarTTCalled(type1);
}

bool Cache::addNextBipStarNTCalled(int stmt1, StatementTypes type2) {
    return nextBipStarCache.addNextBipStarNTCalled(stmt1, type2);
}
bool Cache::isNextBipStarNTCalled(int stmt1, StatementTypes type2) {
    return nextBipStarCache.isNextBipStarNTCalled(stmt1, type2);
}

bool Cache::addNextBipStarTNCalled(StatementTypes type1, int stmt2) {
    return nextBipStarCache.addNextBipStarTNCalled(type1, stmt2);
}
bool Cache::isNextBipStarTNCalled(StatementTypes type1, int stmt2) {
    return nextBipStarCache.isNextBipStarTNCalled(type1, stmt2);
}


vector<std::pair<int, int> > Cache::getNextBipStarNN(int stmt1, int stmt2) {
    return nextBipStarCache.getBipStarNN(stmt1, stmt2);
}

vector<std::pair<int, int> > Cache::getNextBipStarTT(StatementTypes type1, StatementTypes type2) {
    return nextBipStarCache.getBipStarTT(type1, type2);
}

vector<std::pair<int, int> > Cache::getNextBipStarNT(int stmt1, StatementTypes type2) {
    return nextBipStarCache.getBipStarNT(stmt1, type2);
}

vector<std::pair<int, int> > Cache::getNextBipStarTN(StatementTypes type1, int stmt2) {
    return nextBipStarCache.getBipStarTN(type1, stmt2);
}

//////////////////////NEXT STAR///////////////////////////////////////

bool Cache::addNextStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return nextStarCache.addStar(stmt1, type1, stmt2, type2);
}

bool Cache::addNextStarNNCalled(int stmt1, int stmt2) {
    return nextStarCache.addNextStarNNCalled(stmt1, stmt2);
}

bool Cache::isNextStarNNCalled(int stmt1, int stmt2) {
    return nextStarCache.isNextStarNNCalled(stmt1, stmt2);
}

bool Cache::addNextStarTTCalled(StatementTypes type1) {
    return nextStarCache.addNextStarTTCalled(type1);
}

bool Cache::isNextStarTTCalled(StatementTypes type1) {
    return nextStarCache.isNextStarTTCalled(type1);
}

bool Cache::addNextStarNTCalled(int stmt1, StatementTypes type2) {
    return nextStarCache.addNextStarNTCalled(stmt1, type2);
}

bool Cache::isNextStarNTCalled(int stmt1, StatementTypes type2) {
    return nextStarCache.isNextStarNTCalled(stmt1, type2);
}

bool Cache::addNextStarTNCalled(StatementTypes type1, int stmt2) {
    return nextStarCache.addNextStarTNCalled(type1, stmt2);
}

bool Cache::isNextStarTNCalled(StatementTypes type1, int stmt2) {
    return nextStarCache.isNextStarTNCalled(type1, stmt2);
}

vector<std::pair<int, int> > Cache::getNextStarNN(int stmt1, int stmt2) {
    return nextStarCache.getStarNN(stmt1, stmt2);
}

vector<std::pair<int, int> > Cache::getNextStarTT(StatementTypes type1, StatementTypes type2) {
    return nextStarCache.getStarTT(type1, type2);
}

vector<std::pair<int, int> > Cache::getNextStarNT(int stmt1, StatementTypes type2) {
    return nextStarCache.getStarNT(stmt1, type2);
}

vector<std::pair<int, int> > Cache::getNextStarTN(StatementTypes type1, int stmt2) {
    return nextStarCache.getStarTN(type1, stmt2);
}

bool Cache::addAffects(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return affectsCache.add(stmt1, type1, stmt2, type2);
}

vector<std::pair<int, int> > Cache::getAffectsNN(int stmt1, int stmt2) {
    return affectsCache.getNN(stmt1, stmt2);
}
bool Cache::addAffectsNNCalled(int stmt1, int stmt2) {
    return affectsCache.addAffectsNNCalled(stmt1, stmt2);
}
bool Cache::isAffectsNNCalled(int stmt1, int stmt2) {
    return affectsCache.isAffectsNNCalled(stmt1, stmt2);
}

bool Cache::addAffectsNTCalled(int stmt1, StatementTypes type2) {
    return affectsCache.addAffectsNTCalled(stmt1, type2);
}
bool Cache::isAffectsNTCalled(int stmt1, StatementTypes type2) {
    return affectsCache.isAffectsNTCalled(stmt1, type2);
}

bool Cache::addAffectsTTCalled(StatementTypes type1, StatementTypes type2) {
    return affectsCache.addAffectsTTCalled(type1, type2);
}

bool Cache::isAffectsTTCalled(StatementTypes type1, StatementTypes type2) {
    return affectsCache.isAffectsTTCalled(type1, type2);
}

bool Cache::addAffectsTNCalled(StatementTypes type1, int stmt2) {
    return affectsCache.addAffectsTNCalled(type1, stmt2);
}

bool Cache::isAffectsTNCalled(StatementTypes type1, int stmt2) {
    return affectsCache.isAffectsTNCalled(type1, stmt2);
}

vector<std::pair<int, int> > Cache::getAffectsTT(StatementTypes type1, StatementTypes type2) {
    return affectsCache.getTT(type1, type2);
}

vector<std::pair<int, int> > Cache::getAffectsNT(int stmt1, StatementTypes type2) {
    return affectsCache.getNT(stmt1, type2);
}

vector<std::pair<int, int> > Cache::getAffectsTN(StatementTypes type1, int stmt2) {
    return affectsCache.getTN(type1, stmt2);
}

////////////////////////
bool Cache::addAffectsStar(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return affectsCache.addStar(stmt1, type1, stmt2, type2);
}

bool Cache::addAffectsStarNNCalled(int stmt1, int stmt2) {
    return affectsCache.addAffectsStarNNCalled(stmt1, stmt2);
}
bool Cache::isAffectsStarNNCalled(int stmt1, int stmt2) {
    return affectsCache.isAffectsStarNNCalled(stmt1, stmt2);
}

bool Cache::addAffectsStarNTCalled(int stmt1, StatementTypes type2) {
    return affectsCache.addAffectsStarNTCalled(stmt1, type2);
}
bool Cache::isAffectsStarNTCalled(int stmt1, StatementTypes type2) {
    return affectsCache.isAffectsStarNTCalled(stmt1, type2);
}

bool Cache::addAffectsStarTTCalled(StatementTypes type1, StatementTypes type2) {
    return affectsCache.addAffectsStarTTCalled(type1, type2);
}

bool Cache::isAffectsStarTTCalled(StatementTypes type1, StatementTypes type2) {
    return affectsCache.isAffectsStarTTCalled(type1, type2);
}

bool Cache::addAffectsStarTNCalled(StatementTypes type1, int stmt2) {
    return affectsCache.addAffectsStarTNCalled(type1, stmt2);
}

bool Cache::isAffectsStarTNCalled(StatementTypes type1, int stmt2) {
    return affectsCache.isAffectsStarTNCalled(type1, stmt2);
}

vector<std::pair<int, int> > Cache::getAffectsStarNN(int stmt1, int stmt2) {
    return affectsCache.getStarNN(stmt1, stmt2);
}

vector<std::pair<int, int> > Cache::getAffectsStarTT(StatementTypes type1, StatementTypes type2) {
    return affectsCache.getStarTT(type1, type2);
}

vector<std::pair<int, int> > Cache::getAffectsStarNT(int stmt1, StatementTypes type2) {
    return affectsCache.getStarNT(stmt1, type2);
}

vector<std::pair<int, int> > Cache::getAffectsStarTN(StatementTypes type1, int stmt2) {
    return affectsCache.getStarTN(type1, stmt2);
}

/////////////// CLEAR ///////////////
void Cache::clear() {
    //nextBipCache.clear();
    nextStarCache.clear();
    affectsCache.clear();
}