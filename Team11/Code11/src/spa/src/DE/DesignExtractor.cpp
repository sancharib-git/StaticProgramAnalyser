#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "DesignExtractor.h"
#include "ParserException.h"
#include "Graph.h"
#include "TransitiveExtractor.h"
#include "UsesModifiesExtractor.h"
#include "AssignmentExtractor.h"
#include "NextExtractor.h"
#include "NextStarExtractor.h"
#include "NextBipExtractor.h"
#include "NextBipStarExtractor.h"
#include "AffectsExtractor.h"
#include "AffectsBipExtractor.h"
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "PKB/PKB.h"
#include "PKB/CallsStore.h"
#include "StatementTypes.h"
#include "BinaryTree.h"
#include "TreeProcessor.h"
#include <cstdio>
#include <ctime>

using namespace std;

bool DesignExtractor::populateDesignAbstractions() {
    DesignExtractor* newExtractor = this;
    TransitiveExtractor* transitiveExtractor = new TransitiveExtractor();
    UsesModifiesExtractor* usesModifiesExtractor = new UsesModifiesExtractor();
    NextExtractor* nextExtractor = new NextExtractor();
    // populate Follows*/Parent*/Calls* relationship
    bool isFollowsParentCallStarSuccess = transitiveExtractor->addTransitiveRelationship();

    bool isUsesModifiesSuccess = usesModifiesExtractor->extractUsesModifies();

    bool isNextSuccess = nextExtractor->extractNext();

    delete transitiveExtractor;
    delete usesModifiesExtractor;
    delete nextExtractor;

    return (isFollowsParentCallStarSuccess && isUsesModifiesSuccess && isNextSuccess);
}

bool DesignExtractor::populateAssignmentSubExpressions() {
    AssignmentExtractor* assignmentExtractor = new AssignmentExtractor();
    bool isSuccess = assignmentExtractor->populateAssignmentSubExpressions();
    delete assignmentExtractor;
    return isSuccess;
}

// static method to interface with PKB to extractNextBip
bool DesignExtractor::extractNextBip() {
    return NextBipExtractor::extractNextBip();
}

vector<std::pair<int, int>> DesignExtractor::getNextStarNN(int stmt1, int stmt2) {
    return NextStarExtractor::getNextStarNN(stmt1, stmt2);
}

vector<std::pair<int, int>> DesignExtractor::getNextStarNT(int stmt1, StatementTypes stmt2Type) {
    return NextStarExtractor::getNextStarNT(stmt1, stmt2Type);
}

void DesignExtractor::setNextStarNTBoolean(bool isBooleanClause) {
    return NextStarExtractor::setNextStarNTBoolean(isBooleanClause);
}

void DesignExtractor::setNextStarTNBoolean(bool isBooleanClause) {
    return NextStarExtractor::setNextStarTNBoolean(isBooleanClause);
}

void DesignExtractor::setNextStarTTBoolean(bool isBooleanClause) {
    return NextStarExtractor::setNextStarTTBoolean(isBooleanClause);
}

vector<std::pair<int, int>> DesignExtractor::getNextStarTN(StatementTypes stmt1Type, int stmt2) {
    return NextStarExtractor::getNextStarTN(stmt1Type, stmt2);
}
vector<std::pair<int, int>> DesignExtractor::getNextStarTT(StatementTypes stmt1Type,
    StatementTypes stmt2Type) {
    return NextStarExtractor::getNextStarTT(stmt1Type, stmt2Type);
}

vector<pair<int, int>> DesignExtractor::getAffectsNN(int stmt1, int stmt2) {
    return AffectsExtractor::getAffectsNN(stmt1, stmt2);
}

vector<pair<int, int>> DesignExtractor::getAffectsNT(int stmt1, StatementTypes stmt2Type) {
    return AffectsExtractor::getAffectsNT(stmt1, stmt2Type);
}

vector<pair<int, int>> DesignExtractor::getAffectsTN(StatementTypes stmt1Type, int stmt2) {
    return AffectsExtractor::getAffectsTN(stmt1Type, stmt2);
}

vector<pair<int, int>> DesignExtractor::getAffectsTT(StatementTypes stmt1Type, StatementTypes stmt2Type) {
    return AffectsExtractor::getAffectsTT(stmt1Type, stmt2Type);
}

