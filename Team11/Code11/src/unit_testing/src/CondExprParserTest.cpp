#include "catch.hpp"
#include "Parser/CondExprParser.h"
#include "StatementTypes.h"
#include "ParserException.h"
using namespace std;

TEST_CASE("Conditional Expr Parser Test - Positive 1")
{
    CondExprParser condExprParser;
    std::string condString1 = "(!(x>0))"; //no spaces
    std::string condString2 = "(!(x<=0))";
    std::string condString3 = "(!(x==9))";
    std::string condString4 = "(!((p+8)>=(q-9)))";
    std::string condString5 = "(x<67)";
    std::string condString6 = "(!(((a-c+50%f*g-h!=(a+b+c-d*e+(g/h)))&&(j==b-c+50))||(b>=(50+45)%33-a)))";
    REQUIRE(condExprParser.checkConditionSyntax(condString1, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString2, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString3, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString4, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString5, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString6, StatementTypes::NO_TYPE));
}

TEST_CASE("Conditional Expr Parser Test - Positive 2")
{
    CondExprParser condExprParser;
    std::string condString1 = "((x>0)&&(y<=9))"; //no spaces, no outer brackets
    std::string condString2 = "((x<=0)&&((x>=45)&&(u!=po)))";
    std::string condString3 = "((x==9)&&(((x+3%5)<(p-q/8))&&(var/5>=var*5)))";
    std::string condString4 = "((!((p+8)>=(q-9)))&&((c<90)||(q>99)))"; //left of && should be within brackets
    std::string condString5 = "((a+b-(c%d)*(e/f)<=g+h-i)||(!(a+b+c-90<=b-g+10)))";
    std::string condString6 = "(((a+b-(c%d)*(e/f)<=g+h-i)&&(x+3>=y-5))||((!(a+b+c-90<=b-g+10))||((a+b-20<=45%63)&&(q==90-23+670*7))))"; //102 chars
    std::string condString7 = "((!(((7777777-abcdefg)>(526445731))&&(2320%8989%673==a+b-c/d*e%f)))&&((!(500+400-300%200-100<=(a*b/d+e)))||(!(45%fg+gh-ad*bc>=((((((alba-jane+xo)/(67*89-90)%rog)))))))))";
    REQUIRE(condExprParser.checkConditionSyntax(condString1, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString2, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString3, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString4, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString5, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString6, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString7, StatementTypes::NO_TYPE));
}

TEST_CASE("Conditional Expr Parser Test - Positive 3")
{
    CondExprParser condExprParser;
    std::string condString1 = "((x>0)||(y<=9))"; //no spaces, no outer brackets
    std::string condString2 = "((x<=0)||((x>=45)||(u!=po)))";
    std::string condString3 = "((x==9)||(((x+3%5)<(p-q/8))||(var/5>=var*5)))";
    std::string condString4 = "((!((p+8)>=(q-9)))||((c<90)&&(q>99)))";
    std::string condString5 = "(m<l+p-10+a+b)";
    std::string condString6 = "(F>=a+b-c%10)";
    std::string condString7 = "(11111>(2222%3333+4444-555*6666)-777)";
    REQUIRE(condExprParser.checkConditionSyntax(condString1, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString2, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString3, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString4, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString5, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString6, StatementTypes::NO_TYPE));
    REQUIRE(condExprParser.checkConditionSyntax(condString7, StatementTypes::NO_TYPE));
}

TEST_CASE("Conditional Expr Parser Test - Negative 1")
{
    CondExprParser condExprParser;
    std::string condString1 = "((x>0&&(y<=9))"; //exits, so test terminated //no spaces, no outer brackets
    std::string condString2 = "((x<=0)&&((x=45)&&(u!=po)))";
    std::string condString3 = "((x==9)&&(((x+3%5)<(p-q/8))&(var/5>=var*5)))"; //current check for && || not good enough
    std::string condString4 = "(!((p+8)>=(q-9)&&((c<90)|(q>99))))";
    std::string condString5 = "(!((p+8)>=(q-9))&&((c<90)|(q>99)))"; //doesnt process && afteer the ! block
    std::string condString6 = "(x-90)";
    std::string condString7 = "(abc>=rty&&45<30)";
    std::string condString8 = "(((abc>=rty) && (ab<c))&&45<30)";
    REQUIRE_THROWS_AS(condExprParser.checkConditionSyntax(condString1, StatementTypes::NO_TYPE), ParserException);
    REQUIRE_FALSE(condExprParser.checkConditionSyntax(condString2, StatementTypes::NO_TYPE));
    REQUIRE_FALSE(condExprParser.checkConditionSyntax(condString3, StatementTypes::NO_TYPE));
    REQUIRE_FALSE(condExprParser.checkConditionSyntax(condString4, StatementTypes::NO_TYPE));
    REQUIRE_FALSE(condExprParser.checkConditionSyntax(condString5, StatementTypes::NO_TYPE));
    REQUIRE_FALSE(condExprParser.checkConditionSyntax(condString6, StatementTypes::NO_TYPE));
    REQUIRE_THROWS_AS(condExprParser.checkConditionSyntax(condString7, StatementTypes::NO_TYPE), ParserException);
    REQUIRE_THROWS_AS(condExprParser.checkConditionSyntax(condString8, StatementTypes::NO_TYPE), ParserException);
}

TEST_CASE("Cond Expr Parser Test - Negative 2")
{
    CondExprParser condExprParser;
    std::string condString1 = "((x>0)&&(y<=9)))"; //extra close at the end
    std::string condString2 = "((x>0)))&&(y<=9))"; //extra close in lhs
    std::string condString3 = "(()&&(x<(c+56%(9))))"; //empty bracket
    std::string condString4 = "(((x-(90%67))&&(50<d))"; //extra open bracket
    std::string condString5 = "()";
    std::string condString6 = "((x>=9)||((x-c<(x+t)))"; //extra open rhs
    REQUIRE_THROWS_AS(condExprParser.checkConditionSyntax(condString2, StatementTypes::NO_TYPE), ParserException);
    REQUIRE_THROWS_AS(condExprParser.checkConditionSyntax(condString3, StatementTypes::NO_TYPE), ParserException);
    REQUIRE_THROWS_AS(condExprParser.checkConditionSyntax(condString1, StatementTypes::NO_TYPE), ParserException);
    REQUIRE_THROWS_AS(condExprParser.checkConditionSyntax(condString4, StatementTypes::NO_TYPE), ParserException);
    REQUIRE_THROWS_AS(condExprParser.checkConditionSyntax(condString5, StatementTypes::NO_TYPE), ParserException);
    REQUIRE_THROWS_AS(condExprParser.checkConditionSyntax(condString6, StatementTypes::NO_TYPE), ParserException);
}


