#pragma once
#include <string>
#include <stdio.h>
#include <iostream>
#include "BinaryTree.h"
#include "BinaryTreeNode.h"

using namespace std;

BinaryTree::BinaryTree() {
    root = NULL;
    leftSubtree = NULL;
    rightSubtree = NULL;
}
BinaryTree::BinaryTree(BinaryTreeNode* rootElement) {
    root = rootElement;
    leftSubtree = rootElement->getLeftElement();
    rightSubtree = rootElement->getRightElement();
}

bool BinaryTree::addRoot(BinaryTreeNode* rootElement) {
    // current binary tree has empty root
    root = rootElement;
    leftSubtree = root->getLeftElement();
    rightSubtree = root->getRightElement();
    return true;
}

bool BinaryTree::changeRoot(BinaryTreeNode* rootElement) {
    // if binary tree has a root, current root should be left subtree, and left element of root
    // assuming that new root will not have a left or right element
    BinaryTreeNode* currentRoot = root;
    currentRoot->addOrChangeParent(rootElement);
    rootElement->setLeftElement(currentRoot);
    leftSubtree = currentRoot;
    rightSubtree = rootElement->getRightElement();
    root = rootElement;
    return true;
}

bool BinaryTree::addLeftSubtree(BinaryTreeNode* newLeft) {
    // current tree does not have a left subtree. Simply add the leftsubtree
    leftSubtree = newLeft;
    // parent of newLeft will be root of current tree
    newLeft->addOrChangeParent(root);
    // left element of root node will be newLeft
    root->addOrChangeLeftElement(newLeft);
    return true;
}

bool BinaryTree::replaceLeftSubtree(BinaryTreeNode* newLeft) {
    // when replacing, change the current left to the left element
    // every change in left sub tree will put current left subtree to the left
    BinaryTreeNode* currentLeftSubtree = leftSubtree;

    // Updating nodes
    // 1) Update parent of current subtree to point to new left
    currentLeftSubtree->addOrChangeParent(newLeft);
    // 2) Update parent of newLeft to point to root. Root's left element to newLeft.
    // newLeft's left element to point to root of currentLeftSubtree.
    newLeft->addOrChangeParent(root);
    root->setLeftElement(newLeft);
    newLeft->setLeftElement(currentLeftSubtree);

    // Updating left subtree
    leftSubtree = newLeft;
    return true;
}

bool BinaryTree::addRightSubtree(BinaryTreeNode* newRight) {
    // current tree does not have a right subtree. Simply add the right subtree
    rightSubtree = newRight;
    // parent of newRight will be root of current tree
     newRight->addOrChangeParent(root);
    // right element of root node will be newRight
     root->addOrChangeRightElement(newRight);
    return true;
}

bool BinaryTree::replaceRightSubtree(BinaryTreeNode* newRight) {
    // every change in right subtree will put current right subtree to LHS of the tree
    BinaryTreeNode* currentRightSubtree = rightSubtree;
    // Nodes updates
    // 1) Update parent of right subtree to point to new right
    currentRightSubtree->addOrChangeParent(newRight);
    // 2) Update parent of new right to point to root.
    // Root right element to newRight, newRight's left to be current rightSubtree's root
    newRight->addOrChangeParent(root);
    root->setRightElement(newRight);
    newRight->setLeftElement(currentRightSubtree);
    // updating right subtree
    rightSubtree = newRight;
    return true;
}

BinaryTreeNode* BinaryTree::getRoot() {
    return root;
}

BinaryTreeNode* BinaryTree::getRightSubtree() {
    return rightSubtree;
}

BinaryTreeNode* BinaryTree::getLeftSubtree() {
    return leftSubtree;
}





