#include "catch.hpp"
#include "Parser/Tokeniser.h"

using namespace std;


TEST_CASE("Tokenise - Standard assignment test") {
    Tokeniser tokeniser;
    std::string assignmentStmt = "x = 5";
    std::vector<std::string> tokens = {"x", "=", "5"};
    std::vector<std::string> tokenized = tokeniser.Tokenise(assignmentStmt);
    REQUIRE(tokenized == tokens);
}

TEST_CASE("Tokenise - Assignment with multiple spaces") {
    Tokeniser tokeniser;
    std::string assignmentStmt = "x     ="
                                 " 5";
    std::vector<std::string> tokens = {"x", "=", "5"};
    std::vector<std::string> tokenized = tokeniser.Tokenise(assignmentStmt);
    REQUIRE(tokenized == tokens);
}

TEST_CASE("Tokenise - Assignment, standard spacing with operators") {
    Tokeniser tokeniser;
    std::string assignmentStmt = "x = q + 3 * e";
    std::vector<std::string> tokens = {"x", "=", "q", "+", "3", "*", "e"};
    std::vector<std::string> tokenized = tokeniser.Tokenise(assignmentStmt);
    REQUIRE(tokenized == tokens);
}

TEST_CASE("Tokenise - Assignment with no spaces") {
    Tokeniser tokeniser;
    std::vector<std::string> assignmentTokens = {"x=5"};
    std::vector<std::string> tokens = {"x", "=", "5"};
    std::vector<std::string> tokenized = tokeniser.tokenizeAssignment(assignmentTokens);
    REQUIRE(tokenized == tokens);
}

