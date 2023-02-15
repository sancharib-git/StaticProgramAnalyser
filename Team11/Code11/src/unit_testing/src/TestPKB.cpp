//
// Created by Yen Peng Yip on 7/9/21.
//

#include "PKB/PKB.h"

#include "catch.hpp"

PKB &pkb = pkb.getInstance();

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

TEST_CASE("Initialize") {
    pkb.clear();
}

TEST_CASE("Store Variable") {
    SECTION("addVar") {
        REQUIRE(pkb.addVar("p"));
        REQUIRE(pkb.addVar("a"));
        REQUIRE(pkb.addVar("w"));
        REQUIRE(pkb.addVar("pattern"));
        REQUIRE(pkb.addVar("ifs"));
        REQUIRE(pkb.addVar("c"));
        REQUIRE(pkb.addVar("k"));
    }

    SECTION("getTotalNumOfVars") {
        REQUIRE(pkb.getTotalNumOfVars() == 7);
    }
    SECTION("getTotalNumOfVars") {
        unordered_set<string> expected = {"p","a","w","pattern","ifs","c","k"};
        REQUIRE(*(pkb.getAllVars()) == expected);
    }
}

TEST_CASE("Store Constant") {
    SECTION("addConst") {
        REQUIRE(pkb.addConst(2));
        REQUIRE(pkb.addConst(0));
        REQUIRE(pkb.addConst(-1));
    }

    SECTION("getTotalNumOfConst") {
        REQUIRE(pkb.getTotalNumOfConsts() == 3);
    }
    SECTION("getAllConst") {
        unordered_set<int> expected = {2, 0, -1};
        REQUIRE(*(pkb.getAllConsts()) == expected);
    }
}

