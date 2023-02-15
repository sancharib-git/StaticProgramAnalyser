//
// Created by Toh Yu Ting on 27 Oct 2021
//

#include "TreeProcessor.h"
#include "catch.hpp"

TreeProcessor* treeProcessor = new TreeProcessor();
TEST_CASE("Building AST from subexpression") {
    SECTION("one variable") {
        BinaryTree* returnTree = treeProcessor->buildASTFromSubexpr("x");
        REQUIRE(returnTree->getRoot()->getElement() == "x");
        REQUIRE(returnTree->getLeftSubtree() == NULL);
        REQUIRE(returnTree->getRightSubtree() == NULL);
    }
    SECTION("One operator, one character variable") {
        //////////////////////// + operator ////////////////////////
        BinaryTree* returnTreePlus = treeProcessor->buildASTFromSubexpr("x+y");
        REQUIRE(returnTreePlus->getRoot()->getElement() == "+");
        REQUIRE(returnTreePlus->getRoot()->getParent() == NULL);
        REQUIRE(returnTreePlus->getRoot()->getLeftElement()->getElement() == "x");
        REQUIRE(returnTreePlus->getRoot()->getRightElement()->getElement() == "y");

        REQUIRE(returnTreePlus->getLeftSubtree()->getElement() == "x");
        REQUIRE(returnTreePlus->getRightSubtree()->getElement() == "y");
        REQUIRE(returnTreePlus->getLeftSubtree()->getParent() == returnTreePlus->getRoot());
        REQUIRE(returnTreePlus->getRightSubtree()->getParent() == returnTreePlus->getRoot());

        //////////////////////// - operator ////////////////////////
        BinaryTree* returnTreeMinus = treeProcessor->buildASTFromSubexpr("x-y");
        REQUIRE(returnTreeMinus->getRoot()->getElement() == "-");
        REQUIRE(returnTreeMinus->getRoot()->getParent() == NULL);
        REQUIRE(returnTreeMinus->getRoot()->getLeftElement()->getElement() == "x");
        REQUIRE(returnTreeMinus->getRoot()->getRightElement()->getElement() == "y");

        REQUIRE(returnTreeMinus->getLeftSubtree()->getElement() == "x");
        REQUIRE(returnTreeMinus->getRightSubtree()->getElement() == "y");
        REQUIRE(returnTreeMinus->getLeftSubtree()->getParent() == returnTreeMinus->getRoot());
        REQUIRE(returnTreeMinus->getRightSubtree()->getParent() == returnTreeMinus->getRoot());
        //////////////////////// * operator ////////////////////////
        BinaryTree* returnTreeMultiply = treeProcessor->buildASTFromSubexpr("x*y");
        REQUIRE(returnTreeMultiply->getRoot()->getElement() == "*");
        REQUIRE(returnTreeMultiply->getRoot()->getParent() == NULL);
        REQUIRE(returnTreeMultiply->getRoot()->getLeftElement()->getElement() == "x");
        REQUIRE(returnTreeMultiply->getRoot()->getRightElement()->getElement() == "y");

        REQUIRE(returnTreeMultiply->getLeftSubtree()->getElement() == "x");
        REQUIRE(returnTreeMultiply->getRightSubtree()->getElement() == "y");
        REQUIRE(returnTreeMultiply->getLeftSubtree()->getParent()
            == returnTreeMultiply->getRoot());
        REQUIRE(returnTreeMultiply->getRightSubtree()->getParent()
            == returnTreeMultiply->getRoot());
        //////////////////////// / operator ////////////////////////
        BinaryTree* returnTreeDivide = treeProcessor->buildASTFromSubexpr("x/y");
        REQUIRE(returnTreeDivide->getRoot()->getElement() == "/");
        REQUIRE(returnTreeDivide->getRoot()->getParent() == NULL);
        REQUIRE(returnTreeDivide->getRoot()->getLeftElement()->getElement() == "x");
        REQUIRE(returnTreeDivide->getRoot()->getRightElement()->getElement() == "y");

        REQUIRE(returnTreeDivide->getLeftSubtree()->getElement() == "x");
        REQUIRE(returnTreeDivide->getRightSubtree()->getElement() == "y");
        REQUIRE(returnTreeDivide->getLeftSubtree()->getParent()
            == returnTreeDivide->getRoot());
        REQUIRE(returnTreeDivide->getRightSubtree()->getParent()
            == returnTreeDivide->getRoot());
        //////////////////////// % operator ////////////////////////
        BinaryTree* returnTreeMod = treeProcessor->buildASTFromSubexpr("x%y");
        REQUIRE(returnTreeMod->getRoot()->getElement() == "%");
        REQUIRE(returnTreeMod->getRoot()->getParent() == NULL);
        REQUIRE(returnTreeMod->getRoot()->getLeftElement()->getElement() == "x");
        REQUIRE(returnTreeMod->getRoot()->getRightElement()->getElement() == "y");

        REQUIRE(returnTreeMod->getLeftSubtree()->getElement() == "x");
        REQUIRE(returnTreeMod->getRightSubtree()->getElement() == "y");
        REQUIRE(returnTreeMod->getLeftSubtree()->getParent() == returnTreeMod->getRoot());
        REQUIRE(returnTreeMod->getRightSubtree()->getParent() == returnTreeMod->getRoot());
    }
    SECTION("One operator, more than one character variable") {
        //////////////////////// + operator ////////////////////////
        BinaryTree* returnTreePlus = treeProcessor->buildASTFromSubexpr("banana+apple");
        REQUIRE(returnTreePlus->getRoot()->getElement() == "+");
        REQUIRE(returnTreePlus->getRoot()->getParent() == NULL);
        REQUIRE(returnTreePlus->getRoot()->getLeftElement()->getElement() == "banana");
        REQUIRE(returnTreePlus->getRoot()->getRightElement()->getElement() == "apple");

        REQUIRE(returnTreePlus->getLeftSubtree()->getElement() == "banana");
        REQUIRE(returnTreePlus->getRightSubtree()->getElement() == "apple");
        REQUIRE(returnTreePlus->getLeftSubtree()->getParent() == returnTreePlus->getRoot());
        REQUIRE(returnTreePlus->getRightSubtree()->getParent() == returnTreePlus->getRoot());

        //////////////////////// - operator ////////////////////////
        BinaryTree* returnTreeMinus = treeProcessor->buildASTFromSubexpr("monkey-horse");
        REQUIRE(returnTreeMinus->getRoot()->getElement() == "-");
        REQUIRE(returnTreeMinus->getRoot()->getParent() == NULL);
        REQUIRE(returnTreeMinus->getRoot()->getLeftElement()->getElement() == "monkey");
        REQUIRE(returnTreeMinus->getRoot()->getRightElement()->getElement() == "horse");

        REQUIRE(returnTreeMinus->getLeftSubtree()->getElement() == "monkey");
        REQUIRE(returnTreeMinus->getRightSubtree()->getElement() == "horse");
        REQUIRE(returnTreeMinus->getLeftSubtree()->getParent() == returnTreeMinus->getRoot());
        REQUIRE(returnTreeMinus->getRightSubtree()->getParent() == returnTreeMinus->getRoot());
        //////////////////////// * operator ////////////////////////
        BinaryTree* returnTreeMultiply = treeProcessor->buildASTFromSubexpr("coffee*caffeine");
        REQUIRE(returnTreeMultiply->getRoot()->getElement() == "*");
        REQUIRE(returnTreeMultiply->getRoot()->getParent() == NULL);
        REQUIRE(returnTreeMultiply->getRoot()->getLeftElement()->getElement() == "coffee");
        REQUIRE(returnTreeMultiply->getRoot()->getRightElement()->getElement() == "caffeine");

        REQUIRE(returnTreeMultiply->getLeftSubtree()->getElement() == "coffee");
        REQUIRE(returnTreeMultiply->getRightSubtree()->getElement() == "caffeine");
        REQUIRE(returnTreeMultiply->getLeftSubtree()->getParent()
            == returnTreeMultiply->getRoot());
        REQUIRE(returnTreeMultiply->getRightSubtree()->getParent()
            == returnTreeMultiply->getRoot());
        //////////////////////// / operator ////////////////////////
        BinaryTree* returnTreeDivide = treeProcessor->buildASTFromSubexpr("bird/wings");
        REQUIRE(returnTreeDivide->getRoot()->getElement() == "/");
        REQUIRE(returnTreeDivide->getRoot()->getParent() == NULL);
        REQUIRE(returnTreeDivide->getRoot()->getLeftElement()->getElement() == "bird");
        REQUIRE(returnTreeDivide->getRoot()->getRightElement()->getElement() == "wings");

        REQUIRE(returnTreeDivide->getLeftSubtree()->getElement() == "bird");
        REQUIRE(returnTreeDivide->getRightSubtree()->getElement() == "wings");
        REQUIRE(returnTreeDivide->getLeftSubtree()->getParent()
            == returnTreeDivide->getRoot());
        REQUIRE(returnTreeDivide->getRightSubtree()->getParent()
            == returnTreeDivide->getRoot());
        //////////////////////// % operator ////////////////////////
        BinaryTree* returnTreeMod = treeProcessor->buildASTFromSubexpr("sour%grapes");
        REQUIRE(returnTreeMod->getRoot()->getElement() == "%");
        REQUIRE(returnTreeMod->getRoot()->getParent() == NULL);
        REQUIRE(returnTreeMod->getRoot()->getLeftElement()->getElement() == "sour");
        REQUIRE(returnTreeMod->getRoot()->getRightElement()->getElement() == "grapes");

        REQUIRE(returnTreeMod->getLeftSubtree()->getElement() == "sour");
        REQUIRE(returnTreeMod->getRightSubtree()->getElement() == "grapes");
        REQUIRE(returnTreeMod->getLeftSubtree()->getParent() == returnTreeMod->getRoot());
        REQUIRE(returnTreeMod->getRightSubtree()->getParent() == returnTreeMod->getRoot());
    }
    SECTION("Two same operators") {
    
    }
    SECTION("Two operators of the same precedence") {
    
    }
}

TEST_CASE("Extract string from an AST") {

}

TEST_CASE("Extract all possible subexpression and populate PKB") {

}