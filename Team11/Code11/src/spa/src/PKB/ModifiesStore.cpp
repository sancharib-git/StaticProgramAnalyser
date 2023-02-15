//
// Created by Yen Peng Yip on 3/10/21.
//

#pragma once
#include <string>
#include "ModifiesStore.h"
#include "Relationship.h"
#include "Relationship.cpp"
#include "Table.cpp"

Relationship<int, string> ModifiesStore::modifiesS;
Relationship<string, string> ModifiesStore::modifiesP;

ModifiesStore::ModifiesStore() {}

bool ModifiesStore::addS(int stmtNum, StatementTypes type, std::string varUsed) {
    return modifiesS.add(stmtNum, type, varUsed, type);
}

LIST_OF_INTSTR_PAIRS ModifiesStore::getSSV(int stmt1, string var2) {
    return modifiesS.getNN(stmt1, var2);
}

LIST_OF_INTSTR_PAIRS ModifiesStore::getSTT(StatementTypes type1, StatementTypes type2) {
    return modifiesS.getTT(type1, type2);
}

LIST_OF_INTSTR_PAIRS ModifiesStore::getSST(int stmt1, StatementTypes type2) {
    return modifiesS.getNT(stmt1, type2);
}

LIST_OF_INTSTR_PAIRS ModifiesStore::getSTV(StatementTypes type1, string var2) {
    return modifiesS.getTN(type1, var2);
}

bool ModifiesStore::addP(string procName, StatementTypes type, std::string varUsed) {
    return modifiesP.add(procName, type, varUsed, type);
}

LIST_OF_STR_PAIRS ModifiesStore::getPPV(string proc1, string var2) {
    return modifiesP.getNN(proc1, var2);
}

LIST_OF_STR_PAIRS ModifiesStore::getPTT(StatementTypes type1, StatementTypes type2) {
    return modifiesP.getTT(type1, type2);
}

LIST_OF_STR_PAIRS ModifiesStore::getPPT(string proc1, StatementTypes type2) {
    return modifiesP.getNT(proc1, type2);
}

LIST_OF_STR_PAIRS ModifiesStore::getPTV(StatementTypes type1, string var2) {
    return modifiesP.getTN(type1, var2);
}

void ModifiesStore::clear() {
    modifiesS.clear();
    modifiesP.clear();
}