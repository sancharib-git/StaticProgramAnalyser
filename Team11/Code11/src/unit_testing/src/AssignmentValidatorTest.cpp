#include "catch.hpp"
#include "Parser/AssignmentValidator.h"
#include "ParserException.h"
using namespace std;

TEST_CASE("Assignment Validator - isExpr Test - Positive")
{
    //RHS string has no spaces
    AssignmentValidator assignmentValidator;
    std::string expr1 = "5";
    std::string expr2 = "vble";
    std::string expr3 = "(4)";
    std::string expr4 = "(insidebrac)";
    std::string expr5 = "((doublebrac1))";
    std::string expr6 = "((99))";
    std::string expr7 = "3*33";
    std::string expr8 = "33098/67";
    std::string expr9 = "898923%3";
    std::string expr10 = "3*99/23";
    std::string expr11 = "445/890*2320";
    std::string expr12 = "8989%673*(34/56)";
    std::string expr13 = "32+67*87/23+(99*567)-((789/98))";
    std::string expr14 = "alba+671+jane/45-(334*7891%56-90)";
    std::string expr15 = "(alba-jane+xo)/(67*89-90)%rog";
    std::string expr16 = "(32+(67*87))/(23+(99*567)-((789/98)))";
    std::string expr17 = "((a+b-20)%30/70+90*(40-30)*40%(20+60-(30+50)))";
    REQUIRE(assignmentValidator.isExpr(expr1)==true);
    REQUIRE(assignmentValidator.isExpr(expr2));
    REQUIRE(assignmentValidator.isExpr(expr3));
    REQUIRE(assignmentValidator.isExpr(expr4));
    REQUIRE(assignmentValidator.isExpr(expr5));
    REQUIRE(assignmentValidator.isExpr(expr6));
    REQUIRE(assignmentValidator.isExpr(expr7));
    REQUIRE(assignmentValidator.isExpr(expr8));
    REQUIRE(assignmentValidator.isExpr(expr9));
    REQUIRE(assignmentValidator.isExpr(expr10));
    REQUIRE(assignmentValidator.isExpr(expr11));
    REQUIRE(assignmentValidator.isExpr(expr12));
    REQUIRE(assignmentValidator.isExpr(expr13));
    REQUIRE(assignmentValidator.isExpr(expr14));
    REQUIRE(assignmentValidator.isExpr(expr15));
    REQUIRE(assignmentValidator.isExpr(expr16));
    REQUIRE(assignmentValidator.isExpr(expr17));

}

TEST_CASE("Assignment Validator - isExpr Test - Negative")
{
    AssignmentValidator assignmentValidator;
    std::string expr1 = "x+)(56/7("; //validity checked in tokens
    std::string expr2 = "xx-";
    std::string expr3 = "()+vv-09";
    std::string expr4 = "56++901--x";
    std::string expr5 = "(900-90";
    std::string expr6 = "61861-polls)";
    std::string expr7 = "(expr+expr-(x/45+(34%6))))";
    string expr8 = ")()()())((((0008888";
    string expr9 = "0-))))+()-(x-3+2(";
    REQUIRE_FALSE(assignmentValidator.isExpr(expr1));
    REQUIRE_FALSE(assignmentValidator.isExpr(expr2));
    REQUIRE_THROWS_AS(assignmentValidator.isExpr(expr3), ParserException);
    REQUIRE_FALSE(assignmentValidator.isExpr(expr4));
    REQUIRE_FALSE(assignmentValidator.isExpr(expr5));
    REQUIRE_FALSE(assignmentValidator.isExpr(expr6));
    REQUIRE_THROWS_AS(assignmentValidator.isExpr(expr7), ParserException);
    REQUIRE_FALSE(assignmentValidator.isExpr(expr8));
    REQUIRE_FALSE(assignmentValidator.isExpr(expr9));
}

