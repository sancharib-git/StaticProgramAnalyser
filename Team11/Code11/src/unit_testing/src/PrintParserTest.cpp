#include "catch.hpp"
#include "Parser/PrintParser.h"
#include "ParserException.h"
using namespace std;

TEST_CASE("PrintParser - checkPrintStmt Test") 
{
	PrintParser printParser;

	std::vector<std::string> tokens1 = { "print", "a"};
	std::vector<std::string> tokens2 = { "print", "a+" };
	std::vector<std::string> tokens3 = { "print", "a", "c"};
	std::vector<std::string> tokens4 = { "printa", "a" };
	std::vector<std::string> tokens5 = { "printa"};
	std::vector<std::string> tokens6 = { "Print", "a" };
	std::vector<std::string> tokens7 = { "prinT", "a" };
	std::vector<std::string> tokens8 = { "pRiNt", "a" };
	std::vector<std::string> tokens9 = { "print", "a-b"};
	std::vector<std::string> tokens10 = { "PrInt"};

	REQUIRE(printParser.checkPrintStmt(tokens1) == true);
	REQUIRE(printParser.checkPrintStmt(tokens2) == false);
	REQUIRE(printParser.checkPrintStmt(tokens3) == false);
	REQUIRE(printParser.checkPrintStmt(tokens4) == false);
	REQUIRE(printParser.checkPrintStmt(tokens5) == false);
	REQUIRE(printParser.checkPrintStmt(tokens6) == false);
	REQUIRE(printParser.checkPrintStmt(tokens7) == false);
	REQUIRE(printParser.checkPrintStmt(tokens8) == false);
	REQUIRE(printParser.checkPrintStmt(tokens9) == false);
	REQUIRE(printParser.checkPrintStmt(tokens10) == false);
}

TEST_CASE("PrintParser - parsePrintStmt Test")
{
	PrintParser printParser;

	std::string stmt1 = "print a";
	std::string stmt2 = "printa";
	std::string stmt3 = "print a b";
	std::string stmt4 = "print";
	std::string stmt5 = "Print a";
	std::string stmt6 = "print a;";
	std::string stmt7 = "print 2";
	std::string stmt8 = "print a+b";
	std::string stmt9 = "pRiNt a";
	std::string stmt10 = " ";

	REQUIRE(printParser.parsePrintStmt(stmt1) == "a");
	REQUIRE_THROWS_AS(printParser.parsePrintStmt(stmt2), ParserException);
	REQUIRE_THROWS_AS(printParser.parsePrintStmt(stmt3), ParserException);
	REQUIRE_THROWS_AS(printParser.parsePrintStmt(stmt4), ParserException);
	REQUIRE_THROWS_AS(printParser.parsePrintStmt(stmt5), ParserException);
	REQUIRE_THROWS_AS(printParser.parsePrintStmt(stmt6), ParserException);
	REQUIRE_THROWS_AS(printParser.parsePrintStmt(stmt7), ParserException);
	REQUIRE_THROWS_AS(printParser.parsePrintStmt(stmt8), ParserException);
	REQUIRE_THROWS_AS(printParser.parsePrintStmt(stmt9), ParserException);
	REQUIRE_THROWS_AS(printParser.parsePrintStmt(stmt10), ParserException);
}