TEST_CASE("Store Assignment") {

    SECTION("addAssignPattern") {
        REQUIRE(pkb.addAssignPattern(1,"4"));
        REQUIRE(pkb.addAssignPattern(2,"p*2"));
        REQUIRE(pkb.addAssignPattern(4,"p*a*a+p*ifs+p*w"));
        REQUIRE(pkb.addAssignPattern(7,"w-1"));
        REQUIRE(pkb.addAssignPattern(8,"ifs-1"));
    }
    SECTION("getAssignPattern") {
        REQUIRE(pkb.getAssignPattern(1) == "4");
        REQUIRE(pkb.getAssignPattern(2) == "p*2");
        REQUIRE(pkb.getAssignPattern(4) == "p*a*a+p*ifs+p*w");
        REQUIRE(pkb.getAssignPattern(7) == "w-1");
        REQUIRE(pkb.getAssignPattern(8) == "ifs-1");
    }
    SECTION("addAssignSubExp") {
        REQUIRE(pkb.addAssignSubExp(1,"4"));
        REQUIRE(pkb.addAssignSubExp(2,"p*2"));
        REQUIRE(pkb.addAssignSubExp(2,"p"));
        REQUIRE(pkb.addAssignSubExp(2,"2"));
        REQUIRE(pkb.addAssignSubExp(4,"p*a*a+p*ifs+p*w"));
        REQUIRE(pkb.addAssignSubExp(4,"p*a"));
        REQUIRE(pkb.addAssignSubExp(4,"p*a*a"));
        REQUIRE(pkb.addAssignSubExp(4,"p*ifs"));
        REQUIRE(pkb.addAssignSubExp(4,"p*w"));
        REQUIRE(pkb.addAssignSubExp(4,"p*ifs+p*w"));
        REQUIRE(pkb.addAssignSubExp(4,"p"));
        REQUIRE(pkb.addAssignSubExp(4,"a"));
        REQUIRE(pkb.addAssignSubExp(4,"ifs"));
        REQUIRE(pkb.addAssignSubExp(4,"w"));
        REQUIRE(pkb.addAssignSubExp(7,"w-1"));
        REQUIRE(pkb.addAssignSubExp(7,"w"));
        REQUIRE(pkb.addAssignSubExp(7,"1"));
        REQUIRE(pkb.addAssignSubExp(8,"ifs-1"));
        REQUIRE(pkb.addAssignSubExp(8,"ifs"));
        REQUIRE(pkb.addAssignSubExp(8,"1"));
        SECTION("addAssignSubExp should return true if already added") {
            REQUIRE(pkb.addAssignSubExp(4,"a"));
        }
    }
    SECTION("getSubExpStmtNums") {
        unordered_set<int> expected1 = {2,4};
        unordered_set<int> expected2 = {4};
        unordered_set<int> expected3 = {7,8};
        unordered_set<int> expectedEmpty = {};
        REQUIRE(pkb.getSubExpStmtNums("p") == expected1);
        REQUIRE(pkb.getSubExpStmtNums("1") == expected3);
        REQUIRE(pkb.getSubExpStmtNums("p*ifs+p*w") == expected2);
        REQUIRE(pkb.getSubExpStmtNums("a") == expected2);
        SECTION("getSubExpStmtNums returns empty set if no result") {
            REQUIRE(pkb.getSubExpStmtNums("t") == expectedEmpty);
            REQUIRE(pkb.getSubExpStmtNums("p*ifs+p") == expectedEmpty);
            REQUIRE(pkb.getSubExpStmtNums("p*if") == expectedEmpty);
            REQUIRE(pkb.getSubExpStmtNums("ifs-") == expectedEmpty);
        }
    }

    SECTION("addAssignLHSVar") {
        REQUIRE(pkb.addAssignLHSVar(1, "p"));
        REQUIRE(pkb.addAssignLHSVar(2, "a"));
        REQUIRE(pkb.addAssignLHSVar(4, "pattern"));
        REQUIRE(pkb.addAssignLHSVar(7, "w"));
        REQUIRE(pkb.addAssignLHSVar(8, "ifs"));
    }

    SECTION("addAssignRHSVar") {
        REQUIRE(pkb.addAssignRHSVar(2, "p"));
        REQUIRE(pkb.addAssignRHSVar(4, "p"));
        REQUIRE(pkb.addAssignRHSVar(4, "a"));
        REQUIRE(pkb.addAssignRHSVar(4, "ifs"));
        REQUIRE(pkb.addAssignRHSVar(4, "w"));
        REQUIRE(pkb.addAssignRHSVar(7, "w"));
        REQUIRE(pkb.addAssignRHSVar(8, "ifs"));
    }

    SECTION("addAssignRHSConst") {
        REQUIRE(pkb.addAssignRHSConst(1, 4));
        REQUIRE(pkb.addAssignRHSConst(2, 2));
        REQUIRE(pkb.addAssignRHSConst(7, 1));
        REQUIRE(pkb.addAssignRHSConst(8, 1));
    }

    SECTION("addAssignConst") {
        std::unordered_set<int> expected1 = {4};
        std::unordered_set<int> expected2 = {2};
        std::unordered_set<int> expected7 = {1};
        std::unordered_set<int> expected8 = {1};
        REQUIRE(pkb.getAssignRHSConst(8) == expected8);
        REQUIRE(pkb.getAssignRHSConst(7) == expected7);
        REQUIRE(pkb.getAssignRHSConst(2) == expected2);
        REQUIRE(pkb.getAssignRHSConst(1) == expected1);
    }
    SECTION("addIfStmtCV") {
        REQUIRE(pkb.addIfStmtCV(3, "k"));
        REQUIRE(pkb.addIfStmtCV(3, "c"));
    }
    SECTION("addWhileStmtCV") {
        REQUIRE(pkb.addWhileStmtCV(5, "w"));
    }
    SECTION("getIfStmtsWithCV") {
        unordered_set<int> expected = {3};
        unordered_set<int> expectedEmpty = {};
        REQUIRE(pkb.getIfStmtsWithCV("k") == expected);
        REQUIRE(pkb.getIfStmtsWithCV("c") == expected);
        REQUIRE(pkb.getIfStmtsWithCV("") == expectedEmpty);
    }
    SECTION("getWhileStmtsWithCV") {
        unordered_set<int> expected = {5};
        unordered_set<int> expectedEmpty = {};
        REQUIRE(pkb.getWhileStmtsWithCV("w") == expected);
        REQUIRE(pkb.getWhileStmtsWithCV("k") == expectedEmpty);
    }
}

TEST_CASE("Store Read") {
    SECTION("addReadStmt") {
        REQUIRE(pkb.addReadStmt(9, "p"));
    }

    SECTION("getReadStmtList") {
        unordered_set<int> expected = {9};
        REQUIRE(*(pkb.getReadStmtList()) == expected);
    }
}

TEST_CASE("Store Print") {
    SECTION("addPrintStmt") {
        REQUIRE(pkb.addPrintStmt(10, "pattern"));
    }

    SECTION("getPrintStmtList") {
        unordered_set<int> expected = {10};
        REQUIRE(*(pkb.getPrintStmtList()) == expected);
    }
}

