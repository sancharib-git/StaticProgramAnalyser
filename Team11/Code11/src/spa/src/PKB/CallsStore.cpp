//
// Created by Yen Peng Yip on 3/10/21.
//
#pragma once
#include "CallsStore.h"
#include "Relationship.h"
#include "Relationship.cpp"
#include "Table.cpp"
#include "Definitions.h"

Relationship<int, string> CallsStore::callsS;
Relationship<string, string> CallsStore::callsP;
Relationship<string, string> CallsStore::callsStar;

CallsStore::CallsStore() {}

bool CallsStore::addS(int stmtNum, StatementTypes type, string procCalled) {
    return callsS.add(stmtNum, type, procCalled, type);
}

LIST_OF_INTSTR_PAIRS CallsStore::getSSP(int stmtNum, string procCalled) {
    return callsS.getNN(stmtNum, procCalled);
}

LIST_OF_INTSTR_PAIRS CallsStore::getSTT(StatementTypes type1, StatementTypes type2) {
    return callsS.getTT(type1, type2);
}

LIST_OF_INTSTR_PAIRS CallsStore::getSST(int stmtNum, StatementTypes type2) {
    return callsS.getNT(stmtNum, type2);
}

LIST_OF_INTSTR_PAIRS CallsStore::getSTP(StatementTypes type1, string procCalled) {
    return callsS.getTN(type1, procCalled);
}

bool CallsStore::addP(string procName, StatementTypes type, string procCalled) {
    return callsP.add(procName, type, procCalled, type);
}

LIST_OF_STR_PAIRS CallsStore::getPPP(string procName, string procCalled) {
    return callsP.getNN(procName, procCalled);
}

LIST_OF_STR_PAIRS CallsStore::getPTT(StatementTypes type1, StatementTypes type2) {
    return callsP.getTT(type1, type2);
}

LIST_OF_STR_PAIRS CallsStore::getPPT(string procName, StatementTypes type2) {
    return callsP.getNT(procName, type2);
}

LIST_OF_STR_PAIRS CallsStore::getPTP(StatementTypes type1, string procCalled) {
    return callsP.getTN(type1, procCalled);
}

/////////////// CALLS STAR PROCEDURE API ///////////////

bool CallsStore::addStar(string procName, string procCalled) {
    return callsStar.add(procName, NO_TYPE, procCalled, NO_TYPE);
}

LIST_OF_STR_PAIRS CallsStore::getStarNN(string procName, string procCalled) {
    return callsStar.getNN(procName, procCalled);
}

LIST_OF_STR_PAIRS CallsStore::getStarTT(StatementTypes type1, StatementTypes type2) {
    return callsStar.getTT(type1, type2);
}

LIST_OF_STR_PAIRS CallsStore::getStarNT(string procName, StatementTypes type2) {
    return callsStar.getNT(procName, type2);
}

LIST_OF_STR_PAIRS CallsStore::getStarTN(StatementTypes type1, string procCalled) {
    return callsStar.getTN(type1, procCalled);
}

void CallsStore::clear() {
    callsS.clear();
    callsP.clear();
    callsStar.clear();
}