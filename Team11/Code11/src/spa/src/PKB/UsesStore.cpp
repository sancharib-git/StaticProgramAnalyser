//
// Created by Yen Peng Yip on 3/10/21.
//

#pragma once
#include <string>
#include "UsesStore.h"
#include "Relationship.h"
#include "Relationship.cpp"
#include "Table.cpp"

Relationship<int, string> UsesStore::usesS;
Relationship<string, string> UsesStore::usesP;

UsesStore::UsesStore() {}

bool UsesStore::addS(int stmtNum, StatementTypes type, std::string varUsed) {
    return usesS.add(stmtNum, type, varUsed, type);
}

LIST_OF_INTSTR_PAIRS UsesStore::getSSV(int stmt1, string var2) {
    return usesS.getNN(stmt1, var2);
}

LIST_OF_INTSTR_PAIRS UsesStore::getSTT(StatementTypes type1, StatementTypes type2) {
    return usesS.getTT(type1, type2);
}

LIST_OF_INTSTR_PAIRS UsesStore::getSST(int stmt1, StatementTypes type2) {
    return usesS.getNT(stmt1, type2);
}
LIST_OF_INTSTR_PAIRS UsesStore::getSTV(StatementTypes type1, string var2) {
    return usesS.getTN(type1, var2);
}

bool UsesStore::addP(string procName, StatementTypes type, std::string varUsed) {
    return usesP.add(procName, type, varUsed, type);
}

LIST_OF_STR_PAIRS UsesStore::getPPV(string proc1, string var2) {
    return usesP.getNN(proc1, var2);
}

LIST_OF_STR_PAIRS UsesStore::getPTT(StatementTypes type1, StatementTypes type2) {
    return usesP.getTT(type1, type2);
}

LIST_OF_STR_PAIRS UsesStore::getPPT(string proc1, StatementTypes type2) {
    return usesP.getNT(proc1, type2);
}

LIST_OF_STR_PAIRS UsesStore::getPTV(StatementTypes type1, string var2) {
    return usesP.getTN(type1, var2);
}

void UsesStore::clear() {
    usesS.clear();
    usesP.clear();
}
