//
// Created by Xuan Zhi on 11/10/21.
//

#ifndef AUTOTESTER_TREEPROCESSOR_H
#define AUTOTESTER_TREEPROCESSOR_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "DE/DesignExtractor.h"
#include "DE/Graph.h"
#include "StatementTypes.h"
#include "DE/BinaryTree.h"

class TreeProcessor {
private:
    bool processSubExprTree(BinaryTree& currentTree, BinaryTree& subExprTree);
    bool processAfterFirstBracketOperator(BinaryTree& currentTree, char assignmentOp);
    bool processVarConst(BinaryTree& currentTree, std::string varOrConst);
    std::pair<std::string, int> processBrackets(std::string expressionInsideBrackets);
    bool processPlusMinus(BinaryTree& currentTree, char assignmentOp);
    bool processMulDivMod(BinaryTree& currentTree, char assignmentOp);
public:
    TreeProcessor() {};
    BinaryTree* buildASTFromSubexpr(std::string subexpr);
    std::string treeInString(BinaryTree& tree);
    std::string extractAndPopulateAssignmentSubexpr(BinaryTree& tree, int stmtNum);
};


#endif //AUTOTESTER_TREEPROCESSOR_H