TEST_CASE("Store If") {
    SECTION("addIfStmt") {
        REQUIRE(pkb.addIfStmt(3));
        REQUIRE(pkb.addIfStmt(8));
    }

    SECTION("getIfList") {
        unordered_set<int> expected = {3,8};
        REQUIRE(*(pkb.getIfList()) == expected);
    }

    SECTION("addIfBlockStmt") {
        REQUIRE(pkb.addIfBlockStmt(3, 4));
        REQUIRE(pkb.addIfBlockStmt(3, 5));
        REQUIRE(pkb.addIfBlockStmt(3, 6));
        REQUIRE(pkb.addIfBlockStmt(3, 7));
        REQUIRE(pkb.addIfBlockStmt(3, 8));
    }
    SECTION("getIfBlockList") {
        std::vector<int> expected = {4,5,6,7,8};
        REQUIRE(pkb.getIfBlockList(3) == expected);
    }
    SECTION("getIfBlockList returns empty vector if no result") {
        std::vector<int> expectedEmpty = {};
        REQUIRE(pkb.getIfBlockList(4) == expectedEmpty);
    }
    SECTION("addElseBlockStmt") {
        REQUIRE(pkb.addElseBlockStmt(3, 9));
        REQUIRE(pkb.addElseBlockStmt(3, 10));
    }
    SECTION("getElseBlockList") {
        std::vector<int> expected = {9,10};
        REQUIRE(pkb.getElseBlockList(3) == expected);
    }
    SECTION("getElseBlockList returns empty vector if no result") {
        std::vector<int> expectedEmpty = {};
        REQUIRE(pkb.getElseBlockList(9) == expectedEmpty);
    }
}

TEST_CASE("Store While") {
    SECTION("addWhileStmt") {
        REQUIRE(pkb.addWhileStmt(5));
    }

    SECTION("getWhileList") {
        unordered_set<int> expected = {5};
        REQUIRE(*(pkb.getWhileList()) == expected);
    }

    SECTION("addWhileBlockStmt") {
        REQUIRE(pkb.addWhileBlockStmt(5, 6));
        REQUIRE(pkb.addWhileBlockStmt(5, 7));
    }
    SECTION("getWhileBlockList") {
        std::vector<int> expected = {6,7};
        REQUIRE(pkb.getWhileBlockList(5) == expected);
    }
    SECTION("getWhileBlockList returns empty vector if no result") {
        std::vector<int> expectedEmpty = {};
        REQUIRE(pkb.getWhileBlockList(6) == expectedEmpty);
    }
}
TEST_CASE("Store Procedure") {
    SECTION("addProcedure") {
        REQUIRE(pkb.addProcedure("simple"));
    }
    SECTION("getProcDigit") {
        REQUIRE(pkb.getProcDigit("simple") == 1);
    }
    SECTION("getProcDigit return -1 if no result") {
        REQUIRE(pkb.getProcDigit("noexist") == -1);
    }
    SECTION("getProcName") {
        REQUIRE(pkb.getProcName(1) == "simple");
    }
    SECTION("getProcName return empty string if no result") {
        REQUIRE(pkb.getProcName(2) == "");
    }
    SECTION("getProcList") {
        unordered_set<std::string> expected = {"simple"};
        REQUIRE(*(pkb.getProcList()) == expected);
    }
}

TEST_CASE("Store Follows") {
    SECTION("addFollows") {
        REQUIRE(pkb.addFollows(1, ASSIGNMENT, 2, ASSIGNMENT));
        REQUIRE(pkb.addFollows(2, ASSIGNMENT, 3, IF));
        REQUIRE(pkb.addFollows(4, ASSIGNMENT, 5, WHILE));
        REQUIRE(pkb.addFollows(6, CALL, 7, ASSIGNMENT));
        REQUIRE(pkb.addFollows(9, READ, 10, PRINT));
    }
    SECTION("getFollows") {
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
            vector<std::pair<int, int> > table = pkb.getFollowsTN(CALL, 7);

            REQUIRE(pkb.getFollowsTN(CALL, 7) == expectedFollowsTN);
        }
        SECTION("getFollowsTN returns empty list if no result") {
            vector<std::pair<int, int> > expectedEmpty = {};
            REQUIRE(pkb.getFollowsTN(WHILE, 10) == expectedEmpty);
        }
    }
}

