//
// Created by Xuan Zhi on 11/9/21.
//

#include "PQL/QueryPreprocessor.h"
#include "catch.hpp"

/*
assign a; while w;
Select a such that Parent*(w, a) pattern a (v, _)
*/

QueryPreprocessor::QueryPreprocessor() {

}
// more extensive test is done in integration test as QPP and QE are highly coupled

TEST_CASE("addDeclaration") {
    REQUIRE(QueryPreprocessor::addDeclaration("assign a"));
    REQUIRE(QueryPreprocessor::addDeclaration("while w"));
    REQUIRE(QueryPreprocessor::addDeclaration("stmt s"));
    REQUIRE(QueryPreprocessor::addDeclaration("stmt w"));
}

TEST_CASE("parseDeclaration") {
    QueryPreprocessor::resetQE();
    QueryPreprocessor::declarations = new queue<string>();
    QueryPreprocessor::addDeclaration("assign a");
    REQUIRE(QueryPreprocessor::parseDeclaration());
}