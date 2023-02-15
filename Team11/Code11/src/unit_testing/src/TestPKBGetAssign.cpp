#include "PKB/PKB.h"
#include "Parser/Parser.h"
#include "catch.hpp"
#include <string>

PKB &pkb3 = pkb3.getInstance();
Parser* parser2 = new Parser();
/*
procedure simple {
1 p = 4;
2 a = p * 2;
3 if (c == k) then {
4 pattern = p * a * a + p * ifs + p * w;
5 while(w > 0) {
6 call ast;
7 w = w - 1;}
8 ifs = ifs -1;}
else {
9 read p;
10 print pattern;}}
 */
TEST_CASE("clear 766869") {
    pkb3.clear();
}

TEST_CASE("getAssignRHSVarsVector") {
    pkb3.addAssignRHSVar(2, "p");
    pkb3.addAssignRHSVar(2, "q");
    vector<string> test2 = pkb3.getAssignRHSVarsVector(2);
    vector<string> expected2 = {"p", "q"};
    REQUIRE(test2 == expected2);
}