vector<pair<int, int>> DesignExtractor::getAffectsStarNN(int stmt1, int stmt2) {
    return AffectsExtractor::getAffectsStarNN(stmt1, stmt2);
}

vector<pair<int, int>> DesignExtractor::getAffectsStarNT(int stmt1, StatementTypes stmt2Type) {
    return AffectsExtractor::getAffectsStarNT(stmt1, stmt2Type);
}

vector<pair<int, int>> DesignExtractor::getAffectsStarTN(StatementTypes stmt1Type, int stmt2) {
    return AffectsExtractor::getAffectsStarTN(stmt1Type, stmt2);
}

vector<pair<int, int>> DesignExtractor::getAffectsStarTT(StatementTypes stmt1Type, StatementTypes stmt2Type) {
    return AffectsExtractor::getAffectsStarTT(stmt1Type, stmt2Type);
}

void DesignExtractor::setAffectsNTBoolean(bool isBooleanClause) {
    AffectsExtractor::setAffectsNTBoolean(isBooleanClause);
}

void DesignExtractor::setAffectsTNBoolean(bool isBooleanClause) {
    AffectsExtractor::setAffectsTNBoolean(isBooleanClause);
}

void DesignExtractor::setAffectsTTBoolean(bool isBooleanClause) {
    AffectsExtractor::setAffectsTTBoolean(isBooleanClause);
}

void DesignExtractor::setAffectsStarNTBoolean(bool isBooleanClause) {
    AffectsExtractor::setAffectsStarNTBoolean(isBooleanClause);
}

void DesignExtractor::setAffectsStarTNBoolean(bool isBooleanClause) {
    AffectsExtractor::setAffectsStarTNBoolean(isBooleanClause);
}

void DesignExtractor::setAffectsStarTTBoolean(bool isBooleanClause) {
    AffectsExtractor::setAffectsStarTTBoolean(isBooleanClause);
}

vector<pair<int, int>> DesignExtractor::getAffectsBipNN(int stmt1, int stmt2) {
    return AffectsBipExtractor::getAffectsBipNN(stmt1, stmt2);
}

vector<pair<int, int>> DesignExtractor::getAffectsBipNT(int stmt1, StatementTypes stmt2Type) {
    return AffectsBipExtractor::getAffectsBipNT(stmt1, stmt2Type);
}

vector<pair<int, int>> DesignExtractor::getAffectsBipTN(StatementTypes stmt1Type, int stmt2) {
    return AffectsBipExtractor::getAffectsBipTN(stmt1Type, stmt2);
}

vector<pair<int, int>> DesignExtractor::getAffectsBipTT(StatementTypes stmt1Type, StatementTypes stmt2Type) {
    return AffectsBipExtractor::getAffectsBipTT(stmt1Type, stmt2Type);
}

void DesignExtractor::setNextBipStarNTBoolean(bool isBooleanClause) {
    return NextBipStarExtractor::setNextBipStarNTBoolean(isBooleanClause);
}

void DesignExtractor::setNextBipStarTNBoolean(bool isBooleanClause) {
    return NextBipStarExtractor::setNextBipStarTNBoolean(isBooleanClause);
}

void DesignExtractor::setNextBipStarTTBoolean(bool isBooleanClause) {
    return NextBipStarExtractor::setNextBipStarTTBoolean(isBooleanClause);
}

vector<std::pair<int, int>> DesignExtractor::getNextBipStarNN(int stmt1, int stmt2) {
    return NextBipStarExtractor::getNextBipStarNN(stmt1, stmt2);
}

vector<std::pair<int, int>> DesignExtractor::getNextBipStarNT(int stmt1, StatementTypes stmt2Type) {
    return NextBipStarExtractor::getNextBipStarNT(stmt1, stmt2Type);
}

vector<std::pair<int, int>> DesignExtractor::getNextBipStarTN(StatementTypes stmt1Type, int stmt2) {
    return NextBipStarExtractor::getNextBipStarTN(stmt1Type, stmt2);
}
vector<std::pair<int, int>> DesignExtractor::getNextBipStarTT(StatementTypes stmt1Type,
    StatementTypes stmt2Type) {
    return NextBipStarExtractor::getNextBipStarTT(stmt1Type, stmt2Type);
}