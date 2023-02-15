//
// Created by Toh Yu Ting on 13 Oct 2021
//

#include "DE/BinaryTree.h"
#include "DE/BinaryTreeNode.h"
#include "catch.hpp"

// Consider adding a test stub?


TEST_CASE("Check TreeNode") {
    SECTION("Change Parent") {
        BinaryTreeNode* testNode = new BinaryTreeNode("testNode");
        BinaryTreeNode* replacementNode = new BinaryTreeNode("changing node");
        testNode->addOrChangeParent(replacementNode);

        REQUIRE(testNode->getParent() == replacementNode);
    }

    SECTION("Change Left Element") {
        BinaryTreeNode* testNode = new BinaryTreeNode("testNode");
        BinaryTreeNode* replacementNode = new BinaryTreeNode("changing node");
        testNode->addOrChangeLeftElement(replacementNode);

        REQUIRE(testNode->getLeftElement() == replacementNode);
    }

    SECTION("Change Right Element") {
        BinaryTreeNode* testNode = new BinaryTreeNode("testNode");
        BinaryTreeNode* replacementNode = new BinaryTreeNode("changing node");
        testNode->addOrChangeRightElement(replacementNode);

        REQUIRE(testNode->getRightElement() == replacementNode);
    }
}

TEST_CASE("Check Binary Tree") {
    SECTION("Add Root") {
        BinaryTree* testTree = new BinaryTree();       
        BinaryTreeNode* testNode = new BinaryTreeNode("testNode");
        testTree->addRoot(testNode);

        REQUIRE(testTree->getRoot() == testNode);
    }

    SECTION("Change Root") {
        BinaryTree* testTree = new BinaryTree();
        BinaryTreeNode* rootNode = new BinaryTreeNode("I am root");
        testTree->addRoot(rootNode);

        REQUIRE(testTree->getRoot() == rootNode);

        BinaryTreeNode* changeNode = new BinaryTreeNode("changing root");
        testTree->changeRoot(changeNode);

        REQUIRE(testTree->getRoot() == changeNode);
        REQUIRE(testTree->getLeftSubtree() == rootNode);
        REQUIRE(rootNode->getParent() == changeNode);
        REQUIRE(changeNode->getLeftElement() == rootNode);

    }

    SECTION("Add Left Subtree") {
        SECTION("Right subtree is empty") {
            BinaryTree* testTree = new BinaryTree();
            BinaryTreeNode* rootNode = new BinaryTreeNode("I am root");
            testTree->addRoot(rootNode);

            BinaryTreeNode* leftTreeNode = new BinaryTreeNode("new left");
            testTree->addLeftSubtree(leftTreeNode);

            REQUIRE(testTree->getRoot() == rootNode);
            REQUIRE(testTree->getLeftSubtree() == leftTreeNode);
            REQUIRE(testTree->getRightSubtree() == NULL);
            REQUIRE(rootNode->getParent() == NULL);
            REQUIRE(rootNode->getLeftElement() == leftTreeNode);
            REQUIRE(leftTreeNode->getParent() == rootNode);
        }

        SECTION("Right subtree is not empty") {
            BinaryTree* testTree = new BinaryTree();
            BinaryTreeNode* rootNode = new BinaryTreeNode("I am root");
            testTree->addRoot(rootNode);
            testTree->addRightSubtree(new BinaryTreeNode("I am new right"));
            BinaryTreeNode* sameRight = testTree->getRightSubtree();

            BinaryTreeNode* leftTreeNode = new BinaryTreeNode("new left");
            testTree->addLeftSubtree(leftTreeNode);

            REQUIRE(testTree->getRoot() == rootNode);
            REQUIRE(testTree->getLeftSubtree() == leftTreeNode);
            REQUIRE(testTree->getRightSubtree() == sameRight);
            REQUIRE(rootNode->getParent() == NULL);
            REQUIRE(rootNode->getLeftElement() == leftTreeNode);
            REQUIRE(rootNode->getRightElement() == sameRight);
            REQUIRE(leftTreeNode->getParent() == rootNode);
        }

    }

    SECTION("Change Left Subtree") {
        SECTION("Right subtree is empty") {
            BinaryTree* testTree = new BinaryTree();
            BinaryTreeNode* rootNode = new BinaryTreeNode("I am root");
            testTree->addRoot(rootNode);
            testTree->addLeftSubtree(new BinaryTreeNode("first left"));

            BinaryTreeNode* leftTreeNode = new BinaryTreeNode("new left");
            BinaryTreeNode* oldLeft = testTree->getLeftSubtree();
            testTree->replaceLeftSubtree(leftTreeNode);

            REQUIRE(testTree->getRoot() == rootNode);
            REQUIRE(testTree->getLeftSubtree() == leftTreeNode);
            REQUIRE(testTree->getRightSubtree() == NULL);
            REQUIRE(rootNode->getParent() == NULL);
            REQUIRE(rootNode->getLeftElement() == leftTreeNode);
            REQUIRE(leftTreeNode->getParent() == rootNode);
            REQUIRE(leftTreeNode->getLeftElement() == oldLeft);
            REQUIRE(oldLeft->getParent() == leftTreeNode);
        }

        SECTION("Right subtree is not empty") {
            BinaryTree* testTree = new BinaryTree();
            BinaryTreeNode* rootNode = new BinaryTreeNode("I am root");
            testTree->addRoot(rootNode);
            testTree->addLeftSubtree(new BinaryTreeNode("first left"));
            testTree->addRightSubtree(new BinaryTreeNode("first right"));
            BinaryTreeNode* sameRight = testTree->getRightSubtree();

            BinaryTreeNode* leftTreeNode = new BinaryTreeNode("new left");
            BinaryTreeNode* oldLeft = testTree->getLeftSubtree();
            testTree->replaceLeftSubtree(leftTreeNode);

            REQUIRE(testTree->getRoot() == rootNode);
            REQUIRE(testTree->getLeftSubtree() == leftTreeNode);
            REQUIRE(testTree->getRightSubtree() == sameRight);
            REQUIRE(sameRight->getParent() == rootNode);
            REQUIRE(rootNode->getParent() == NULL);
            REQUIRE(rootNode->getLeftElement() == leftTreeNode);
            REQUIRE(rootNode->getRightElement() == sameRight);
            REQUIRE(leftTreeNode->getParent() == rootNode);
            REQUIRE(leftTreeNode->getLeftElement() == oldLeft);
            REQUIRE(oldLeft->getParent() == leftTreeNode);
        }
    }

    SECTION("Add Right Subtree") {
        SECTION("Left subtree is empty") {
            BinaryTree* testTree = new BinaryTree();
            BinaryTreeNode* rootNode = new BinaryTreeNode("I am root");
            testTree->addRoot(rootNode);

            BinaryTreeNode* rightTreeNode = new BinaryTreeNode("new right");
            testTree->addRightSubtree(rightTreeNode);

            REQUIRE(testTree->getRoot() == rootNode);
            REQUIRE(testTree->getLeftSubtree() == NULL);
            REQUIRE(testTree->getRightSubtree() == rightTreeNode);
            REQUIRE(rootNode->getParent() == NULL);
            REQUIRE(rootNode->getLeftElement() == NULL);
            REQUIRE(rootNode->getRightElement() == rightTreeNode);
            REQUIRE(rightTreeNode->getParent() == rootNode);
        }

        SECTION("Left subtree is not empty") {
            BinaryTree* testTree = new BinaryTree();
            BinaryTreeNode* rootNode = new BinaryTreeNode("I am root");
            testTree->addRoot(rootNode);
            testTree->addRightSubtree(new BinaryTreeNode("I am new right"));
            BinaryTreeNode* sameRight = testTree->getRightSubtree();

            BinaryTreeNode* leftTreeNode = new BinaryTreeNode("new left");
            testTree->addLeftSubtree(leftTreeNode);

            REQUIRE(testTree->getRoot() == rootNode);
            REQUIRE(testTree->getLeftSubtree() == leftTreeNode);
            REQUIRE(testTree->getRightSubtree() == sameRight);
            REQUIRE(rootNode->getParent() == NULL);
            REQUIRE(rootNode->getLeftElement() == leftTreeNode);
            REQUIRE(rootNode->getRightElement() == sameRight);
            REQUIRE(leftTreeNode->getParent() == rootNode);
        }

    }

    SECTION("Change Right Subtree") {
        SECTION("Left subtree is empty") {
            BinaryTree* testTree = new BinaryTree();
            BinaryTreeNode* rootNode = new BinaryTreeNode("I am root");
            testTree->addRoot(rootNode);
            testTree->addRightSubtree(new BinaryTreeNode("first right"));

            BinaryTreeNode* rightTreeNode = new BinaryTreeNode("new right");
            BinaryTreeNode* oldRight = testTree->getRightSubtree();
            testTree->replaceRightSubtree(rightTreeNode);

            REQUIRE(testTree->getRoot() == rootNode);
            REQUIRE(testTree->getLeftSubtree() == NULL);
            REQUIRE(testTree->getRightSubtree() == rightTreeNode);
            REQUIRE(rootNode->getParent() == NULL);
            REQUIRE(rootNode->getRightElement() == rightTreeNode);
            REQUIRE(rightTreeNode->getParent() == rootNode);
            REQUIRE(rightTreeNode->getLeftElement() == oldRight);
            REQUIRE(oldRight->getParent() == rightTreeNode);
        }

        SECTION("Left subtree is not empty") {
            BinaryTree* testTree = new BinaryTree();
            BinaryTreeNode* rootNode = new BinaryTreeNode("I am root");
            testTree->addRoot(rootNode);
            testTree->addLeftSubtree(new BinaryTreeNode("first left"));
            testTree->addRightSubtree(new BinaryTreeNode("first right"));
            BinaryTreeNode* sameLeft = testTree->getLeftSubtree();

            BinaryTreeNode* rightTreeNode = new BinaryTreeNode("new right");
            BinaryTreeNode* oldRight = testTree->getRightSubtree();
            testTree->replaceRightSubtree(rightTreeNode);

            REQUIRE(testTree->getRoot() == rootNode);
            REQUIRE(testTree->getLeftSubtree() == sameLeft);
            REQUIRE(testTree->getRightSubtree() == rightTreeNode);
            REQUIRE(sameLeft->getParent() == rootNode);
            REQUIRE(rootNode->getParent() == NULL);
            REQUIRE(rootNode->getLeftElement() == sameLeft);
            REQUIRE(rootNode->getRightElement() == rightTreeNode);
            REQUIRE(rightTreeNode->getParent() == rootNode);
            REQUIRE(rightTreeNode->getLeftElement() == oldRight);
            REQUIRE(oldRight->getParent() == rightTreeNode);
        }


    }
}