TEST_CASE("Store FollowStar") {
    SECTION("addFollowsStar") {
        REQUIRE(pkb.addFollowsStar(1, ASSIGNMENT, 2, ASSIGNMENT));
        REQUIRE(pkb.addFollowsStar(1, ASSIGNMENT, 3, IF));
        REQUIRE(pkb.addFollowsStar(2, ASSIGNMENT, 3, IF));
        REQUIRE(pkb.addFollowsStar(4, ASSIGNMENT, 5, WHILE));
        REQUIRE(pkb.addFollowsStar(4, ASSIGNMENT, 8, ASSIGNMENT));
        REQUIRE(pkb.addFollowsStar(6, CALL, 7, ASSIGNMENT));
        REQUIRE(pkb.addFollowsStar(9, READ, 10, PRINT));

    }
    SECTION("getFollowsStar") {
        SECTION("getFollowsStarNN") {
            vector<std::pair<int, int> > expectedFollowsStarNN
                    = {make_pair(4, 8)};
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

TEST_CASE("Store Parent") {
    SECTION("addParent valid if first is container stmt and second is nested stmt") {
        REQUIRE(pkb.addParent(3, IF, 4, ASSIGNMENT));
        REQUIRE(pkb.addParent(3, IF, 5, WHILE));
        REQUIRE(pkb.addParent(3, IF, 8, ASSIGNMENT));
        REQUIRE(pkb.addParent(3, IF, 9, READ));
        REQUIRE(pkb.addParent(3, IF, 10, PRINT));
        REQUIRE(pkb.addParent(5, WHILE, 6, CALL));
        REQUIRE(pkb.addParent(5, WHILE, 7, ASSIGNMENT));
    }
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

TEST_CASE("Store ParentStar") {
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
            vector<std::pair<int, int> > expected = {make_pair(3, 4), make_pair(3, 7), make_pair(3, 8)};
            REQUIRE(pkb.getParentStarTT(IF, ASSIGNMENT) == expected);
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
            REQUIRE(pkb.getParentStarTT(NO_TYPE, ASSIGNMENT) == expected);
        }
        SECTION("getParentStarTT returns empty with NO_TYPE as arg") {
            vector<std::pair<int, int> > expectedEmpty = {};
            REQUIRE(pkb.getParentStarTT(ASSIGNMENT, NO_TYPE) == expectedEmpty);
        }
    }
}

TEST_CASE("Store Uses") {
    SECTION("addUsesS") {
        REQUIRE(pkb.addUsesS(2, ASSIGNMENT, "p"));
        REQUIRE(pkb.addUsesS(3, IF, "c"));
        REQUIRE(pkb.addUsesS(3, IF, "k"));
        REQUIRE(pkb.addUsesS(4, ASSIGNMENT, "p"));
        REQUIRE(pkb.addUsesS(4, ASSIGNMENT, "a"));
        REQUIRE(pkb.addUsesS(4, ASSIGNMENT, "ifs"));
        REQUIRE(pkb.addUsesS(4, ASSIGNMENT, "p"));
        REQUIRE(pkb.addUsesS(4, ASSIGNMENT, "w"));
        REQUIRE(pkb.addUsesS(5, WHILE, "w"));
        REQUIRE(pkb.addUsesS(7, ASSIGNMENT, "w"));
        REQUIRE(pkb.addUsesS(5, WHILE, "ifs"));
        REQUIRE(pkb.addUsesS(9, PRINT, "pattern"));
    }
    SECTION("addUsesP") {
        REQUIRE(pkb.addUsesP("simple", ASSIGNMENT, "p"));
        REQUIRE(pkb.addUsesP("simple", IF, "c"));
        REQUIRE(pkb.addUsesP("simple", IF, "k"));
        REQUIRE(pkb.addUsesP("simple", ASSIGNMENT, "a"));
        REQUIRE(pkb.addUsesP("simple", ASSIGNMENT, "ifs"));
        REQUIRE(pkb.addUsesP("simple", ASSIGNMENT, "w"));
        REQUIRE(pkb.addUsesP("simple", PRINT, "pattern"));
    }
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
            = {pair(2, "p"),pair(4, "p"), pair(4, "a"), pair(4, "ifs"), pair(4, "w"), pair(7, "w")};
            REQUIRE(pkb.getUsesSTT(ASSIGNMENT, ASSIGNMENT) == expected);
        }
        SECTION("getUsesSTT where second arg is NO_TYPE returns result") {
            vector<std::pair<int, string> > expected = {pair(3,"c"), pair(3,"k")};
            REQUIRE(pkb.getUsesSTT(IF, NO_TYPE) == expected);
        }
        SECTION("getUsesSTT returns empty list if no result") {
            vector<std::pair<int, string> > expectedEmpty = {};
            REQUIRE(pkb.getUsesSTT(IF, READ) == expectedEmpty);
        }
        SECTION("getUsesSST") {
            vector<std::pair<int, string> > expected = {make_pair(7, "w")};
            REQUIRE(pkb.getUsesSST(7, NO_TYPE) == expected);
        }
        SECTION("getUsesSST returns empty list if no result") {
            vector<std::pair<int, string> > expectedEmpty = {};
            REQUIRE(pkb.getUsesSST(6, NO_TYPE) == expectedEmpty);
        }
        SECTION("getUsesSTV") {
            vector<std::pair<int, string> > expected = {pair(4, "w"), pair(7, "w")};
            REQUIRE(pkb.getUsesSTV(ASSIGNMENT, "w") == expected);
        }
        SECTION("getUsesSTV returns empty list if no result") {
            vector<std::pair<int, string> > expectedEmpty = {};
            REQUIRE(pkb.getUsesSTV(NO_TYPE, "d") == expectedEmpty);
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
        SECTION("getUsesPTT") {
            vector<std::pair<string, string> > expected
                    = {pair("simple", "c"),pair("simple","k")};
            REQUIRE(pkb.getUsesPTT(IF, IF) == expected);
        }
        SECTION("getUsesPTT where second arg is NO_TYPE returns result") {
            vector<std::pair<string, string> > expected = {pair("simple", "c"),pair("simple","k")};
            REQUIRE(pkb.getUsesPTT(IF, NO_TYPE) == expected);
        }
        SECTION("getUsesPTT returns empty list if no result") {
            vector<std::pair<string, string> > expectedEmpty = {};
            REQUIRE(pkb.getUsesPTT(READ, PRINT) == expectedEmpty);
        }
        SECTION("getUsesPTT where first arg is NO_TYPE") {
            vector<std::pair<string, string> > expected = {pair("simple","pattern")};
            REQUIRE(pkb.getUsesPTT(NO_TYPE, PRINT) == expected);
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
            vector<std::pair<string, string> > expected = {pair("simple", "w")};
            REQUIRE(pkb.getUsesPTV(ASSIGNMENT, "w") == expected);
        }
        SECTION("getUsesPTV with first arg as NO_TYPE returns empty set") {
            vector<std::pair<string, string> > expectedEmpty = {};
            REQUIRE(pkb.getUsesPTV(NO_TYPE, "d") == expectedEmpty);
        }
    }
}

TEST_CASE("Store Modifies") {
    SECTION("addModifiesS") {
        REQUIRE(pkb.addModifiesS(1, ASSIGNMENT, "p"));
        REQUIRE(pkb.addModifiesS(2, ASSIGNMENT, "a"));
        REQUIRE(pkb.addModifiesS(4, ASSIGNMENT, "pattern"));
        REQUIRE(pkb.addModifiesS(7, ASSIGNMENT, "w"));
        REQUIRE(pkb.addModifiesS(8, ASSIGNMENT, "ifs"));
        REQUIRE(pkb.addModifiesS(9, READ, "p"));
    }
    SECTION("addModifiesP") {
        REQUIRE(pkb.addModifiesP("simple", ASSIGNMENT, "p"));
        REQUIRE(pkb.addModifiesP("simple", ASSIGNMENT, "a"));
        REQUIRE(pkb.addModifiesP("simple", ASSIGNMENT, "pattern"));
        REQUIRE(pkb.addModifiesP("simple", ASSIGNMENT, "w"));
        REQUIRE(pkb.addModifiesP("simple", ASSIGNMENT, "ifs"));
        REQUIRE(pkb.addModifiesP("simple", READ, "p"));
    }
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
            REQUIRE(pkb.getModifiesSST(5, NO_TYPE) == expectedEmpty);
        }
        SECTION("getModifiesSTV") {
            vector<std::pair<int, string> > expected = {pair(2, "a")};
            REQUIRE(pkb.getModifiesSTV(ASSIGNMENT, "a") == expected);
        }
        SECTION("getModifiesSTV with first arg as NO_TYPE") {
            vector<std::pair<int, string> > expected = {pair(7,"w")};
            REQUIRE(pkb.getModifiesSTV(NO_TYPE, "w") == expected);
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
        SECTION("getModifiesPTT") {
            vector<std::pair<string, string> > expected
                    = {pair("simple", "p"),pair("simple","a"), pair("simple", "pattern"),pair("simple","w"),pair("simple", "ifs")};
            REQUIRE(pkb.getModifiesPTT(ASSIGNMENT, ASSIGNMENT) == expected);
        }
        SECTION("getModifiesPTT where second arg is NO_TYPE returns result") {
            vector<std::pair<string, string> > expected
            = {pair("simple", "p")};
            REQUIRE(pkb.getModifiesPTT(READ, NO_TYPE) == expected);
        }
        SECTION("getModifiesPTT where second arg is NO_TYPE returns result") {
            vector<std::pair<string, string> > expected
                    = {pair("simple", "p")};
            REQUIRE(pkb.getModifiesPTT(READ, NO_TYPE) == expected);
        }
        SECTION("getModifiesPTT returns empty list if no result") {
            vector<std::pair<string, string> > expectedEmpty = {};
            REQUIRE(pkb.getModifiesPTT(READ, PRINT) == expectedEmpty);
        }
        SECTION("getModifiesPTT where first arg is NO_TYPE should return empty set") {
            vector<std::pair<string, string> > expectedEmpty = {};
            REQUIRE(pkb.getModifiesPTT(NO_TYPE, PRINT) == expectedEmpty);
        }
        SECTION("getModifiesPPT") {
            vector<std::pair<string, string> > expected
                    = {pair("simple", "p"), pair("simple", "a"), pair("simple", "pattern"), pair("simple", "w"),pair("simple", "ifs")};
            REQUIRE(pkb.getModifiesPPT("simple", NO_TYPE).size() == expected.size());
        }
        SECTION("getModifiesPPT returns empty list if no result") {
            vector<std::pair<string, string> > expectedEmpty = {};
            REQUIRE(pkb.getModifiesPPT("simple", IF) == expectedEmpty);
        }
        SECTION("getModifiesPTV") {
            vector<std::pair<string, string> > expected = {pair("simple", "w")};
            REQUIRE(pkb.getModifiesPTV(ASSIGNMENT, "w") == expected);
        }
        SECTION("getModifiesPTV with first arg as NO_TYPE") {
            vector<std::pair<string, string> > expected = {pair("simple","w")};
            REQUIRE(pkb.getModifiesPTV(NO_TYPE, "w") == expected);
        }
    }
}

