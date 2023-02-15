#include <vector>
#include "NextExtractor.h"
#include <unordered_set>
#include "PKB/PKB.h"
#include "StatementTypes.h"
#include <algorithm>

using namespace std;


bool NextExtractor::extractNext() {
    PKB& sharedPkb = sharedPkb.getInstance();
    unordered_set<string>* allProcedures = sharedPkb.getProcList();
    for (const string& procedure : *allProcedures) {
        vector<int> procStmtNums = sharedPkb.getProcedureStmtNums(procedure);
        for (int stmt : procStmtNums) {
            /* Case 1: Is a ifCondStmt */
            if (sharedPkb.getStatementType(stmt) == StatementTypes::IF) {
                processNextIfProgLine(stmt);
                /* Case 2: Is a whileCondStmt */
            }
            else if (sharedPkb.getStatementType(stmt) == StatementTypes::WHILE) {
                processNextWhileProgLine(stmt);
                /* Case 3: Is a nested IfBlockStmt / elseBlockStmt */
            }
            else if (!(sharedPkb.getParentTN(StatementTypes::IF, stmt).empty())) {
                processNextIfContainerStatements(stmt);
                /* Case 4: Is a nested whileBlockStmt */
            }
            else if (!(sharedPkb.getParentTN(StatementTypes::WHILE, stmt).empty())) {
                processNextWhileContainerStatements(stmt);
                /* Case 5: Is not nested and not ifCondStmt/whileCondStmt */
            }
            else {
                processNextOuterProgLines(stmt);
            }
        }
    }
    return true;
}

bool NextExtractor::processNextIfProgLine(int progLine) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int ifFirstBlockStmt = sharedPkb.getIfBlockList(progLine).front();
    /* Add Next(ifCondStmt, ifFirstBlockStmt) */
    sharedPkb.addNext(progLine, ifFirstBlockStmt);

    int elseFirstBlockStmt = sharedPkb.getElseBlockList(progLine).front();
    /* Add Next(ifCondStmt, elseFirstBlockStmt) */
    sharedPkb.addNext(progLine, elseFirstBlockStmt);
    return true;
}

bool NextExtractor::processNextWhileProgLine(int progLine) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int whileFirstBlockStmt = sharedPkb.getWhileBlockList(progLine).front();
    /* Add Next(whileCondStmt, whileFirstBlockStmt) */
    sharedPkb.addNext(progLine, whileFirstBlockStmt);

    /* Check for existence of whileCondStmt's follower before adding */
    if (!(sharedPkb.getFollowsNT(progLine, StatementTypes::NO_TYPE).empty())) {
        /* Add Next(whileCondStmt, follower) */
        sharedPkb.addNext(progLine, sharedPkb.getFollowsNT(progLine, StatementTypes::NO_TYPE).front().second);
    }
    else {
        // follower empty, check if this is a nested while
        if (!(sharedPkb.getParentTN(StatementTypes::WHILE, progLine).empty())) {
            // nested while progLine
            int parent = sharedPkb.getParentTN(StatementTypes::WHILE, progLine).front().first;
            sharedPkb.addNext(progLine, parent);
            /*if ((progLine - parent) == 1) {
                sharedPkb.addNext(progLine, parent);
            }*/
        }
        else if (!(sharedPkb.getParentTN(StatementTypes::IF, progLine).empty())) {
            // nested inside an if parent, either in if stmtList or else stmtList
            //TODO: CHANGE NAMING OF THE FUNCTION CALLED
            processNextIfLastContainerStatementNestedIf(progLine, progLine);
        }
        else {
            // last statement in a procedure
            return false;
        }
    }
    return true;
}

bool NextExtractor::processNextIfContainerStatements(int progLine) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int parent = sharedPkb.getParentTN(StatementTypes::IF, progLine).front().first;
    int ifLastBlockStmt = sharedPkb.getIfBlockList(parent).back();
    int elseLastBlockStmt = sharedPkb.getElseBlockList(parent).back();

    /* If is ifLastBlockStmt or elseLastBlockStmt */
    if (progLine == ifLastBlockStmt || progLine == elseLastBlockStmt) {
        processNextIfLastContainerStatement(progLine, parent);
    }
    else {
        /* Check for existence of nested stmt's follower before adding */
        if (!(sharedPkb.getFollowsNT(progLine, StatementTypes::NO_TYPE).empty())) {
            /* Add Next(stmt, follower) */
            sharedPkb.addNext(progLine, sharedPkb.getFollowsNT(progLine, StatementTypes::NO_TYPE).front().second);
        }

    }
    return true;
}

