//
// Created by X ZZ on 13/9/21.
//

#include "PKB/Relationship.h"
#include "PKB/Relationship.cpp"
#include "PKB/Table.cpp"
#include "catch.hpp"

Relationship<int, int> followsTable;

bool addTest(int stmt1, StatementTypes type1, int stmt2, StatementTypes type2) {
    return followsTable.add(stmt1, type1, stmt2, type2);
}
bool compair(pair<int, int> one, pair<int, int> two) {
    return (one.first < two.first) || (one.second < two.second);
}

vector<pair<int, int>> sort(vector<pair<int, int>> v) {
    sort(v.begin(), v.end(), compair);
    return v;
}

TEST_CASE("addR") {
    REQUIRE(addTest(1, ASSIGNMENT, 2, ASSIGNMENT));
    REQUIRE(addTest(2, ASSIGNMENT, 3, IF));
    REQUIRE(addTest(3, IF, 4, ASSIGNMENT));
    REQUIRE(addTest(4, ASSIGNMENT, 5, WHILE));
    REQUIRE(addTest(5, WHILE, 6, CALL));
    REQUIRE(addTest(6, CALL, 7, ASSIGNMENT));
    REQUIRE(addTest(7, ASSIGNMENT, 8, ASSIGNMENT));
    REQUIRE(addTest(9, READ, 10, PRINT));
}

TEST_CASE("getR") {
    SECTION("getRNN") {
        vector<std::pair<int, int> > expectedFollowsNN = {make_pair(1,2)};
        REQUIRE(followsTable.getNN(1, 2) == expectedFollowsNN);
    }
    SECTION("getRNN returns empty if no result") {
        vector<std::pair<int, int> > expectedEmpty = {};
        REQUIRE(followsTable.getNN(11, 10) == expectedEmpty);
    }
    SECTION("getRTT") {
        vector<std::pair<int, int> > expectedFollowsTT = {make_pair(1,2), make_pair(7, 8)};
        REQUIRE(followsTable.getTT(ASSIGNMENT, ASSIGNMENT) == expectedFollowsTT);
    }
    SECTION("getRTT returns empty if no result") {
        vector<std::pair<int, int> > expectedEmpty = {};
        REQUIRE(followsTable.getTT(IF, CALL) == expectedEmpty);
    }
    SECTION("getRNT") {
        vector<std::pair<int, int> > expectedFollowsNT = {make_pair(6,7)};
        REQUIRE(followsTable.getNT(6, ASSIGNMENT) == expectedFollowsNT);
    }
    SECTION("getRNT returns empty if no result") {
        vector<std::pair<int, int> > expectedEmpty = {};
        REQUIRE(followsTable.getNT(6, CALL) == expectedEmpty);
    }
    SECTION("getRTN") {
        vector<std::pair<int, int> > expectedFollowsTN = {make_pair(6,7)};
        REQUIRE(followsTable.getTN(CALL, 7) == expectedFollowsTN);
    }
    SECTION("getRTN returns empty if no result") {
        vector<std::pair<int, int> > expectedEmpty = {};
        REQUIRE(followsTable.getTN(WHILE, 10) == expectedEmpty);
    }
}

TEST_CASE("getR with NO_TYPE") {
    SECTION("follows(_, _)") {
        vector<std::pair<int, int> > expected = {make_pair(1,2), make_pair(2,3), make_pair(3,4),
                                                 make_pair(4,5), make_pair(5,6), make_pair(6,7),
                                                 make_pair(7,8), make_pair(9,10)};
        vector<std::pair<int, int> > test = followsTable.getTT(NO_TYPE, NO_TYPE);
        REQUIRE(sort(test) == expected);
    }
    SECTION("follows(ASSIGNMENT, _)") {
        vector<std::pair<int, int> > expected = {make_pair(1,2), make_pair(2,3),
                                                 make_pair(4,5), make_pair(7,8)};
        REQUIRE(sort(followsTable.getTT(ASSIGNMENT, NO_TYPE)) == expected);
    }
    SECTION("follows(_, ASSIGNMENT)") {
        vector<std::pair<int, int> > expected = {make_pair(1,2), make_pair(3,4),
                                                 make_pair(6,7), make_pair(7,8)};
        REQUIRE(sort(followsTable.getTT(NO_TYPE, ASSIGNMENT)) == expected);
    }
    SECTION("follows(_, 2)") {
        vector<std::pair<int, int> > expected = {make_pair(1,2)};
        REQUIRE(sort(followsTable.getTN(NO_TYPE, 2)) == expected);
    }
    SECTION("follows(1,_)") {
        vector<std::pair<int, int> > expected = {make_pair(1,2)};
        REQUIRE(sort(followsTable.getNT(1, NO_TYPE)) == expected);
    }
}