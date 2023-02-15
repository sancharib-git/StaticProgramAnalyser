//
// Created by X ZZ on 8/9/21.
//

#include "PKB/Table.h"
#include "PKB/PKB.h"
#include "catch.hpp"

Table<int, int, int, int> NNTable;
Table<int, StatementTypes, int, int> NTTable;
Table<StatementTypes, int, int, int> TNTable;
Table<StatementTypes, StatementTypes, int, int> TTTable;

TEST_CASE("addNN") {
    REQUIRE(NNTable.add(1,2,1,2));
}

TEST_CASE("addNT") {
    REQUIRE(NTTable.add(1,WHILE,1,2));
}

TEST_CASE("addTN") {
    REQUIRE(TNTable.add(ASSIGNMENT,2,1,2));
}

TEST_CASE("addTT") {
    REQUIRE(TTTable.add(ASSIGNMENT,WHILE,1,2));
}