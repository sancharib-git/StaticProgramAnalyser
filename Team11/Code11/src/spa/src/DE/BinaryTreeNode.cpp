#include "BinaryTreeNode.h"
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

BinaryTreeNode::BinaryTreeNode(string nodeElement) {
    element = nodeElement;
    parent = NULL;
    leftElement = NULL;
    rightElement = NULL;
}

string BinaryTreeNode::getElement() {
    return element;
}

bool BinaryTreeNode::addOrChangeParent(BinaryTreeNode* parentNode) {
    parent = parentNode;
    return true;
}

bool BinaryTreeNode::addOrChangeLeftElement(BinaryTreeNode* leftNode) {
    leftElement = leftNode;
    return true;
}

bool BinaryTreeNode::addOrChangeRightElement(BinaryTreeNode* rightNode) {
    rightElement = rightNode;
    return true;
}

BinaryTreeNode* BinaryTreeNode::getParent() {
    return parent;
}
BinaryTreeNode* BinaryTreeNode::getLeftElement() {
    return leftElement;
}

BinaryTreeNode* BinaryTreeNode::getRightElement() {
    return rightElement;
}

bool BinaryTreeNode::setParent(BinaryTreeNode* parentNode) {
    parent = parentNode;
    return true;
}
bool BinaryTreeNode::setLeftElement(BinaryTreeNode* leftNode) {
    leftElement = leftNode;
    return true;
}
bool BinaryTreeNode::setRightElement(BinaryTreeNode* rightNode) {
    rightElement = rightNode;
    return true;
}