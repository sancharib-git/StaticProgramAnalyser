#include "catch.hpp"
#include "Parser/Parser.h"
#include "Parser/ParserUtility.h"
#include "ParserException.h"
using namespace std;

TEST_CASE("Parser Test - Extract Block - Positive 1")
{
    std::string code = "procedure test {"
                       "a=b;"
                       "b=b+7-(8+(b/(7%3)));"
                       "while(a>t){"
                       "if(b<v) then{"
                       "a=j*2;"
                       "} else{"
                       "x=o;"
                       "}"
                       "}"
                       "}";
    //100
    std::string insideBlock = "a=b;"
                              "b=b+7-(8+(b/(7%3)));"
                              "while(a>t){"
                              "if(b<v) then{"
                              "a=j*2;"
                              "} else{"
                              "x=o;"
                              "}"
                              "}";
    int lastPos = (int)ParserUtility::extractBraces(code);
    size_t first_pos = code.find('{');
    std::string toCompare = code.substr(first_pos+1, lastPos-first_pos-1);
    REQUIRE(insideBlock == toCompare);
}

TEST_CASE("Parser Test - Extract Block - Positive 2")
{
    std::string code = "while(x>0){"
                       "if(x<0)then{"
                       "while(i<8){"
                       "if(v==v) then{"
                       "x=0;"
                       "} else {"
                       "x=0;"
                       "}"
                       "} else{"
                       "c=0;"
                       "}"
                       "}"
                       "}";
    //100
    std::string insideBlock = "if(x<0)then{"
                              "while(i<8){"
                              "if(v==v) then{"
                              "x=0;"
                              "} else {"
                              "x=0;"
                              "}"
                              "} else{"
                              "c=0;"
                              "}"
                              "}";
    int lastPos = (int)ParserUtility::extractBraces(code);
    size_t first_pos = code.find('{');
    std::string toCompare = code.substr(first_pos+1, lastPos-first_pos-1);
    REQUIRE(insideBlock == toCompare);
}

TEST_CASE("Parser Test - Extract Block - Negative 1")
{
    std::string code = "while(x>0){"
                       "if(x<0)then{"
                       "while(i<8){"
                       "if(v==v) then{"
                       "x=0;"
                       "} else {"
                       "x=0;"
                       "}"
                       "} else{"
                       "c=0;"
                       "}"
                       "}";
    REQUIRE_THROWS_AS(ParserUtility::extractBraces(code), ParserException); //terminated bc system exit from parser
}

TEST_CASE("Parser Test - Multiple Procedures - Positive 1")
{
    Parser parser;
    std::string multipleProcs = "procedure a {"
                                "a=b;"
                                "c=d;"
                                "}"
                                "procedure b{"
                                "e=f;"
                                "f=g;"
                                "g=h;"
                                "}";
    REQUIRE(parser.Parse(multipleProcs) == 0);
}
TEST_CASE("Parser Test - Multiple Procedures - Positive 2")
{
    Parser parser;
    std::string multipleProcs = "procedure a {"
                                "a=b;"
                                "c=d;"
                                "}"
                                "procedure b{"
                                "e=f;"
                                "f=g;"
                                "g=h;"
                                "}"
                                "procedure c{"
                                "a=b;"
                                "b=c+67/98;"
                                "while(x<0){"
                                "read p;"
                                "print x;"
                                "}"
                                "}";
    REQUIRE(parser.Parse(multipleProcs) == 0);
}

TEST_CASE("Parser - Multiple Procedures with nesting - Positive")
{
    Parser parser;
    std::string multipleProcs = "procedure a {"
                                "a=b;"
                                "c=d;"
                                "}"
                                "procedure b{"
                                "e=f;"
                                "f=g;"
                                "g=h;"
                                "}"
                                "procedure c{"
                                "a=b;"
                                "b=c+67/98;"
                                "while(x<0){"
                                "read p;"
                                "print x;"
                                "}"
                                "}"
                                "procedure d{"
                                "while((b>v)&&(r<=9)){"
                                "if(x==9) then {"
                                "x=9;"
                                "} else{"
                                "x=10;"
                                "while(xc>23){"
                                "vol=789;"
                                "read p;"
                                "print x;"
                                "}"
                                "}"
                                "}"
                                "}"
                                "   "
                                "  ";
    REQUIRE(parser.Parse(multipleProcs) == 0);
}

TEST_CASE("Multiple Procedures - Negative 1 - Missing brace for last procedure")
{
    Parser parser;
    std::string multipleProcs = "procedure a {"
                                "a=b;"
                                "c=d;"
                                "}"
                                "procedure b{"
                                "e=f;"
                                "f=g;"
                                "g=h;"
                                "}"
                                "procedure c{"
                                "a=b;"
                                "b=c+67/98;"
                                "while(x<0){"
                                "read p;"
                                "print x;"
                                "}"
                                "}"
                                "procedure d{"
                                "while((b>v)&&(r<=9)){"
                                "if(x==9) then {"
                                "x=9;"
                                "} else{"
                                "x=10;"
                                "while(xc>23){"
                                "vol=789;"
                                "read p;"
                                "print x;"
                                "}"
                                "}"
                                "}"
                                "   "
                                "  ";
    REQUIRE_THROWS_AS(parser.Parse(multipleProcs), ParserException);
}

TEST_CASE("Multiple Procedures - Negative 2 - Missing declaration")
{
    Parser parser;
    std::string multipleProcs = "procedure a {"
                                "a=b;"
                                "c=d;"
                                "}"
                                "procedure b{"
                                "e=f;"
                                "f=g;"
                                "g=h;"
                                "}"
                                "c{"
                                "a=b;"
                                "b=c+67/98;"
                                "while(x<0){"
                                "read p;"
                                "print x;"
                                "}"
                                "}"
                                "procedure d{"
                                "while((b>v)&&(r<=9)){"
                                "if(x==9) then {"
                                "x=9;"
                                "} else{"
                                "x=10;"
                                "while(xc>23){"
                                "vol=789;"
                                "read p;"
                                "print x;"
                                "}"
                                "}"
                                "}"
                                "   "
                                "  ";
    REQUIRE_THROWS_AS(parser.Parse(multipleProcs), ParserException);
}

TEST_CASE("Multiple Procedures - Negative 3 - Missing open brace")
{
    Parser parser;
    std::string multipleProcs = "procedure a {"
                                "a=b;"
                                "c=d;"
                                "}"
                                "procedure b"
                                "e=f;"
                                "f=g;"
                                "g=h;"
                                "}"
                                "procedure c{"
                                "a=b;"
                                "b=c+67/98;"
                                "while(x<0){"
                                "read p;"
                                "print x;"
                                "}"
                                "}"
                                "procedure d{"
                                "while((b>v)&&(r<=9)){"
                                "if(x==9) then {"
                                "x=9;"
                                "} else{"
                                "x=10;"
                                "while(xc>23){"
                                "vol=789;"
                                "read p;"
                                "print x;"
                                "}"
                                "}"
                                "}"
                                "}"
                                "   "
                                "  ";
    REQUIRE_THROWS_AS(parser.Parse(multipleProcs), ParserException);
}