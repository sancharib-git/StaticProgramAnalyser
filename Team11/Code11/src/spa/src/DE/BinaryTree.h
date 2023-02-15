#pragma once
#include <string>
#include "BinaryTreeNode.h"

class BinaryTree {
private:
    BinaryTreeNode* root;
    BinaryTreeNode* leftSubtree;
    BinaryTreeNode* rightSubtree;

public:
    BinaryTree(BinaryTreeNode* rootElement);
    BinaryTree();
    bool addRoot(BinaryTreeNode* newRoot);
    bool changeRoot(BinaryTreeNode* newRoot);
    bool addLeftSubtree(BinaryTreeNode* newLeft);
    bool replaceLeftSubtree(BinaryTreeNode* newLeft);
    bool addRightSubtree(BinaryTreeNode* newRight);
    bool replaceRightSubtree(BinaryTreeNode* newRight);
    BinaryTreeNode* getRoot();
    BinaryTreeNode* getRightSubtree();
    BinaryTreeNode* getLeftSubtree();
};