TEST_CASE("Store Calls") {
    SECTION("addCalls") {
        REQUIRE(pkb.addCalls("simple", CALL, "ast", 6));
    }
    SECTION("getCallsSSP") {
        vector<std::pair<int, string> > expected = {pair(6, "ast")};
        REQUIRE(pkb.getCallsSSP(6, "ast") == expected);
    }
    SECTION("getCallsSST") {
        vector<std::pair<int, string> > expected = {pair(6, "ast")};
        REQUIRE(pkb.getCallsSST(6, NO_TYPE) == expected);
    }
    SECTION("getCallsSTT") {
        vector<std::pair<int, string> > expected = {pair(6, "ast")};
        REQUIRE(pkb.getCallsSTT(NO_TYPE, NO_TYPE) == expected);
    }
    SECTION("getCallsSTP") {
        vector<std::pair<int, string> > expected = {pair(6, "ast")};
        REQUIRE(pkb.getCallsSTP(NO_TYPE, "ast") == expected);
    }
    SECTION("getCallsPPP") {
        vector<std::pair<string, string> > expected = {pair("simple", "ast")};
        REQUIRE(pkb.getCallsPPP("simple", "ast") == expected);
    }
    SECTION("getCallsPPT") {
        vector<std::pair<string, string> > expected = {pair("simple", "ast")};
        REQUIRE(pkb.getCallsPPT("simple", NO_TYPE) == expected);
    }
    SECTION("getCallsPTP") {
        vector<std::pair<string, string> > expected = {pair("simple", "ast")};
        REQUIRE(pkb.getCallsPTP(NO_TYPE, "ast") == expected);
    }
    SECTION("getCallsPTT") {
        vector<std::pair<string, string> > expected = {pair("simple", "ast")};
        REQUIRE(pkb.getCallsPTT(NO_TYPE, NO_TYPE) == expected);
    }
    SECTION("getCallStmtList") {
        std::unordered_set<int> expected = {6};
        REQUIRE(*(pkb.getCallStmtList()) == expected);
    }
}

TEST_CASE("Reset") {
    pkb.clear();
}

