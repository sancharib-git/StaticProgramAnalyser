#pragma once
#include <string>

class BinaryTreeNode {
private:
    std::string element;
    BinaryTreeNode* parent;
    BinaryTreeNode* leftElement;
    BinaryTreeNode* rightElement;

public:
    BinaryTreeNode(std::string nodeElement);
    bool addOrChangeParent(BinaryTreeNode* parentNode);
    bool addOrChangeLeftElement(BinaryTreeNode* leftNode);
    bool addOrChangeRightElement(BinaryTreeNode* rightNode);
    BinaryTreeNode* getParent();
    BinaryTreeNode* getLeftElement();
    BinaryTreeNode* getRightElement();
    std::string getElement();
    bool setParent(BinaryTreeNode* parentNode);
    bool setLeftElement(BinaryTreeNode* leftNode);
    bool setRightElement(BinaryTreeNode* rightNode);
};