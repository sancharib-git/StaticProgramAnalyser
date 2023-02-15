#include "catch.hpp"
#include "Parser/CallParser.h"
#include "ParserException.h"
using namespace std;

TEST_CASE("CallParser - checkCallStmt Test")
{
	CallParser callParser;

	std::vector<std::string> tokens1 = { "call", "a" };
	std::vector<std::string> tokens2 = { "call", "a+" };
	std::vector<std::string> tokens3 = { "call", "a", "c" };
	std::vector<std::string> tokens4 = { "calla", "a" };
	std::vector<std::string> tokens5 = { "calla" };
	std::vector<std::string> tokens6 = { "Call", "a" };
	std::vector<std::string> tokens7 = { "calL", "a" };
	std::vector<std::string> tokens8 = { "cAlL", "a" };
	std::vector<std::string> tokens9 = { "call", "a-b" };
	std::vector<std::string> tokens10 = { "caLL" };

	REQUIRE(callParser.checkCallStmt(tokens1) == true);
	REQUIRE(callParser.checkCallStmt(tokens2) == false);
	REQUIRE(callParser.checkCallStmt(tokens3) == false);
	REQUIRE(callParser.checkCallStmt(tokens4) == false);
	REQUIRE(callParser.checkCallStmt(tokens5) == false);
	REQUIRE(callParser.checkCallStmt(tokens6) == false);
	REQUIRE(callParser.checkCallStmt(tokens7) == false);
	REQUIRE(callParser.checkCallStmt(tokens8) == false);
	REQUIRE(callParser.checkCallStmt(tokens9) == false);
	REQUIRE(callParser.checkCallStmt(tokens10) == false);
}

TEST_CASE("CallParser - parseCallStmt Test")
{
	CallParser callParser;

	std::string stmt1 = "call a";
	std::string stmt2 = "calla";
	std::string stmt3 = "call a b";
	std::string stmt4 = "call";
	std::string stmt5 = "Call a";
	std::string stmt6 = "call a;";
	std::string stmt7 = "call 2";
	std::string stmt8 = "call a+b";
	std::string stmt9 = "cAlL a";
	std::string stmt10 = " ";

	REQUIRE(callParser.parseCallStmt(stmt1) == "a");
	REQUIRE_THROWS_AS(callParser.parseCallStmt(stmt2), ParserException);
	REQUIRE_THROWS_AS(callParser.parseCallStmt(stmt3), ParserException);
	REQUIRE_THROWS_AS(callParser.parseCallStmt(stmt4), ParserException);
	REQUIRE_THROWS_AS(callParser.parseCallStmt(stmt5), ParserException);
	REQUIRE_THROWS_AS(callParser.parseCallStmt(stmt6), ParserException);
	REQUIRE_THROWS_AS(callParser.parseCallStmt(stmt7), ParserException);
	REQUIRE_THROWS_AS(callParser.parseCallStmt(stmt8), ParserException);
	REQUIRE_THROWS_AS(callParser.parseCallStmt(stmt9), ParserException);
	REQUIRE_THROWS_AS(callParser.parseCallStmt(stmt10), ParserException);
}