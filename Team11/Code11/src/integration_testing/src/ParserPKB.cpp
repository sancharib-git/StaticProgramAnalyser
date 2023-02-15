#include "Parser/ProcedureParser.h"
#include "PKB/PKB.h"
#include "../../unit_testing/src/Stubforpkb.h"
#include <string>
#include <vector>
#include "StatementTypes.h"
#include "catch.hpp"
#include "DE/DesignExtractor.h"
#include "Parser/Parser.h"

using namespace std;

string procString =
        string("procedure simple{ p=4; a = p * 2;")
        + string("if (c == k) then {")
        + string("pattern = p * a * a + p * ifs + p * w;")
        + string("while(w > 0) { print c; w = w - 1; } ifs = ifs - 1;")
        + string("} else {read p;print pattern;}}");

PKB &pkb = pkb.getInstance();
ProcedureParser procedureParser;
Parser parser;
DesignExtractor* designExtractor;

//TEST_CASE("Initialize & parse") {
//    pkb.clear();
//    Parser* parser = new Parser();
//    std::string fileString = parser->fileToString("ParserPKB.txt");
//    parser->Parse(fileString);
//}

TEST_CASE("Parser-PKB Test") {
    SECTION("Initialize and parse") {
        pkb.clear();
        parser.Parse(procString);
        //procedureParser.parseProcedure(procString);
        designExtractor->populateDesignAbstractions();
    }
    SECTION("Get procedure") {
        REQUIRE(*(pkb.getProcList()) == expected_procedureList);
    }
    SECTION("Get vars") {
        REQUIRE(*(pkb.getAllVars()) == expected_varList);
        REQUIRE(pkb.getTotalNumOfVars() == expected_varList.size());
    }
    SECTION("Get const") {
        REQUIRE(*(pkb.getAllConsts()) == expected_constList);
        REQUIRE(pkb.getTotalNumOfConsts() == expected_constList.size());
    }
    SECTION("Get statement") {
        REQUIRE(pkb.getStatementType(1) == ASSIGNMENT);
        REQUIRE(pkb.getStatementType(2) == ASSIGNMENT);
        REQUIRE(pkb.getStatementType(3) == IF);
        REQUIRE(pkb.getStatementType(4) == ASSIGNMENT);
        REQUIRE(pkb.getStatementType(5) == WHILE);
        REQUIRE(pkb.getStatementType(6) == PRINT);
        REQUIRE(pkb.getStatementType(7) == ASSIGNMENT);
        REQUIRE(pkb.getStatementType(8) == ASSIGNMENT);
        REQUIRE(pkb.getStatementType(9) == READ);
        REQUIRE(pkb.getStatementType(10) == PRINT);
    }
    SECTION("Get assignments") {
        REQUIRE(*(pkb.getAssignList()) == expected_assignList);
    }
    SECTION("Get read") {
        REQUIRE(*(pkb.getReadStmtList()) == expected_readStmtList);
        REQUIRE(pkb.getReadVar(9) == "p");
        REQUIRE(pkb.getReadVar(3) == "");
    }
    SECTION("Get print") {
        REQUIRE(*(pkb.getPrintStmtList()) == expected_printStmtList);
        REQUIRE(pkb.getPrintVar(6) == "c");
        REQUIRE(pkb.getPrintVar(10) == "pattern");
        REQUIRE(pkb.getPrintVar(11) == "");
    }
    SECTION("Get if") {
        REQUIRE(*(pkb.getIfList()) == expected_ifList);
    }
    SECTION("Get ifBlock") {
        REQUIRE(pkb.getIfBlockList(3) == expected_ifBlockList);
    }
    SECTION("Get elseBlock") {
        REQUIRE(pkb.getElseBlockList(3) == expected_elseBlockList);
    }
    SECTION("Get while") {
        REQUIRE(*(pkb.getWhileList()) == expected_whileList);
    }
    SECTION("Get whileBlock") {
        REQUIRE(pkb.getWhileBlockList(5) == expected_whileBlockList);
    }
    SECTION("Get control variables") {
        SECTION("getIfStmtsWithCV"){
            unordered_set<int> expected = {3};
            REQUIRE(pkb.getIfStmtsWithCV("c") == expected);
        }
        SECTION("getAllCVInIfStmt"){
            unordered_set<string> expected = {"c","k"};
            REQUIRE(pkb.getAllCVInIfStmt(3) == expected);
        }
        SECTION("getWhileStmtsWithCV"){
            unordered_set<int> expected = {5};
            REQUIRE(pkb.getWhileStmtsWithCV("w") == expected);
        }
        SECTION("getAllCVInWhileStmt"){
            unordered_set<string> expected = {"w"};
            REQUIRE(pkb.getAllCVInWhileStmt(5) == expected);
        }
    }
    SECTION("Get Follows") {
        SECTION("getFollowsNN") {
            vector<std::pair<int, int> > expectedFollowsNN = {make_pair(1, 2)};
            REQUIRE(pkb.getFollowsNN(1, 2) == expectedFollowsNN);
        }
        SECTION("getFollowsNN returns empty list if no result") {
            vector<std::pair<int, int> > expectedEmpty = {};
            REQUIRE(pkb.getFollowsNN(11, 10) == expectedEmpty);
        }
        SECTION("getFollowsTT") {
            vector<std::pair<int, int> > expectedFollowsTT = {make_pair(1, 2)};
            REQUIRE(pkb.getFollowsTT(ASSIGNMENT, ASSIGNMENT) == expectedFollowsTT);
        }
        SECTION("getFollowsTT returns empty list if no result") {
            vector<std::pair<int, int> > expectedEmpty = {};
            REQUIRE(pkb.getFollowsTT(IF, CALL) == expectedEmpty);
        }
        SECTION("getFollowsNT") {
            vector<std::pair<int, int> > expectedFollowsNT = {make_pair(6, 7)};
            REQUIRE(pkb.getFollowsNT(6, ASSIGNMENT) == expectedFollowsNT);
        }
        SECTION("getFollowsNT returns empty list if no result") {
            vector<std::pair<int, int> > expectedEmpty = {};
            REQUIRE(pkb.getFollowsNT(6, CALL) == expectedEmpty);
        }
        SECTION("getFollowsTN") {
            vector<std::pair<int, int> > expectedFollowsTN = {make_pair(6, 7)};
            REQUIRE(pkb.getFollowsTN(PRINT, 7) == expectedFollowsTN);
        }
        SECTION("getFollowsTN returns empty list if no result") {
            vector<std::pair<int, int> > expectedEmpty = {};
            REQUIRE(pkb.getFollowsTN(ASSIGNMENT, 8) == expectedEmpty);
        }
    }
    SECTION("Get Follows Star") {
        SECTION("getFollowsStar") {
            SECTION("getFollowsStarNN") {
                vector<std::pair<int, int> > expectedFollowsStarNN = {make_pair(4, 8)};
                REQUIRE(pkb.getFollowsStarNN(4, 8) == expectedFollowsStarNN);
            }
            SECTION("getFollowsStarNN returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getFollowsStarNN(10, 11) == expectedEmpty);
            }
            SECTION("getFollowsStarTT") {
                vector<std::pair<int, int> > expectedFollowsStarTT
                        = {make_pair(1, 2), make_pair(4, 8)};
                REQUIRE(pkb.getFollowsStarTT(ASSIGNMENT, ASSIGNMENT) == expectedFollowsStarTT);
            }
            SECTION("getFollowsStarTT returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getFollowsStarTT(IF, CALL) == expectedEmpty);
            }
            SECTION("getFollowsStarNT") {
                vector<std::pair<int, int> > expectedFollowsStarNT = {make_pair(2, 3)};
                REQUIRE(pkb.getFollowsStarNT(2, IF) == expectedFollowsStarNT);
            }
            SECTION("getFollowsStarNT returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getFollowsStarNT(1, WHILE) == expectedEmpty);
            }
            SECTION("getFollowsStarTN") {
                vector<std::pair<int, int> > expectedFollowsStarTN = {make_pair(4, 5)};
                REQUIRE(pkb.getFollowsStarTN(ASSIGNMENT, 5) == expectedFollowsStarTN);
            }
            SECTION("getFollowsStarTN returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getFollowsStarTN(WHILE, 10) == expectedEmpty);
            }
        }
    }
    SECTION("Get Parent") {
        SECTION("getParent") {
            SECTION("getParentNN") {
                vector<std::pair<int, int> > expectedParentNN = {make_pair(3, 4)};
                REQUIRE(pkb.getParentNN(3, 4) == expectedParentNN);
            }
            SECTION("getParentNN returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getParentNN(1, 2) == expectedEmpty);
                REQUIRE(pkb.getParentNN(7, 5) == expectedEmpty);
            }
            SECTION("getParentTT") {
                vector<std::pair<int, int> > expectedParentTT = {make_pair(3, 4), make_pair(3, 8)};
                REQUIRE(pkb.getParentTT(IF, ASSIGNMENT) == expectedParentTT);
            }
            SECTION("getParentTT returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getParentTT(IF, CALL) == expectedEmpty);
            }
            SECTION("getParentNT") {
                vector<std::pair<int, int> > expectedParentNT = {make_pair(5, 7)};
                REQUIRE(pkb.getParentNT(5, ASSIGNMENT) == expectedParentNT);
            }
            SECTION("getParentNT returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getParentNT(3, CALL) == expectedEmpty);
            }
            SECTION("getParentTN") {
                vector<std::pair<int, int> > expectedParentTN = {make_pair(5, 6)};
                REQUIRE(pkb.getParentTN(WHILE, 6) == expectedParentTN);
            }
            SECTION("getParentTN returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getParentTN(WHILE, 5) == expectedEmpty);
            }
            SECTION("getParentTT with NO_TYPE as second arg") {
                vector<std::pair<int, int> > expected = {make_pair(5, 6), make_pair(5, 7)};
                REQUIRE(pkb.getParentTT(WHILE, NO_TYPE) == expected);
            }
            SECTION("getParentTT with NO_TYPE as first arg") {
                vector<std::pair<int, int> > expected = {make_pair(3, 4), make_pair(3, 8),
                                                         make_pair(5, 7)};
                REQUIRE(pkb.getParentTT(NO_TYPE, ASSIGNMENT) == expected);
            }
            SECTION("getParentTT returns empty with NO_TYPE as arg") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getParentTT(ASSIGNMENT, NO_TYPE) == expectedEmpty);
            }
        }
    }
    SECTION("Get Parent Star") {
        SECTION("addParentStar valid if first is container stmt and second is nested stmt") {
            REQUIRE(pkb.addParentStar(3, IF, 4, ASSIGNMENT));
            REQUIRE(pkb.addParentStar(3, IF, 5, WHILE));
            REQUIRE(pkb.addParentStar(3, IF, 6, CALL));
            REQUIRE(pkb.addParentStar(3, IF, 7, ASSIGNMENT));
            REQUIRE(pkb.addParentStar(3, IF, 8, ASSIGNMENT));
            REQUIRE(pkb.addParentStar(3, IF, 9, READ));
            REQUIRE(pkb.addParentStar(3, IF, 10, PRINT));
            REQUIRE(pkb.addParentStar(5, WHILE, 6, CALL));
            REQUIRE(pkb.addParentStar(5, WHILE, 7, ASSIGNMENT));
        }
        SECTION("getParentStar") {
            SECTION("getParentStarNN") {
                vector<std::pair<int, int> > expected = {make_pair(3, 4)};
                REQUIRE(pkb.getParentStarNN(3, 4) == expected);
            }
            SECTION("getParentStarNN returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getParentStarNN(1, 2) == expectedEmpty);
                REQUIRE(pkb.getParentStarNN(7, 5) == expectedEmpty);
            }
            SECTION("getParentStarTT") {
                vector<std::pair<int, int> > expected = {make_pair(3, 8),make_pair(3, 4), make_pair(3, 7)};
                vector<std::pair<int, int> > getParentStarTT = pkb.getParentStarTT(IF, ASSIGNMENT);
                REQUIRE(getParentStarTT.size() == expected.size());
                for (int i = 0; i < getParentStarTT.size(); i++) {
                    pair<int, int> innerPair = getParentStarTT[i];
                    bool isPresent = std::find(getParentStarTT.begin(), getParentStarTT.end(), innerPair)
                                     != getParentStarTT.end();
                    REQUIRE(isPresent);
                }

            }
            SECTION("getParentStarTT returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getParentStarTT(WHILE, READ) == expectedEmpty);
            }
            SECTION("getParentStarNT") {
                vector<std::pair<int, int> > expected = {make_pair(5, 7)};
                REQUIRE(pkb.getParentStarNT(5, ASSIGNMENT) == expected);
            }
            SECTION("getParentStarNT returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getParentStarNT(5, READ) == expectedEmpty);
            }
            SECTION("getParentStarTN") {
                vector<std::pair<int, int> > expected = {make_pair(5, 6)};
                REQUIRE(pkb.getParentStarTN(WHILE, 6) == expected);
            }
            SECTION("getParentStarTN returns empty list if no result") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getParentStarTN(WHILE, 5) == expectedEmpty);
            }
            SECTION("getParentStarTT with NO_TYPE as second arg") {
                vector<std::pair<int, int> > expected = {make_pair(5, 6), make_pair(5, 7)};
                REQUIRE(pkb.getParentStarTT(WHILE, NO_TYPE) == expected);
            }
            SECTION("getParentStarTT with NO_TYPE as first arg") {
                vector<std::pair<int, int> > expected
                        = {make_pair(3, 4), make_pair(3, 7),
                           make_pair(3, 8), make_pair(5, 7)};

                vector<std::pair<int, int> > getParentStarTT = pkb.getParentStarTT(NO_TYPE, ASSIGNMENT);
                REQUIRE(getParentStarTT.size() == expected.size());
                for (int i = 0; i < getParentStarTT.size(); i++) {
                    pair<int, int> innerPair = getParentStarTT[i];
                    bool isPresent = std::find(getParentStarTT.begin(), getParentStarTT.end(), innerPair)
                                     != getParentStarTT.end();
                    REQUIRE(isPresent);
                }
            }
            SECTION("getParentStarTT returns empty with NO_TYPE as arg") {
                vector<std::pair<int, int> > expectedEmpty = {};
                REQUIRE(pkb.getParentStarTT(ASSIGNMENT, NO_TYPE) == expectedEmpty);
            }
        }
    }
    SECTION("Get Uses") {
        SECTION("getUsesS") {
            SECTION("getUsesSSV") {
                vector<std::pair<int, string> > expected = {pair(4, "p")};
                REQUIRE(pkb.getUsesSSV(4, "p") == expected);
            }
            SECTION("getUsesSSV returns empty list if no result") {
                vector<std::pair<int, string> > expectedEmpty = {};
                REQUIRE(pkb.getUsesSSV(11, "") == expectedEmpty);
            }
            SECTION("getUsesSTT") {
                vector<std::pair<int, string> > expected
                        = {pair(2, "p"),pair(4, "p"), pair(4, "a"), pair(4, "ifs"), pair(4, "w"), pair(7, "w"),pair(8, "ifs")};
                REQUIRE(pkb.getUsesSTT(ASSIGNMENT, ASSIGNMENT) == expected);
            }
            SECTION("getUsesSTT where second arg is NO_TYPE returns result") {
                vector<std::pair<int, string> > expected = {pair(3,"c"),pair(3,"k"),
                pair(3,"p"), pair(3,"a"), pair(3,"ifs"),pair(3,"w"),pair(3,"pattern")};

                vector<std::pair<int, string> > getUsesSTT = pkb.getUsesSTT(IF, NO_TYPE);
                for (int i = 0; i < getUsesSTT.size(); i++) {
                    pair<int, string> innerPair = getUsesSTT[i];
                    bool isPresent = std::find(getUsesSTT.begin(), getUsesSTT.end(), innerPair)
                                     != getUsesSTT.end();
                    REQUIRE(isPresent);
                }
                REQUIRE(getUsesSTT.size() == expected.size());
            }
            SECTION("getUsesSTT returns empty list if no result") {
                vector<std::pair<int, string> > expectedEmpty = {};
                REQUIRE(pkb.getUsesSTT(IF, READ) == expectedEmpty);
            }
            SECTION("getUsesSTT where first arg is NO_TYPE should return result?") {
                vector<std::pair<int, string> > expected = {pair(6,"c"),pair(10, "pattern")};
                REQUIRE(pkb.getUsesSTT(NO_TYPE, PRINT) == expected);
            }
            SECTION("getUsesSST") {
                vector<std::pair<int, string> > expected = {make_pair(7, "w")};
                REQUIRE(pkb.getUsesSST(7, NO_TYPE) == expected);
            }
            SECTION("getUsesSST returns empty list if no result") {
                vector<std::pair<int, string> > expectedEmpty = {};
                REQUIRE(pkb.getUsesSST(9, NO_TYPE) == expectedEmpty);
            }
            SECTION("getUsesSTV") {
                vector<std::pair<int, string> > expected = {pair(4, "w"), pair(7, "w")};
                REQUIRE(pkb.getUsesSTV(ASSIGNMENT, "w") == expected);
            }
            SECTION("getUsesSTV with first arg as NO_TYPE") {
                vector<std::pair<int, string> > expected = {pair(3,"w"),pair(4,"w"),pair(5,"w"),pair(7,"w")};
                vector<std::pair<int, string> > getUsesSTV = pkb.getUsesSTV(NO_TYPE, "w");
                REQUIRE(getUsesSTV.size() == expected.size());
                for (int i = 0; i < getUsesSTV.size(); i++) {
                    pair<int, string> innerPair = getUsesSTV[i];
                    bool isPresent = std::find(getUsesSTV.begin(), getUsesSTV.end(), innerPair)
                                     != getUsesSTV.end();
                    REQUIRE(isPresent);
                }
            }
        }
        SECTION("getUsesP") {
            SECTION("getUsesPPV") {
                vector<std::pair<string, string> > expected = {pair("simple", "p")};
                REQUIRE(pkb.getUsesPPV("simple", "p") == expected);
            }
            SECTION("getUsesPPV returns empty list if no result") {
                vector<std::pair<string, string> > expectedEmpty = {};
                REQUIRE(pkb.getUsesPPV("simple", "pat") == expectedEmpty);
            }
            SECTION("getUsesPTT(NO_TYPE, NOTYPE) returns all Uses pairs") {
                vector<std::pair<string, string> > expected = {
                        pair("simple","p"),
                        pair("simple","c"),
                        pair("simple","k"),
                        pair("simple","a"),
                        pair("simple","ifs"),
                        pair("simple","w"),
                        pair("simple","pattern")};
                vector<std::pair<string, string> > result = pkb.getUsesPTT(NO_TYPE,NO_TYPE);
                REQUIRE(result.size() == expected.size());
                for (int i = 0; i < result.size(); i++) {
                    pair<string, string> pair = result[i];
                    bool isPresent = std::find(result.begin(), result.end(), pair)
                                     != result.end();
                    REQUIRE(isPresent);
                }
            }
            SECTION("getUsesPTT where first arg isnt NO_TYPE is considered invalid, returns empty set") {
                vector<std::pair<string, string> > expectedEmpty = {};
                REQUIRE(pkb.getUsesPTT(READ, PRINT) == expectedEmpty);
            }
            SECTION("getUsesPTT where first arg is NO_TYPE should return empty set") {
                vector<std::pair<string, string> > expectedEmpty = {};
                REQUIRE(pkb.getUsesPTT(NO_TYPE, PRINT) == expectedEmpty);
            }
            SECTION("getUsesPPT") {
                vector<std::pair<string, string> > expected
                        = {pair("simple", "p"), pair("simple", "c"), pair("simple", "k"), pair("simple", "a"),
                           pair("simple", "ifs"),pair("simple", "w"), pair("simple", "pattern")};
                REQUIRE(pkb.getUsesPPT("simple", NO_TYPE).size() == expected.size());
            }
            SECTION("getUsesPPT returns empty list if no result") {
                vector<std::pair<string, string> > expectedEmpty = {};
                REQUIRE(pkb.getUsesPPT("simple", WHILE) == expectedEmpty);
            }
            SECTION("getUsesPTV") {
                vector<std::pair<string, string> > expected = {pair("simple", "a")};
                REQUIRE(pkb.getUsesPTV(NO_TYPE, "a") == expected);
            }
            SECTION("getUsesPTV where first arg isnt NO_TYPE is considered invalid, returns empty set") {
                vector<std::pair<string, string> > expectedEmpty = {};
                REQUIRE(pkb.getUsesPTV(ASSIGNMENT, "w") == expectedEmpty);
            }
        }
    }
    SECTION("Get Modifies") {
        SECTION("getModifiesS") {
            SECTION("getModifiesSSV") {
                vector<std::pair<int, string> > expected = {pair(4, "pattern")};
                REQUIRE(pkb.getModifiesSSV(4, "pattern") == expected);
            }
            SECTION("getModifiesSSV returns empty list if no result") {
                vector<std::pair<int, string> > expectedEmpty = {};
                REQUIRE(pkb.getModifiesSSV(-1, "") == expectedEmpty);
            }
            SECTION("getModifiesSTT") {
                vector<std::pair<int, string> > expected
                        = {pair(1, "p"),pair(2, "a"), pair(4, "pattern"), pair(7, "w"), pair(8, "ifs")};
                REQUIRE(pkb.getModifiesSTT(ASSIGNMENT, ASSIGNMENT) == expected);
            }
            SECTION("getModifiesSTT where second arg is NO_TYPE returns result") {
                vector<std::pair<int, string> > expected = {pair(9,"p")};
                REQUIRE(pkb.getModifiesSTT(READ, NO_TYPE) == expected);
            }
            SECTION("getModifiesSTT returns empty list if no result") {
                vector<std::pair<int, string> > expectedEmpty = {};
                REQUIRE(pkb.getModifiesSTT(ASSIGNMENT, READ) == expectedEmpty);
            }
            SECTION("getModifiesSTT where first arg is NO_TYPE should return empty set") {
                vector<std::pair<int, string> > expectedEmpty = {};
                REQUIRE(pkb.getModifiesSTT(NO_TYPE, PRINT) == expectedEmpty);
            }
            SECTION("getModifiesSST") {
                vector<std::pair<int, string> > expected = {make_pair(7, "w")};
                REQUIRE(pkb.getModifiesSST(7, NO_TYPE) == expected);
            }
            SECTION("getModifiesSST returns empty list if no result") {
                vector<std::pair<int, string> > expectedEmpty = {};
                REQUIRE(pkb.getModifiesSST(6, NO_TYPE) == expectedEmpty);
            }
            SECTION("getModifiesSTV") {
                vector<std::pair<int, string> > expected = {pair(2, "a")};
                REQUIRE(pkb.getModifiesSTV(ASSIGNMENT, "a") == expected);
            }
            SECTION("getModifiesSTV with first arg as NO_TYPE") {
                vector<std::pair<int, string> > expected = {pair(4,"w"), pair(6,"w"),pair(8,"w")};
                vector<std::pair<int, string> > getModifiesSTV = pkb.getModifiesSTV(NO_TYPE, "w");
                REQUIRE(getModifiesSTV.size() == expected.size());
                for (int i = 0; i < getModifiesSTV.size(); i++) {
                    pair<int, string> innerPair = getModifiesSTV[i];
                    bool isPresent = std::find(getModifiesSTV.begin(), getModifiesSTV.end(), innerPair)
                                     != getModifiesSTV.end();
                    REQUIRE(isPresent);
                }
            }
            SECTION("getModifies read") {
                vector<std::pair<int, string> > expected = {pair{9,"p"}};
                REQUIRE(pkb.getModifiesSTT(READ, NO_TYPE) == expected);
            }
        }
        SECTION("getModifiesP") {
            SECTION("getModifiesPPV") {
                vector<std::pair<string, string> > expected = {pair("simple", "p")};
                REQUIRE(pkb.getModifiesPPV("simple", "p") == expected);
            }
            SECTION("getModifiesPPV returns empty list if no result") {
                vector<std::pair<string, string> > expectedEmpty = {};
                REQUIRE(pkb.getModifiesPPV("simpl", "pattern") == expectedEmpty);
            }
            SECTION("getUsesPTT(NO_TYPE, NOTYPE) returns all Uses pairs") {
                vector<std::pair<string, string> > expected = {
                        pair("simple","a"),
                        pair("simple","pattern"),
                        pair("simple","w"),
                        pair("simple","ifs"),
                        pair("simple","p")};
                vector<std::pair<string, string> > result = pkb.getModifiesPTT(NO_TYPE,NO_TYPE);
                REQUIRE(result.size() == expected.size());
                for (int i = 0; i < result.size(); i++) {
                    pair<string, string> pair = result[i];
                    bool isPresent = std::find(result.begin(), result.end(), pair)
                                     != result.end();
                    REQUIRE(isPresent);
                }
            }
            SECTION("getModifiesPTT where first arg isnt NO_TYPE is considered invalid, returns empty set") {
                vector<std::pair<string, string> > expectedEmpty = {};
                REQUIRE(pkb.getModifiesPTT(READ, READ) == expectedEmpty);
            }
            SECTION("getModifiesPPT") {
                vector<std::pair<string, string> > expected = {pair("simple", "pattern"), pair("simple", "w"), pair("simple", "ifs")};
                REQUIRE(pkb.getModifiesPPT("simple", IF) == expected);
            }
            SECTION("getModifiesPTV") {
                vector<std::pair<string, string> > expected = {pair("simple", "w")};
                REQUIRE(pkb.getModifiesPTV(NO_TYPE, "w") == expected);
            }
            SECTION("getModifiesPTV where first arg isnt NO_TYPE is considered invalid, returns empty set") {
                vector<std::pair<string, string> > expectedEmpty = {};
                REQUIRE(pkb.getModifiesPTV(ASSIGNMENT, "w") == expectedEmpty);
            }
        }
    }

    SECTION("Get Next") {

        SECTION("getNextNN") {
            vector<std::pair<int, int> > expected = {pair(1,2)};
            REQUIRE(pkb.getNextNN(1,2) == expected);
            vector<std::pair<int, int> > expected2 = {pair(2,3)};
            REQUIRE(pkb.getNextNN(2,3) == expected2);
            vector<std::pair<int, int> > expected3 = {pair(3,4)};
            REQUIRE(pkb.getNextNN(3,4) == expected3);
            vector<std::pair<int, int> > expected4 = {pair(4,5)};
            REQUIRE(pkb.getNextNN(4,5) == expected4);
            vector<std::pair<int, int> > expected5 = {pair(5,6)};
            REQUIRE(pkb.getNextNN(5,6) == expected5);
            vector<std::pair<int, int> > expected6 = {pair(6,7)};
            REQUIRE(pkb.getNextNN(6,7) == expected6);
            vector<std::pair<int, int> > expected7 = {pair(7,5)};
            REQUIRE(pkb.getNextNN(7,5) == expected7);
            vector<std::pair<int, int> > expected8 = {pair(5,8)};
            REQUIRE(pkb.getNextNN(5,8) == expected8);
            vector<std::pair<int, int> > expected9 = {pair(3,9)};
            REQUIRE(pkb.getNextNN(3,9) == expected9);
            vector<std::pair<int, int> > expected10 = {pair(9,10)};
            REQUIRE(pkb.getNextNN(9,10) == expected10);
        }
        SECTION("getNextTT") {
            vector<std::pair<int, int> > expected = {pair(1, 2)};
            REQUIRE(pkb.getNextTT(ASSIGNMENT, ASSIGNMENT) == expected);
        }
        SECTION("getNextTT(NO_TYPE, NOTYPE) returns all Next pairs") {
            vector<std::pair<int, int> > expected = {
                    pair(1,2),
                    pair(2,3),
                    pair(3,4),
                    pair(4,5),
                    pair(5,6),
                    pair(6,7),
                    pair(7,5),
                    pair(5,8),
                    pair(3,9),
                    pair(9,10)};
            vector<std::pair<int, int> > result = pkb.getNextTT(NO_TYPE,NO_TYPE);
            REQUIRE(result.size() == expected.size());
            for (int i = 0; i < result.size(); i++) {
                pair<int, int> pair = result[i];
                bool isPresent = std::find(result.begin(), result.end(), pair)
                                 != result.end();
                REQUIRE(isPresent);
            }
        }
        SECTION("getNextNT") {
            vector<std::pair<int, int> > expected = {pair(2,3)};
            REQUIRE(pkb.getNextNT(2,IF) == expected);
        }
        SECTION("getNextNT with no result returns empty set") {
            vector<std::pair<int, int> > expectedEmpty = {};
            REQUIRE(pkb.getNextNT(4,IF) == expectedEmpty);
        }
        SECTION("getNextTN") {
            vector<std::pair<int, int> > expected = {pair(5,8)};
            REQUIRE(pkb.getNextTN(WHILE,8) == expected);
        }
        SECTION("getNextTN with no result returns empty set") {
            vector<std::pair<int, int> > expectedEmpty = {};
            REQUIRE(pkb.getNextTN(PRINT,10) == expectedEmpty);
        }

    }
}


