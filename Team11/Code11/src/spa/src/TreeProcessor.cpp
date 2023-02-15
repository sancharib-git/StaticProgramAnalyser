//
// Created by Xuan Zhi on 11/10/21.
//

#include "TreeProcessor.h"
#include "TreeProcessorToPKB.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include "DE/BinaryTree.h"
#include "DE/BinaryTreeNode.h"
#include "PKB/PKB.h"

using namespace std;

BinaryTree* TreeProcessor::buildASTFromSubexpr(std::string subexpr) {
    BinaryTree* assignmentAST = new BinaryTree();
    // Loops through the entire string per character. And add to the binary tree, which acts as the abstract syntax tree
    std::string currentVariable = "";
    bool firstBracket = false;
    for (int i = 0; i < subexpr.size(); i) {
        char currentToken = subexpr[i];
        if (currentToken == '+' || currentToken == '-') {
            if (firstBracket) {
                // turn firstBracket off
                firstBracket = false;
            }
            // replace current root
            processPlusMinus(*assignmentAST, currentToken);
            i++;
            continue;
        }
        else if (currentToken == '*' || currentToken == '/' || currentToken == '%') {
            if (firstBracket) {
                processAfterFirstBracketOperator(*assignmentAST, currentToken);
                firstBracket = false;
                i++;
                continue;
            }
            processMulDivMod(*assignmentAST, currentToken);
            i++;
            continue;
        }
        else if (currentToken == '(') {
            // extract out the sub expr, and recurse call this
            std::string currentSubstring = subexpr.substr(i);
            std::pair<std::string, int> subStringAndIndex = processBrackets(currentSubstring);
            std::string stringInsideBrackets = subStringAndIndex.first;
            // Recursively call this function to process sub expressions
            BinaryTree* subExprTree = buildASTFromSubexpr(stringInsideBrackets);
            processSubExprTree(*assignmentAST, *subExprTree);
            // need to update i
            if (i == 0) {
                firstBracket = true;
            }
            i = subStringAndIndex.second + i + 1;
            continue;
        }
        // process the variable/const char -> extract out the entire var/const
        std::string currentSubstring = subexpr.substr(i);
        size_t nextIndex = currentSubstring.find_first_of("+-*/%()");
        currentSubstring = subexpr.substr(i, nextIndex);
        processVarConst(*assignmentAST, currentSubstring);
        // updating i
        if (nextIndex == string::npos) {
            //            i++;
            break;
        }
        else {
            i = nextIndex + i;
        }
    }

    return assignmentAST;
}

bool TreeProcessor::processAfterFirstBracketOperator(BinaryTree& currentTree, char assignmentOp) {
    BinaryTreeNode* newNode = new BinaryTreeNode(std::string(1, assignmentOp));
    BinaryTreeNode* currentRoot = currentTree.getRoot();
    currentTree.changeRoot(newNode);
    return true;
}

bool TreeProcessor::processSubExprTree(BinaryTree& currentTree, BinaryTree& subExprTree) {
    BinaryTreeNode* rootOfSubExpr = subExprTree.getRoot();

    if (currentTree.getRoot() == NULL) {
        // subexpr at the start of the assignment statement
        currentTree.addRoot(rootOfSubExpr);
        return true;
    }

    BinaryTreeNode* currentTreeRightSubtree = currentTree.getRightSubtree();
    if (currentTreeRightSubtree == NULL) {
        currentTree.addRightSubtree(rootOfSubExpr);
    }
    else {
        currentTreeRightSubtree->addOrChangeRightElement(rootOfSubExpr);
    }
    return true;
}

bool TreeProcessor::processVarConst(BinaryTree& currentTree, std::string varOrConst) {

    BinaryTreeNode* newVarOrConst = new BinaryTreeNode(varOrConst);
    if (currentTree.getRoot() == NULL) {
        // first var of the tree
        currentTree.addRoot(newVarOrConst);
        return true;
    }
    // either root->rightSubtree is empty, or root->rightSubtree->rightSubtree is empty
    BinaryTreeNode* currentTreeRightSubtree = currentTree.getRightSubtree();
    if (currentTreeRightSubtree == NULL) {
        currentTree.addRightSubtree(newVarOrConst);
    }
    else {
        // root->rightSubtree->rightSubtree is empty. This change should update BinaryTree too, given that we are using pointer
        currentTreeRightSubtree->addOrChangeRightElement(newVarOrConst);
    }
    return true;
}

