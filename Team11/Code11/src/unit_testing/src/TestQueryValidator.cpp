//
// Created by Xuan Zhi on 13/9/21.
//

#include "PQL/QueryValidator.h"
#include "catch.hpp"

TEST_CASE("isSelectExist") {
    REQUIRE(QueryValidator::isSelectExist("Select v"));
    REQUIRE(QueryValidator::isSelectExist("Select    v"));
    REQUIRE(QueryValidator::isSelectExist("    Select v"));
    REQUIRE(QueryValidator::isSelectExist("Select Select"));
    REQUIRE_FALSE(QueryValidator::isSelectExist("Selects w"));
    REQUIRE_FALSE(QueryValidator::isSelectExist("w Select"));
    REQUIRE_FALSE(QueryValidator::isSelectExist("S$lect"));
}

TEST_CASE("isBracketValid") {
    REQUIRE(QueryValidator::isBracketValid("(,)"));
    REQUIRE(QueryValidator::isBracketValid("(v, )"));
    REQUIRE(QueryValidator::isBracketValid("(q, a)"));
    REQUIRE_FALSE(QueryValidator::isBracketValid("a, v)"));
    REQUIRE_FALSE(QueryValidator::isBracketValid("( , _"));
}

TEST_CASE("isContentExist") {
    REQUIRE(QueryValidator::isContentExist("a, _"));
    REQUIRE(QueryValidator::isContentExist("_, _"));
    REQUIRE(QueryValidator::isContentExist("_, a"));
    REQUIRE_FALSE(QueryValidator::isContentExist(", a"));
    REQUIRE_FALSE(QueryValidator::isContentExist(","));
    REQUIRE_FALSE(QueryValidator::isContentExist("_, "));
}