bool NextExtractor::processNextIfLastContainerStatement(int progLine, int parent) {
    PKB& sharedPkb = sharedPkb.getInstance();
    /* Check for existence of parent's follower before adding */
    if (!(sharedPkb.getFollowsNT(parent, StatementTypes::NO_TYPE).empty())) {
        /* Add Next(ifLastBlockStmt, follower) or Next(elseLastBlockStmt, follower) */
        int parentFollower = sharedPkb.getFollowsNT(parent, StatementTypes::NO_TYPE).front().second;
        sharedPkb.addNext(progLine, parentFollower);
    }
    else {
        // if no follower, check that it is not nested inside a while
        if (!(sharedPkb.getParentTN(StatementTypes::WHILE, parent).empty())) {
            // nested while progLine
            int parentWhile = sharedPkb.getParentTN(StatementTypes::WHILE, parent).front().first;
            sharedPkb.addNext(progLine, parentWhile);
        }
        else if (!(sharedPkb.getParentTN(StatementTypes::IF, parent).empty())) {
            // nested inside an if parent, either in if stmtList or else stmtList
            processNextIfLastContainerStatementNestedIf(progLine, parent);
        }
    }
    return true;
}

bool NextExtractor::processNextIfLastContainerStatementNestedIf(int progLine, int parent) {
    PKB& sharedPkb = sharedPkb.getInstance();
    // get parent statement from PKB, tested in caller that parent is nested in IF
    int nestedParentStatement = sharedPkb.getParentTN(StatementTypes::IF, parent).front().first;
    bool isParent = 1;
    while (isParent) {
        // check type of parent first
        StatementTypes parentType = sharedPkb.getStatementType(nestedParentStatement);
        if (parentType == WHILE) {
            sharedPkb.addNext(progLine, nestedParentStatement);
            isParent = 0;
            break;
        }
        else if (parentType == IF) {
            // check if parent has a follower
            isParent = processNextDeeplyNestedIfLastStatement(progLine, nestedParentStatement);
            if (isParent) {
                nestedParentStatement = sharedPkb.getParentTN(NO_TYPE, nestedParentStatement).front().first;
            }
        }
        else if (sharedPkb.getFollowsNT(nestedParentStatement, StatementTypes::NO_TYPE).empty()) {
            // check this else if condition if it is needed again.
            // parent in most outer loop without a follows, end of a procedure
            isParent = 0;
            break;
        }
    }
    return true;
}

int NextExtractor::processNextDeeplyNestedIfLastStatement(int progLine, int parent) {
    PKB& sharedPkb = sharedPkb.getInstance();
    if (!(sharedPkb.getFollowsNT(parent, NO_TYPE).empty())) {
        sharedPkb.addNext(progLine, sharedPkb.getFollowsNT(parent, NO_TYPE).front().second);
        return 0;
    }

    if (!(sharedPkb.getParentStarTN(NO_TYPE, parent).empty())) {
        return 1;
    }
    return 0;
}

bool NextExtractor::processNextWhileContainerStatements(int progLine) {
    PKB& sharedPkb = sharedPkb.getInstance();
    int parent = sharedPkb.getParentTN(StatementTypes::WHILE, progLine).front().first;
    int whileLastBlockStmt = sharedPkb.getWhileBlockList(parent).back();

    /* If is whileLastBlockStmt */
    if (progLine == whileLastBlockStmt) {
        /* Add Next(whileLastBlockStmt, whileCondStmt) */
        sharedPkb.addNext(progLine, parent);
    }
    else {
        /* Check for existence of nested stmt's follower before adding */
        if (!(sharedPkb.getFollowsNT(progLine, StatementTypes::NO_TYPE).empty())) {
            /* Add Next(stmt, follower) */
            sharedPkb.addNext(progLine, sharedPkb.getFollowsNT(progLine, StatementTypes::NO_TYPE).front().second);
        }
    }
    return true;
}

bool NextExtractor::processNextOuterProgLines(int progLine) {
    PKB& sharedPkb = sharedPkb.getInstance();
    /* Check for existence of follower before adding */
    if (!(sharedPkb.getFollowsNT(progLine, NO_TYPE).empty())) {
        /* Add Next(stmt, follower) */
        sharedPkb.addNext(progLine, sharedPkb.getFollowsNT(progLine, NO_TYPE).front().second);
    }
    return true;
}