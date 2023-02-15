#include "catch.hpp"
#include "Parser/ReadParser.h"
#include "ParserException.h"
using namespace std;

TEST_CASE("ReadParser - checkReadStmt Test")
{
	ReadParser readParser;

	std::vector<std::string> tokens1 = { "read", "a" };
	std::vector<std::string> tokens2 = { "read", "a+" };
	std::vector<std::string> tokens3 = { "read", "a", "c" };
	std::vector<std::string> tokens4 = { "reada", "a" };
	std::vector<std::string> tokens5 = { "reada" };
	std::vector<std::string> tokens6 = { "Read", "a" };
	std::vector<std::string> tokens7 = { "reaD", "a" };
	std::vector<std::string> tokens8 = { "rEaD", "a" };
	std::vector<std::string> tokens9 = { "read", "a-b" };
	std::vector<std::string> tokens10 = { "ReAd" };

	REQUIRE(readParser.checkReadStmt(tokens1) == true);
	REQUIRE(readParser.checkReadStmt(tokens2) == false);
	REQUIRE(readParser.checkReadStmt(tokens3) == false);
	REQUIRE(readParser.checkReadStmt(tokens4) == false);
	REQUIRE(readParser.checkReadStmt(tokens5) == false);
	REQUIRE(readParser.checkReadStmt(tokens6) == false);
	REQUIRE(readParser.checkReadStmt(tokens7) == false);
	REQUIRE(readParser.checkReadStmt(tokens8) == false);
	REQUIRE(readParser.checkReadStmt(tokens9) == false);
	REQUIRE(readParser.checkReadStmt(tokens10) == false);
}

TEST_CASE("ReadParser - parseReadStmt Test")
{
	ReadParser readParser;

	std::string stmt1 = "read a";
	std::string stmt2 = "reada";
	std::string stmt3 = "read a b";
	std::string stmt4 = "read";
	std::string stmt5 = "Read a";
	std::string stmt6 = "read a;";
	std::string stmt7 = "read 2";
	std::string stmt8 = "read a+b";
	std::string stmt9 = "rEaD a";
	std::string stmt10 = " ";

	REQUIRE(readParser.parseReadStmt(stmt1) == "a");
	REQUIRE_THROWS_AS(readParser.parseReadStmt(stmt2), ParserException);
	REQUIRE_THROWS_AS(readParser.parseReadStmt(stmt3), ParserException);
	REQUIRE_THROWS_AS(readParser.parseReadStmt(stmt4), ParserException);
	REQUIRE_THROWS_AS(readParser.parseReadStmt(stmt5), ParserException);
	REQUIRE_THROWS_AS(readParser.parseReadStmt(stmt6), ParserException);
	REQUIRE_THROWS_AS(readParser.parseReadStmt(stmt7), ParserException);
	REQUIRE_THROWS_AS(readParser.parseReadStmt(stmt8), ParserException);
	REQUIRE_THROWS_AS(readParser.parseReadStmt(stmt9), ParserException);
	REQUIRE_THROWS_AS(readParser.parseReadStmt(stmt10), ParserException);
}
