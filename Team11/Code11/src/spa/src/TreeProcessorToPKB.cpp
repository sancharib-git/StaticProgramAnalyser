#include "TreeProcessorToPKB.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include "DE/BinaryTree.h"
#include "PKB/PKB.h"

using namespace std;

bool TreeProcessorToPKB::addAssignmentToPKB(int stmtNum, string assignmentSubExpr) {
    PKB& sharedPkb = sharedPkb.getInstance();
    sharedPkb.addAssignSubExp(stmtNum, assignmentSubExpr);
    return true;
}