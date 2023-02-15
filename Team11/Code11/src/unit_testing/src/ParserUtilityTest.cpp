#include "catch.hpp"
#include "Parser/ParserUtility.h"
#include "ParserException.h"
using namespace std;

TEST_CASE("Extract brackets String - positive")
{
    string string1 = "(a+B-((a-b%6000+ffff-(a*b-1000/ghbsd)))/(xxx-1000*1000/45%tgh+we))";
    string expected1 = "a+B-((a-b%6000+ffff-(a*b-1000/ghbsd)))/(xxx-1000*1000/45%tgh+we)";
    string1 = ParserUtility::extractBracketString(string1);
    REQUIRE(string1 == expected1);
    string string2 = "((cadblcladc/(sdciadcadc+adkcakc)*80000-1201218021*(uaiclauc%972192))+a+B-((a-b%6000+ffff-(a*b-1000/ghbsd)))/(xxx-1000*1000/45%tgh+we))";
    string expected2 = "(cadblcladc/(sdciadcadc+adkcakc)*80000-1201218021*(uaiclauc%972192))+a+B-((a-b%6000+ffff-(a*b-1000/ghbsd)))/(xxx-1000*1000/45%tgh+we)";
    string2 = ParserUtility::extractBracketString(string2);
    REQUIRE(string2 == expected2);
}

TEST_CASE("Extract bracket forwards - positive")
{
    string brackstring1 = "(a+b-(b+c/(e%f)))";
    int expected1 = 16;
    int result1 = ParserUtility::extractBracketsForwards(brackstring1);
    REQUIRE(expected1 == result1);
    string brackstring2 = "((((((((((a+b-c+d/(a*d/d%c)))))))))))";
    int result2 = ParserUtility::extractBracketsForwards(brackstring2);
    REQUIRE(result2 == 36);
}

TEST_CASE("Extract bracket string negative")
{
    ParserUtility parserUtility;
    string string1 = "(a+B-((a-b%6000+ffff-(a*b-1000/ghbsd)))/(xxx-1000*1000/45%tgh+we)";
    REQUIRE_THROWS_AS(parserUtility.extractBracketString(string1), ParserException);
    string string3 = "x+)(56/7(";
    REQUIRE_THROWS_AS(parserUtility.extractBracketString(string3), ParserException);
    string string4 = "())(ozvoaioa+4598223-aisdbcijdc/13018)";
    REQUIRE_THROWS_AS(parserUtility.extractBracketString(string4), ParserException);
}

TEST_CASE("Extract brackets backwards")
{
    ParserUtility parserUtility;
    string string1 = "((300%56))/(((fj-100*(700))))";
    REQUIRE(parserUtility.extractBracketsBackwards(string1,28) == 11);
    string string3 = "x+)(56/7(";
    REQUIRE_THROWS_AS(parserUtility.extractBracketsBackwards(string3,9), ParserException);
}
