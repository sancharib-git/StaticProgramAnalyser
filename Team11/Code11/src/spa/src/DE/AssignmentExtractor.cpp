#include<stdio.h>
#include <iostream>
#include <string>
#include "AssignmentExtractor.h"
#include <unordered_set>
#include <utility>
#include "PKB/PKB.h"
#include "StatementTypes.h"
#include "BinaryTree.h"
#include "TreeProcessor.h"

using namespace std;

bool AssignmentExtractor::populateAssignmentSubExpressions() {
    PKB& sharedPkb = sharedPkb.getInstance();
    // 1) Retrieves all assignment statement numbers from PKB
    unordered_set<int>* assignmentStmtNums = sharedPkb.getAssignList();
    // 2) Iterate through all assignment statement, creating AST for each assignment statement, store every sub expression in PKB
    TreeProcessor* treeProcessor = new TreeProcessor();
    for (const int& assignmentStmtNum : *assignmentStmtNums) {
        string assignmentExpr = sharedPkb.getAssignPattern(assignmentStmtNum);
        BinaryTree* assignmentAST = treeProcessor->buildASTFromSubexpr(assignmentExpr);
        // 3) Find all subtrees of assignmentAST
        treeProcessor->extractAndPopulateAssignmentSubexpr(*assignmentAST, assignmentStmtNum);
    }
    return true;
}