std::pair<std::string, int> TreeProcessor::processBrackets(std::string expressionInsideBrackets) {
    // all expression into this function has first char as (
    int numberOfFrontBracket = 0;
    int backBracketIndex = 0;
    for (int i = 0; i < expressionInsideBrackets.size(); i++) {
        char token = expressionInsideBrackets[i];
        if (token == '(') {
            numberOfFrontBracket++;
        }
        else if (token == ')') {
            if (numberOfFrontBracket == 1) {
                // this is the corresponding closing bracket
                backBracketIndex = i;
                break;
            }
            numberOfFrontBracket--;
        }
    }
    std::string subString = expressionInsideBrackets.substr(1, backBracketIndex - 1);
    return make_pair(subString, backBracketIndex);
}

bool TreeProcessor::processPlusMinus(BinaryTree& currentTree, char assignmentOp) {
    BinaryTreeNode* newNode = new BinaryTreeNode(std::string(1, assignmentOp));
    currentTree.changeRoot(newNode);
    return true;
}

bool TreeProcessor::processMulDivMod(BinaryTree& currentTree, char assignmentOp) {
    BinaryTreeNode* newNode = new BinaryTreeNode(std::string(1, assignmentOp));
    BinaryTreeNode* currentRoot = currentTree.getRoot();
    if (currentRoot->getElement() == "+" || currentRoot->getElement() == "-") {
        currentTree.replaceRightSubtree(newNode);
    }
    else {
        currentTree.changeRoot(newNode);
    }
    return true;
}


std::string processSubstring(BinaryTreeNode* nodeToCheck, BinaryTreeNode* currentNode,
    std::pair<std::string, std::string> currentStringAndNewSubString) {
    std::string currentSubstring = currentStringAndNewSubString.first;
    std::string newSubstring = currentStringAndNewSubString.second;
    currentSubstring = currentSubstring + "(" + newSubstring + ")";

    return currentSubstring;
}

std::string TreeProcessor::treeInString(BinaryTree& assignmentTree) {
    std::string currentSubstring = "";
    if (assignmentTree.getLeftSubtree() == NULL && assignmentTree.getRightSubtree() == NULL) {
        currentSubstring = assignmentTree.getRoot()->getElement();
        return currentSubstring;
    }

    // AST will always be a complete binary tree, either no leaves nodes or 2 leaves nodes
    // 1) Recurse left subtree, then recurse right subtree. Then add the sub expression and return
    BinaryTreeNode* currentRoot = assignmentTree.getRoot();
    BinaryTreeNode* leftSubtreeRoot = assignmentTree.getLeftSubtree();
    BinaryTree* leftSubtree = new BinaryTree(leftSubtreeRoot);
    std::string leftSubstring = treeInString(*leftSubtree);
    currentSubstring = processSubstring(leftSubtreeRoot, currentRoot, make_pair(currentSubstring, leftSubstring));
    currentSubstring += currentRoot->getElement();

    BinaryTreeNode* rightSubtreeRoot = assignmentTree.getRightSubtree();
    BinaryTree* rightSubtree = new BinaryTree(rightSubtreeRoot);

    std::string rightSubstring = treeInString(*rightSubtree);
    currentSubstring = processSubstring(rightSubtreeRoot, currentRoot,
        make_pair(currentSubstring, rightSubstring));

    return currentSubstring;
}

std::string TreeProcessor::extractAndPopulateAssignmentSubexpr(BinaryTree& assignmentTree, int stmtNum) {
    std::string currentSubstring = "";
    if (assignmentTree.getLeftSubtree() == NULL && assignmentTree.getRightSubtree() == NULL) {
        currentSubstring = assignmentTree.getRoot()->getElement();
        TreeProcessorToPKB::addAssignmentToPKB(stmtNum, currentSubstring);
        return currentSubstring;
    }
    // AST will always be a complete binary tree, either no leaves nodes or 2 leaves nodes
    // 1) Recurse left subtree, then recurse right subtree. Then add the sub expression and return
    BinaryTreeNode* currentRoot = assignmentTree.getRoot();
    BinaryTreeNode* leftSubtreeRoot = assignmentTree.getLeftSubtree();
    BinaryTree* leftSubtree = new BinaryTree(leftSubtreeRoot);
    string leftSubstring = extractAndPopulateAssignmentSubexpr(*leftSubtree, stmtNum);
    currentSubstring = processSubstring(leftSubtreeRoot, currentRoot, make_pair(currentSubstring, leftSubstring));
    currentSubstring += currentRoot->getElement();

    BinaryTreeNode* rightSubtreeRoot = assignmentTree.getRightSubtree();
    BinaryTree* rightSubtree = new BinaryTree(rightSubtreeRoot);
    string rightSubstring = extractAndPopulateAssignmentSubexpr(*rightSubtree, stmtNum);

    currentSubstring = processSubstring(rightSubtreeRoot, currentRoot,
        make_pair(currentSubstring, rightSubstring));
    TreeProcessorToPKB::addAssignmentToPKB(stmtNum, currentSubstring);
    return currentSubstring;
}