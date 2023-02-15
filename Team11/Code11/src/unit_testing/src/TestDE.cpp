//
// Created by Toh Yu Ting on 8 Sept 2021
//

#include "DE/DesignExtractor.h"
#include "ParserException.h"
#include "PKB/PKB.h"
#include <algorithm>
#include "catch.hpp"

PKB& pkbDE = pkbDE.getInstance();

TEST_CASE("Test Invalid Procedure") {
    /* Unit test case used
        procedure first {
            a = x + y; //1
            read b; // 2
            print c; // 3
            while (x < y) { //4
                print k; //5
                print done; //6
                call second //7
            }

            s = (123 + c) + (xyz + z); //8
        }

        procedure second {
            t = g * h; //9
            print w; //10
            print x; //11
            if (y < e) { //12
                big = tough + tough; //13
                heavy = light + heavy; //14
            } else {
                feather = bird + wings; //15
                while (d < m) { //16
                    call third; //17
                }
            }
            read f; //18
        }

        procedure third {
            read module; // 19
            print assignments; //20
        }

        */
    pkbDE.clear();
    DesignExtractor* de = new DesignExtractor();

    pkbDE.addProcedure("first");
    pkbDE.addProcedure("second");
    pkbDE.addProcedure("third");
    pkbDE.addStmt(1, ASSIGNMENT, "first");
    pkbDE.addStmt(2, READ, "first");
    pkbDE.addStmt(3, PRINT, "first");
    pkbDE.addStmt(4, WHILE, "first");
    pkbDE.addWhileStmt(4);
    pkbDE.addWhileBlockStmt(4, 5);
    pkbDE.addWhileBlockStmt(4, 6);
    pkbDE.addStmt(5, PRINT, "first");
    pkbDE.addStmt(6, PRINT, "first");
    pkbDE.addStmt(7, CALL, "first");
    pkbDE.addParent(4, WHILE, 5, PRINT);
    pkbDE.addParentStar(4, WHILE, 5, PRINT);
    pkbDE.addParent(4, WHILE, 6, PRINT);
    pkbDE.addParentStar(4, WHILE, 6, PRINT);
    pkbDE.addParent(4, WHILE, 7, CALL);
    pkbDE.addParentStar(4, WHILE, 7, CALL);
    pkbDE.addStmt(8, ASSIGNMENT, "first");
    pkbDE.addStmt(9, ASSIGNMENT, "second");
    pkbDE.addStmt(10, PRINT, "second");
    pkbDE.addStmt(11, PRINT, "second");
    pkbDE.addStmt(12, IF, "second");
    pkbDE.addIfStmt(12);
    pkbDE.addIfBlockStmt(12, 13);
    pkbDE.addIfBlockStmt(12, 14);
    pkbDE.addElseBlockStmt(12, 15);
    pkbDE.addElseBlockStmt(12, 16);
    pkbDE.addElseBlockStmt(12, 17);
    pkbDE.addParent(12, IF, 13, ASSIGNMENT);
    pkbDE.addParentStar(12, IF, 13, ASSIGNMENT);
    pkbDE.addParent(12, IF, 14, ASSIGNMENT);
    pkbDE.addParentStar(12, IF, 14, ASSIGNMENT);
    pkbDE.addParent(12, IF, 15, ASSIGNMENT);
    pkbDE.addParentStar(12, IF, 15, ASSIGNMENT);
    pkbDE.addParent(12, IF, 16, WHILE);
    pkbDE.addParentStar(12, IF, 16, WHILE);
    pkbDE.addParentStar(12, IF, 17, WHILE);
    pkbDE.addStmt(13, ASSIGNMENT, "second");
    pkbDE.addStmt(14, ASSIGNMENT, "second");
    pkbDE.addStmt(15, ASSIGNMENT, "second");
    pkbDE.addStmt(16, WHILE, "second");
    pkbDE.addStmt(17, CALL, "second");
    pkbDE.addWhileStmt(16);
    pkbDE.addWhileBlockStmt(16, 17);
    pkbDE.addParent(16, WHILE, 17, CALL);
    pkbDE.addParentStar(16, WHILE, 17, CALL);
    pkbDE.addStmt(18, READ, "second");

    pkbDE.addStmt(19, READ, "third");
    pkbDE.addStmt(20, PRINT, "third");
    pkbDE.addStmt(21, CALL, "third");

    pkbDE.addCalls("first", CALL, "second", 7);
    pkbDE.addCalls("second", CALL, "third", 17);
    //invalid procedure
    pkbDE.addCalls("third", CALL, "fourth", 21);

    // procedure first
    pkbDE.addUsesS(1, ASSIGNMENT, "x");
    pkbDE.addUsesS(1, ASSIGNMENT, "y");
    pkbDE.addUsesS(3, PRINT, "c");
    pkbDE.addUsesS(4, WHILE, "x");
    pkbDE.addUsesS(4, WHILE, "y");
    pkbDE.addUsesS(4, WHILE, "k");
    pkbDE.addUsesS(4, WHILE, "done");
    pkbDE.addUsesS(5, PRINT, "k");
    pkbDE.addUsesS(6, PRINT, "done");
    pkbDE.addUsesS(8, ASSIGNMENT, "c");
    pkbDE.addUsesS(8, ASSIGNMENT, "xyz");
    pkbDE.addUsesS(8, ASSIGNMENT, "z");

    pkbDE.addModifiesS(1, ASSIGNMENT, "a");
    pkbDE.addModifiesS(2, READ, "b");
    pkbDE.addModifiesS(8, ASSIGNMENT, "s");
    pkbDE.addModifiesS(12, IF, "big");
    pkbDE.addModifiesS(12, IF, "heavy");
    pkbDE.addModifiesS(12, IF, "feather");

    // procedure second
    pkbDE.addUsesS(9, ASSIGNMENT, "g");
    pkbDE.addUsesS(9, ASSIGNMENT, "h");
    pkbDE.addUsesS(10, PRINT, "w");
    pkbDE.addUsesS(11, PRINT, "x");
    pkbDE.addUsesS(12, IF, "y");
    pkbDE.addUsesS(12, IF, "e");
    pkbDE.addUsesS(12, IF, "tough");
    pkbDE.addUsesS(12, IF, "light");
    pkbDE.addUsesS(12, IF, "heavy");
    pkbDE.addUsesS(12, IF, "bird");
    pkbDE.addUsesS(12, IF, "wings");
    pkbDE.addUsesS(12, IF, "d");
    pkbDE.addUsesS(12, IF, "m");

    pkbDE.addUsesS(13, ASSIGNMENT, "tough");
    pkbDE.addUsesS(14, ASSIGNMENT, "light");
    pkbDE.addUsesS(14, ASSIGNMENT, "heavy");
    pkbDE.addUsesS(15, ASSIGNMENT, "bird");
    pkbDE.addUsesS(15, ASSIGNMENT, "wings");
    pkbDE.addUsesS(16, WHILE, "d");
    pkbDE.addUsesS(16, WHILE, "m");
    pkbDE.addUsesS(20, PRINT, "assignments");

    pkbDE.addModifiesS(18, READ, "f");
    pkbDE.addModifiesS(19, READ, "module");
    pkbDE.addModifiesS(9, ASSIGNMENT, "t");
    pkbDE.addModifiesS(13, ASSIGNMENT, "big");
    pkbDE.addModifiesS(14, ASSIGNMENT, "heavy");
    pkbDE.addModifiesS(15, ASSIGNMENT, "feather");

    // procedure third
    pkbDE.addUsesS(20, PRINT, "assignments");
    pkbDE.addModifiesS(19, READ, "module");

    try {
        de->populateDesignAbstractions();
    }
    catch (const ParserException& e) {
        cout << e.what() << endl;
        string expectedException = "Invalid procedure called";
        REQUIRE(!(e.what() == expectedException.c_str()));
    }
}


TEST_CASE("Test Cyclic Procedures") {
    /* Unit test case used
        procedure first {
            call second;
        }

        procedure second {
            call first;
        }

        */
    pkbDE.clear();
    DesignExtractor* de = new DesignExtractor();

    pkbDE.addProcedure("first");
    pkbDE.addProcedure("second");
    pkbDE.addStmt(1, CALL, "first");
    pkbDE.addStmt(2, CALL, "second");
    pkbDE.addCalls("first", CALL, "second", 1);
    pkbDE.addCalls("second", CALL, "first", 2);


    try {
        de->populateDesignAbstractions();
    }
    catch (const ParserException& e) {
        cout << e.what() << endl;
        string expectedException = "Cyclic call detected";
        REQUIRE(!(e.what() == expectedException.c_str()));
    }
}

TEST_CASE("DE Test Add Follows Star") {
    pkbDE.clear();
    DesignExtractor* de = new DesignExtractor();
    // 1) Populate some add Follows r/s into PKB
    REQUIRE(pkbDE.addFollows(1, ASSIGNMENT, 2, ASSIGNMENT));
    REQUIRE(pkbDE.addFollows(2, ASSIGNMENT, 3, IF));
    REQUIRE(pkbDE.addFollows(4, ASSIGNMENT, 5, WHILE));
    REQUIRE(pkbDE.addFollows(5, WHILE, 8, ASSIGNMENT));
    REQUIRE(pkbDE.addFollows(6, CALL, 7, ASSIGNMENT));
    REQUIRE(pkbDE.addFollows(9, READ, 10, PRINT));
    REQUIRE(pkbDE.setStatementType(1, ASSIGNMENT));
    REQUIRE(pkbDE.setStatementType(2, ASSIGNMENT));
    REQUIRE(pkbDE.setStatementType(3, IF));
    REQUIRE(pkbDE.setStatementType(4, ASSIGNMENT));
    REQUIRE(pkbDE.setStatementType(5, WHILE));
    REQUIRE(pkbDE.setStatementType(6, CALL));
    REQUIRE(pkbDE.setStatementType(7, ASSIGNMENT));
    REQUIRE(pkbDE.setStatementType(8, ASSIGNMENT));
    REQUIRE(pkbDE.setStatementType(9, READ));
    REQUIRE(pkbDE.setStatementType(10, PRINT));
    pkbDE.setNum(10);
    // 2) Run DE to generate Follows Star
    de->populateDesignAbstractions();
    // 3) check that it works
    SECTION("get FollowsStar") {
        vector<std::pair<int, int> > expectedFollowsStar
            = { make_pair(1, 2), make_pair(2, 3), make_pair(1, 3), make_pair(4, 5), make_pair(4, 8), make_pair(6,7), make_pair(9,10), make_pair(5,8) };

        vector<pair<int, int>> getAllFollowsStar = pkbDE.getFollowsStarTT(NO_TYPE, NO_TYPE);
        REQUIRE(expectedFollowsStar.size() == getAllFollowsStar.size());
        for (int i = 0; i < getAllFollowsStar.size(); i++) {
            pair<int, int> innerPair = getAllFollowsStar[i];
            bool isPresent = std::find(expectedFollowsStar.begin(), expectedFollowsStar.end(), innerPair)
                != expectedFollowsStar.end();
            REQUIRE(isPresent);
        }

    }
}

TEST_CASE("Test Add Parent Star") {
    pkbDE.clear();
    DesignExtractor* de = new DesignExtractor();
    // 1) Populate some add Parent r/s into PKB
    REQUIRE(pkbDE.addParent(1, WHILE, 2, ASSIGNMENT));
    REQUIRE(pkbDE.addParent(1, WHILE, 3, ASSIGNMENT));
    REQUIRE(pkbDE.addParent(1, WHILE, 4, PRINT));
    REQUIRE(pkbDE.addParent(1, WHILE, 5, ASSIGNMENT));
    REQUIRE(pkbDE.addParent(5, WHILE, 6, READ));
    REQUIRE(pkbDE.addParent(5, WHILE, 7, ASSIGNMENT));
    REQUIRE(pkbDE.addParent(5, WHILE, 8, CALL));
    REQUIRE(pkbDE.addParent(5, WHILE, 9, READ));
    REQUIRE(pkbDE.addParent(9, IF, 10, ASSIGNMENT));

    REQUIRE(pkbDE.setStatementType(1, WHILE));
    REQUIRE(pkbDE.setStatementType(2, ASSIGNMENT));
    REQUIRE(pkbDE.setStatementType(3, ASSIGNMENT));
    REQUIRE(pkbDE.setStatementType(4, PRINT));
    REQUIRE(pkbDE.setStatementType(5, WHILE));
    REQUIRE(pkbDE.setStatementType(6, READ));
    REQUIRE(pkbDE.setStatementType(7, ASSIGNMENT));
    REQUIRE(pkbDE.setStatementType(8, CALL));
    REQUIRE(pkbDE.setStatementType(9, READ));
    REQUIRE(pkbDE.setStatementType(10, ASSIGNMENT));
    pkbDE.setNum(10);
    // 2) Run DE to generate Parent*
    de->populateDesignAbstractions();
    // 3) check that it works
    SECTION("get ParentStar") {
        vector<std::pair<int, int> > expectedParentStar
            = { make_pair(1, 2), make_pair(1, 3), make_pair(1, 4), make_pair(1, 5), make_pair(1, 6), make_pair(1,7),
            make_pair(1,8), make_pair(1,9), make_pair(1,10), make_pair(5, 6), make_pair(5, 7), make_pair(5, 8),
            make_pair(5, 9), make_pair(5, 10), make_pair(9,10) };

        vector<pair<int, int>> getAllParentStar = pkbDE.getParentStarTT(NO_TYPE, NO_TYPE);
        REQUIRE(expectedParentStar.size() == getAllParentStar.size());
        for (int i = 0; i < getAllParentStar.size(); i++) {
            pair<int, int> innerPair = getAllParentStar[i];
            bool isPresent = std::find(expectedParentStar.begin(), expectedParentStar.end(), innerPair)
                != expectedParentStar.end();
            REQUIRE(isPresent);
        }
    }
}

TEST_CASE("Test Add Uses and Modifies for procedures and calls in DE") {

    SECTION("With calls and without while/if") {
        pkbDE.clear();
        DesignExtractor* de = new DesignExtractor();
        /* Unit test case used
        procedure first {
            a = x + y; //1
            read b; // 2
            print c; // 3
            call second //4
            s = (123 + c) + (xyz + z); //5
        }

        procedure second {
            t = g * h; //6
            print w; //7
            print x; //8
            read f; //9
        }

        */
        pkbDE.addProcedure("first");
        pkbDE.addProcedure("second");
        pkbDE.addStmt(1, ASSIGNMENT, "first");
        pkbDE.addStmt(2, READ, "first");
        pkbDE.addStmt(3, PRINT, "first");
        pkbDE.addStmt(4, CALL, "first");
        pkbDE.addStmt(5, ASSIGNMENT, "first");
        pkbDE.addStmt(6, ASSIGNMENT, "second");
        pkbDE.addStmt(7, PRINT, "second");
        pkbDE.addStmt(8, PRINT, "second");
        pkbDE.addStmt(9, READ, "second");
        pkbDE.addCalls("first", CALL, "second", 4);
        // procedure first
        pkbDE.addUsesS(1, ASSIGNMENT, "x");
        pkbDE.addUsesS(1, ASSIGNMENT, "y");
        pkbDE.addUsesS(3, PRINT, "c");
        pkbDE.addUsesS(5, ASSIGNMENT, "c");
        pkbDE.addUsesS(5, ASSIGNMENT, "xyz");
        pkbDE.addUsesS(5, ASSIGNMENT, "z");
        pkbDE.addModifiesS(1, ASSIGNMENT, "a");
        pkbDE.addModifiesS(2, READ, "b");
        pkbDE.addModifiesS(5, ASSIGNMENT, "s");

        // procedure second
        pkbDE.addUsesS(6, ASSIGNMENT, "g");
        pkbDE.addUsesS(6, ASSIGNMENT, "h");
        pkbDE.addUsesS(7, PRINT, "w");
        pkbDE.addUsesS(8, PRINT, "x");
        pkbDE.addModifiesS(9, READ, "f");
        pkbDE.addModifiesS(6, ASSIGNMENT, "t");


        de->populateDesignAbstractions();

        // retrieves all Uses(p,v) relationship and check
        vector<pair<string, string>> allUsesProcVar = pkbDE.getUsesPTT(NO_TYPE, NO_TYPE);
        vector<std::pair<string, string> > expectedUsesProcedures
            = { make_pair("first", "x"), make_pair("first", "y"), make_pair("first", "c"), make_pair("first", "xyz"),
            make_pair("first", "z"), make_pair("first", "g"), make_pair("first", "h"), make_pair("first", "w"),
            make_pair("second", "g"),make_pair("second", "h"), make_pair("second", "w"), make_pair("second", "x") };

        REQUIRE(allUsesProcVar.size() == expectedUsesProcedures.size());
        for (int i = 0; i < allUsesProcVar.size(); i++) {
            pair<string, string> innerPair = allUsesProcVar[i];
            bool isPresent = std::find(expectedUsesProcedures.begin(), expectedUsesProcedures.end(), innerPair)
                != expectedUsesProcedures.end();
            REQUIRE(isPresent);
        }

        // retrieves all Modifies(p,v) relationship and check
        vector<pair<string, string>> allModifiesProcVar = pkbDE.getModifiesPTT(NO_TYPE, NO_TYPE);
        vector<std::pair<string, string> > expectedModifiesProcedures
            = { make_pair("first", "a"), make_pair("first", "b"), make_pair("first", "s"), make_pair("first", "f"),
            make_pair("first", "t"), make_pair("second", "f"),make_pair("second", "t") };

        REQUIRE(allModifiesProcVar.size() == expectedModifiesProcedures.size());
        for (int i = 0; i < allModifiesProcVar.size(); i++) {
            pair<string, string> innerPair = allModifiesProcVar[i];
            bool isPresent = std::find(expectedModifiesProcedures.begin(), expectedModifiesProcedures.end(), innerPair)
                != expectedModifiesProcedures.end();
            REQUIRE(isPresent);
        }


        // retrieves all Uses(c,v) and check
        vector<pair<int, string>> allUsesCallVar = pkbDE.getUsesSTT(CALL, NO_TYPE);
        vector<std::pair<int, string> > expectedUsesCalls
            = { make_pair(4, "g"), make_pair(4, "h"), make_pair(4, "w"),make_pair(4, "x") };

        REQUIRE(allUsesCallVar.size() == expectedUsesCalls.size());
        for (int i = 0; i < allUsesCallVar.size(); i++) {
            pair<int, string> innerPair = allUsesCallVar[i];
            bool isPresent = std::find(expectedUsesCalls.begin(), expectedUsesCalls.end(), innerPair)
                != expectedUsesCalls.end();
            REQUIRE(isPresent);
        }

        // retrieves all Modifies(c,v) and check
        vector<pair<int, string>> allModifiesCallVar = pkbDE.getModifiesSTT(CALL, NO_TYPE);
        vector<std::pair<int, string> > expectedModifiesCalls
            = { make_pair(4, "f"), make_pair(4, "t") };

        REQUIRE(allModifiesCallVar.size() == expectedModifiesCalls.size());
        for (int i = 0; i < allModifiesCallVar.size(); i++) {
            pair<int, string> innerPair = allModifiesCallVar[i];
            bool isPresent = std::find(expectedModifiesCalls.begin(), expectedModifiesCalls.end(), innerPair)
                != expectedModifiesCalls.end();
            REQUIRE(isPresent);
        }
    }

    SECTION("With calls and with while/if") {
        pkbDE.clear();
        DesignExtractor* de = new DesignExtractor();
        /* Unit test case used
        procedure first {
            a = x + y; //1
            read b; // 2
            print c; // 3
            while (x < y) { //4
                print k; //5
                print done; //6
            }
            call second //7
            s = (123 + c) + (xyz + z); //8
        }

        procedure second {
            t = g * h; //9
            print w; //10
            print x; //11
            if (y < e) { //12
                big = tough + tough; //13
                heavy = light + heavy; //14
            } else {
                feather = bird + wings; //15
            }
            read f; //16
        }

        */

        pkbDE.addProcedure("first");
        pkbDE.addProcedure("second");
        pkbDE.addStmt(1, ASSIGNMENT, "first");
        pkbDE.addStmt(2, READ, "first");
        pkbDE.addStmt(3, PRINT, "first");
        pkbDE.addStmt(4, WHILE, "first");
        pkbDE.addWhileStmt(4);
        pkbDE.addWhileBlockStmt(4, 5);
        pkbDE.addWhileBlockStmt(4, 6);
        pkbDE.addStmt(5, PRINT, "first");
        pkbDE.addStmt(6, PRINT, "first");
        pkbDE.addStmt(7, CALL, "first");
        pkbDE.addStmt(8, ASSIGNMENT, "first");
        pkbDE.addStmt(9, ASSIGNMENT, "second");
        pkbDE.addStmt(10, PRINT, "second");
        pkbDE.addStmt(11, PRINT, "second");
        pkbDE.addStmt(12, IF, "second");
        pkbDE.addIfStmt(12);
        pkbDE.addIfBlockStmt(12, 13);
        pkbDE.addIfBlockStmt(12, 14);
        pkbDE.addElseBlockStmt(12, 15);
        pkbDE.addStmt(13, ASSIGNMENT, "second");
        pkbDE.addStmt(14, ASSIGNMENT, "second");
        pkbDE.addStmt(15, ASSIGNMENT, "second");
        pkbDE.addStmt(16, READ, "second");
        pkbDE.addCalls("first", CALL, "second", 7);

        // procedure first
        pkbDE.addUsesS(1, ASSIGNMENT, "x");
        pkbDE.addUsesS(1, ASSIGNMENT, "y");
        pkbDE.addUsesS(3, PRINT, "c");
        pkbDE.addUsesS(4, WHILE, "x");
        pkbDE.addUsesS(4, WHILE, "y");
        pkbDE.addUsesS(4, WHILE, "k");
        pkbDE.addUsesS(4, WHILE, "done");
        pkbDE.addUsesS(5, PRINT, "k");
        pkbDE.addUsesS(6, PRINT, "done");
        pkbDE.addUsesS(8, ASSIGNMENT, "c");
        pkbDE.addUsesS(8, ASSIGNMENT, "xyz");
        pkbDE.addUsesS(8, ASSIGNMENT, "z");

        pkbDE.addModifiesS(1, ASSIGNMENT, "a");
        pkbDE.addModifiesS(2, READ, "b");
        pkbDE.addModifiesS(8, ASSIGNMENT, "s");
        pkbDE.addModifiesS(12, IF, "big");
        pkbDE.addModifiesS(12, IF, "heavy");
        pkbDE.addModifiesS(12, IF, "feather");

        // procedure second
        pkbDE.addUsesS(9, ASSIGNMENT, "g");
        pkbDE.addUsesS(9, ASSIGNMENT, "h");
        pkbDE.addUsesS(10, PRINT, "w");
        pkbDE.addUsesS(11, PRINT, "x");
        pkbDE.addUsesS(12, IF, "y");
        pkbDE.addUsesS(12, IF, "e");
        pkbDE.addUsesS(12, IF, "tough");
        pkbDE.addUsesS(12, IF, "light");
        pkbDE.addUsesS(12, IF, "heavy");
        pkbDE.addUsesS(12, IF, "bird");
        pkbDE.addUsesS(12, IF, "wings");
        pkbDE.addUsesS(13, ASSIGNMENT, "tough");
        pkbDE.addUsesS(14, ASSIGNMENT, "light");
        pkbDE.addUsesS(14, ASSIGNMENT, "heavy");
        pkbDE.addUsesS(15, ASSIGNMENT, "bird");
        pkbDE.addUsesS(15, ASSIGNMENT, "wings");
        pkbDE.addModifiesS(16, READ, "f");
        pkbDE.addModifiesS(9, ASSIGNMENT, "t");
        pkbDE.addModifiesS(13, ASSIGNMENT, "big");
        pkbDE.addModifiesS(14, ASSIGNMENT, "heavy");
        pkbDE.addModifiesS(15, ASSIGNMENT, "feather");


        de->populateDesignAbstractions();

        // retrieves all Uses(p,v) relationship and check
        vector<pair<string, string>> allUsesProcVar = pkbDE.getUsesPTT(NO_TYPE, NO_TYPE);
        vector<std::pair<string, string> > expectedUsesProcedures
            = { make_pair("first", "x"), make_pair("first", "y"), make_pair("first", "c"), make_pair("first", "xyz"),
            make_pair("first", "z"), make_pair("first", "k"), make_pair("first", "done"), make_pair("first", "g"), make_pair("first", "h"), make_pair("first", "w"), make_pair("first", "e"), make_pair("first", "tough"), make_pair("first", "light"), make_pair("first", "heavy"), make_pair("first", "bird"),make_pair("first", "wings"),
            make_pair("second", "g"),make_pair("second", "h"), make_pair("second", "w"), make_pair("second", "x"), make_pair("second", "y"), make_pair("second", "e"), make_pair("second", "tough"), make_pair("second", "light"), make_pair("second", "heavy"), make_pair("second", "bird"), make_pair("second", "wings") };

        REQUIRE(allUsesProcVar.size() == expectedUsesProcedures.size());
        for (int i = 0; i < allUsesProcVar.size(); i++) {
            pair<string, string> innerPair = allUsesProcVar[i];
            bool isPresent = std::find(expectedUsesProcedures.begin(), expectedUsesProcedures.end(), innerPair)
                != expectedUsesProcedures.end();
            REQUIRE(isPresent);
        }

        // retrieves all Modifies(p,v) relationship and check
        vector<pair<string, string>> allModifiesProcVar = pkbDE.getModifiesPTT(NO_TYPE, NO_TYPE);
        vector<std::pair<string, string> > expectedModifiesProcedures
            = { make_pair("first", "a"), make_pair("first", "b"), make_pair("first", "s"), make_pair("first", "f"),
            make_pair("first", "t"), make_pair("first", "big"), make_pair("first", "heavy"),
            make_pair("first", "feather"), make_pair("second", "f"), make_pair("second", "t"),
            make_pair("second", "big"), make_pair("second", "heavy"), make_pair("second", "feather") };

        REQUIRE(allModifiesProcVar.size() == expectedModifiesProcedures.size());
        for (int i = 0; i < allModifiesProcVar.size(); i++) {
            pair<string, string> innerPair = allModifiesProcVar[i];
            bool isPresent = std::find(expectedModifiesProcedures.begin(), expectedModifiesProcedures.end(), innerPair)
                != expectedModifiesProcedures.end();
            REQUIRE(isPresent);
        }

        // retrieves all Uses(c,v) and check

        vector<pair<int, string>> allUsesCallVar = pkbDE.getUsesSTT(CALL, NO_TYPE);
        vector<std::pair<int, string> > expectedUsesCalls
            = { make_pair(7, "g"), make_pair(7, "h"), make_pair(7, "w"),make_pair(7, "x"), make_pair(7, "y"),
            make_pair(7, "e"), make_pair(7, "tough"), make_pair(7, "light"), make_pair(7, "heavy"),
            make_pair(7, "bird"),make_pair(7, "wings") };

        REQUIRE(allUsesCallVar.size() == expectedUsesCalls.size());
        for (int i = 0; i < allUsesCallVar.size(); i++) {
            pair<int, string> innerPair = allUsesCallVar[i];
            bool isPresent = std::find(expectedUsesCalls.begin(), expectedUsesCalls.end(), innerPair)
                != expectedUsesCalls.end();
            REQUIRE(isPresent);
        }

        // retrieves all Modifies(c,v) and check
        vector<pair<int, string>> allModifiesCallVar = pkbDE.getModifiesSTT(CALL, NO_TYPE);
        vector<std::pair<int, string> > expectedModifiesCalls
            = { make_pair(7, "f"), make_pair(7, "t"), make_pair(7, "big"), make_pair(7, "heavy"), make_pair(7, "feather") };

        REQUIRE(allModifiesCallVar.size() == expectedModifiesCalls.size());
        for (int i = 0; i < allModifiesCallVar.size(); i++) {
            pair<int, string> innerPair = allModifiesCallVar[i];
            bool isPresent = std::find(expectedModifiesCalls.begin(), expectedModifiesCalls.end(), innerPair)
                != expectedModifiesCalls.end();
            REQUIRE(isPresent);
        }

    }


    ///
    SECTION("With call in container") {
        pkbDE.clear();
        DesignExtractor* de = new DesignExtractor();
        /* Unit test case used
        procedure first {
            a = x + y; //1
            read b; // 2
            print c; // 3
            while (x < y) { //4
                print k; //5
                print done; //6
                call second //7
            }

            s = (123 + c) + (xyz + z); //8
        }

        procedure second {
            t = g * h; //9
            print w; //10
            print x; //11
            if (y < e) { //12
                big = tough + tough; //13
                heavy = light + heavy; //14
            } else {
                feather = bird + wings; //15
            }
            read f; //16
        }

        */

        pkbDE.addProcedure("first");
        pkbDE.addProcedure("second");
        pkbDE.addStmt(1, ASSIGNMENT, "first");
        pkbDE.addStmt(2, READ, "first");
        pkbDE.addStmt(3, PRINT, "first");
        pkbDE.addStmt(4, WHILE, "first");
        pkbDE.addWhileStmt(4);
        pkbDE.addWhileBlockStmt(4, 5);
        pkbDE.addWhileBlockStmt(4, 6);
        pkbDE.addStmt(5, PRINT, "first");
        pkbDE.addStmt(6, PRINT, "first");
        pkbDE.addStmt(7, CALL, "first");
        pkbDE.addParent(4, WHILE, 5, PRINT);
        pkbDE.addParentStar(4, WHILE, 5, PRINT);
        pkbDE.addParent(4, WHILE, 6, PRINT);
        pkbDE.addParentStar(4, WHILE, 6, PRINT);
        pkbDE.addParent(4, WHILE, 7, CALL);
        pkbDE.addParentStar(4, WHILE, 7, CALL);
        pkbDE.addStmt(8, ASSIGNMENT, "first");
        pkbDE.addStmt(9, ASSIGNMENT, "second");
        pkbDE.addStmt(10, PRINT, "second");
        pkbDE.addStmt(11, PRINT, "second");
        pkbDE.addStmt(12, IF, "second");
        pkbDE.addIfStmt(12);
        pkbDE.addIfBlockStmt(12, 13);
        pkbDE.addIfBlockStmt(12, 14);
        pkbDE.addElseBlockStmt(12, 15);
        pkbDE.addStmt(13, ASSIGNMENT, "second");
        pkbDE.addStmt(14, ASSIGNMENT, "second");
        pkbDE.addStmt(15, ASSIGNMENT, "second");
        pkbDE.addStmt(16, READ, "second");
        pkbDE.addCalls("first", CALL, "second", 7);

        // procedure first
        pkbDE.addUsesS(1, ASSIGNMENT, "x");
        pkbDE.addUsesS(1, ASSIGNMENT, "y");
        pkbDE.addUsesS(3, PRINT, "c");
        pkbDE.addUsesS(4, WHILE, "x");
        pkbDE.addUsesS(4, WHILE, "y");
        pkbDE.addUsesS(4, WHILE, "k");
        pkbDE.addUsesS(4, WHILE, "done");
        pkbDE.addUsesS(5, PRINT, "k");
        pkbDE.addUsesS(6, PRINT, "done");
        pkbDE.addUsesS(8, ASSIGNMENT, "c");
        pkbDE.addUsesS(8, ASSIGNMENT, "xyz");
        pkbDE.addUsesS(8, ASSIGNMENT, "z");

        pkbDE.addModifiesS(1, ASSIGNMENT, "a");
        pkbDE.addModifiesS(2, READ, "b");
        pkbDE.addModifiesS(8, ASSIGNMENT, "s");
        pkbDE.addModifiesS(12, IF, "big");
        pkbDE.addModifiesS(12, IF, "heavy");
        pkbDE.addModifiesS(12, IF, "feather");

        // procedure second
        pkbDE.addUsesS(9, ASSIGNMENT, "g");
        pkbDE.addUsesS(9, ASSIGNMENT, "h");
        pkbDE.addUsesS(10, PRINT, "w");
        pkbDE.addUsesS(11, PRINT, "x");
        pkbDE.addUsesS(12, IF, "y");
        pkbDE.addUsesS(12, IF, "e");
        pkbDE.addUsesS(12, IF, "tough");
        pkbDE.addUsesS(12, IF, "light");
        pkbDE.addUsesS(12, IF, "heavy");
        pkbDE.addUsesS(12, IF, "bird");
        pkbDE.addUsesS(12, IF, "wings");
        pkbDE.addUsesS(13, ASSIGNMENT, "tough");
        pkbDE.addUsesS(14, ASSIGNMENT, "light");
        pkbDE.addUsesS(14, ASSIGNMENT, "heavy");
        pkbDE.addUsesS(15, ASSIGNMENT, "bird");
        pkbDE.addUsesS(15, ASSIGNMENT, "wings");

        pkbDE.addParent(12, IF, 13, ASSIGNMENT);
        pkbDE.addParentStar(12, IF, 13, ASSIGNMENT);
        pkbDE.addParent(12, IF, 14, ASSIGNMENT);
        pkbDE.addParentStar(12, IF, 14, ASSIGNMENT);
        pkbDE.addParent(12, IF, 15, ASSIGNMENT);
        pkbDE.addParentStar(12, IF, 15, ASSIGNMENT);

        pkbDE.addModifiesS(16, READ, "f");
        pkbDE.addModifiesS(9, ASSIGNMENT, "t");
        pkbDE.addModifiesS(13, ASSIGNMENT, "big");
        pkbDE.addModifiesS(14, ASSIGNMENT, "heavy");
        pkbDE.addModifiesS(15, ASSIGNMENT, "feather");


        de->populateDesignAbstractions();

        // retrieves all Uses(p,v) relationship and check
        vector<pair<string, string>> allUsesProcVar = pkbDE.getUsesPTT(NO_TYPE, NO_TYPE);
        vector<std::pair<string, string> > expectedUsesProcedures
            = { make_pair("first", "x"), make_pair("first", "y"), make_pair("first", "c"), make_pair("first", "xyz"),
            make_pair("first", "z"), make_pair("first", "k"), make_pair("first", "done"), make_pair("first", "g"), make_pair("first", "h"), make_pair("first", "w"), make_pair("first", "e"), make_pair("first", "tough"), make_pair("first", "light"), make_pair("first", "heavy"), make_pair("first", "bird"),make_pair("first", "wings"),
            make_pair("second", "g"),make_pair("second", "h"), make_pair("second", "w"), make_pair("second", "x"), make_pair("second", "y"), make_pair("second", "e"), make_pair("second", "tough"), make_pair("second", "light"), make_pair("second", "heavy"), make_pair("second", "bird"), make_pair("second", "wings") };

        REQUIRE(allUsesProcVar.size() == expectedUsesProcedures.size());
        for (int i = 0; i < allUsesProcVar.size(); i++) {
            pair<string, string> innerPair = allUsesProcVar[i];
            bool isPresent = std::find(expectedUsesProcedures.begin(), expectedUsesProcedures.end(), innerPair)
                != expectedUsesProcedures.end();
            REQUIRE(isPresent);
        }

        // retrieves all Modifies(p,v) relationship and check
        vector<pair<string, string>> allModifiesProcVar = pkbDE.getModifiesPTT(NO_TYPE, NO_TYPE);
        vector<std::pair<string, string> > expectedModifiesProcedures
            = { make_pair("first", "a"), make_pair("first", "b"), make_pair("first", "s"), make_pair("first", "f"),
            make_pair("first", "t"), make_pair("first", "big"), make_pair("first", "heavy"),
            make_pair("first", "feather"), make_pair("second", "f"), make_pair("second", "t"),
            make_pair("second", "big"), make_pair("second", "heavy"), make_pair("second", "feather") };

        REQUIRE(allModifiesProcVar.size() == expectedModifiesProcedures.size());
        for (int i = 0; i < allModifiesProcVar.size(); i++) {
            pair<string, string> innerPair = allModifiesProcVar[i];
            bool isPresent = std::find(expectedModifiesProcedures.begin(), expectedModifiesProcedures.end(), innerPair)
                != expectedModifiesProcedures.end();
            REQUIRE(isPresent);
        }

        // retrieves all Uses(c,v) and check

        vector<pair<int, string>> allUsesCallVar = pkbDE.getUsesSTT(CALL, NO_TYPE);
        vector<std::pair<int, string> > expectedUsesCalls
            = { make_pair(7, "g"), make_pair(7, "h"), make_pair(7, "w"),make_pair(7, "x"), make_pair(7, "y"),
            make_pair(7, "e"), make_pair(7, "tough"), make_pair(7, "light"), make_pair(7, "heavy"),
            make_pair(7, "bird"),make_pair(7, "wings") };

        REQUIRE(allUsesCallVar.size() == expectedUsesCalls.size());
        for (int i = 0; i < allUsesCallVar.size(); i++) {
            pair<int, string> innerPair = allUsesCallVar[i];
            bool isPresent = std::find(expectedUsesCalls.begin(), expectedUsesCalls.end(), innerPair)
                != expectedUsesCalls.end();
            REQUIRE(isPresent);
        }

        // retrieves all Modifies(c,v) and check
        vector<pair<int, string>> allModifiesCallVar = pkbDE.getModifiesSTT(CALL, NO_TYPE);
        vector<std::pair<int, string> > expectedModifiesCalls
            = { make_pair(7, "f"), make_pair(7, "t"), make_pair(7, "big"), make_pair(7, "heavy"), make_pair(7, "feather") };

        REQUIRE(allModifiesCallVar.size() == expectedModifiesCalls.size());
        for (int i = 0; i < allModifiesCallVar.size(); i++) {
            pair<int, string> innerPair = allModifiesCallVar[i];
            bool isPresent = std::find(expectedModifiesCalls.begin(), expectedModifiesCalls.end(), innerPair)
                != expectedModifiesCalls.end();
            REQUIRE(isPresent);
        }

        // check while in first procedure, should contain uses and modifies in second procedure

        // Retrieves uses to check
        vector<pair<int, string>> allUsesWhileVar = pkbDE.getUsesSTT(WHILE, NO_TYPE);
        vector<std::pair<int, string> > expectedUsesWhile
            = { make_pair(4, "x"), make_pair(4, "y"), make_pair(4, "k"),make_pair(4, "done"), make_pair(4, "g"),
            make_pair(4, "h"), make_pair(4, "w"), make_pair(4, "e"), make_pair(4, "tough"),
            make_pair(4, "light"), make_pair(4, "heavy"), make_pair(4,"bird"), make_pair(4, "wings") };

        REQUIRE(allUsesWhileVar.size() == expectedUsesWhile.size());
        for (int i = 0; i < allUsesWhileVar.size(); i++) {
            pair<int, string> innerPair = allUsesWhileVar[i];
            bool isPresent = std::find(expectedUsesWhile.begin(), expectedUsesWhile.end(), innerPair)
                != expectedUsesWhile.end();
            REQUIRE(isPresent);
        }

        // Retrieves modifies to check
        vector<pair<int, string>> allModifiesWhileVar = pkbDE.getModifiesSTT(WHILE, NO_TYPE);
        vector<std::pair<int, string> > expectedModifiesWhile
            = { make_pair(4, "f"), make_pair(4, "t"), make_pair(4, "big"),make_pair(4, "heavy"), make_pair(4, "feather") };

        REQUIRE(allModifiesWhileVar.size() == expectedModifiesWhile.size());
        for (int i = 0; i < allModifiesWhileVar.size(); i++) {
            pair<int, string> innerPair = allModifiesWhileVar[i];
            bool isPresent = std::find(expectedModifiesWhile.begin(), expectedModifiesWhile.end(), innerPair)
                != expectedModifiesWhile.end();
            REQUIRE(isPresent);
        }


    }

    ///
    SECTION("With call in container") {
        pkbDE.clear();
        DesignExtractor* de = new DesignExtractor();
        /* Unit test case used
        procedure first {
            a = x + y; //1
            read b; // 2
            print c; // 3
            while (x < y) { //4
                print k; //5
                print done; //6
                call second //7
            }

            s = (123 + c) + (xyz + z); //8
        }

        procedure second {
            t = g * h; //9
            print w; //10
            print x; //11
            if (y < e) { //12
                big = tough + tough; //13
                heavy = light + heavy; //14
            } else {
                feather = bird + wings; //15
            }
            read f; //16
        }

        */

        pkbDE.addProcedure("first");
        pkbDE.addProcedure("second");
        pkbDE.addStmt(1, ASSIGNMENT, "first");
        pkbDE.addStmt(2, READ, "first");
        pkbDE.addStmt(3, PRINT, "first");
        pkbDE.addStmt(4, WHILE, "first");
        pkbDE.addWhileStmt(4);
        pkbDE.addWhileBlockStmt(4, 5);
        pkbDE.addWhileBlockStmt(4, 6);
        pkbDE.addStmt(5, PRINT, "first");
        pkbDE.addStmt(6, PRINT, "first");
        pkbDE.addStmt(7, CALL, "first");
        pkbDE.addParent(4, WHILE, 5, PRINT);
        pkbDE.addParentStar(4, WHILE, 5, PRINT);
        pkbDE.addParent(4, WHILE, 6, PRINT);
        pkbDE.addParentStar(4, WHILE, 6, PRINT);
        pkbDE.addParent(4, WHILE, 7, CALL);
        pkbDE.addParentStar(4, WHILE, 7, CALL);
        pkbDE.addStmt(8, ASSIGNMENT, "first");
        pkbDE.addStmt(9, ASSIGNMENT, "second");
        pkbDE.addStmt(10, PRINT, "second");
        pkbDE.addStmt(11, PRINT, "second");
        pkbDE.addStmt(12, IF, "second");
        pkbDE.addIfStmt(12);
        pkbDE.addIfBlockStmt(12, 13);
        pkbDE.addIfBlockStmt(12, 14);
        pkbDE.addElseBlockStmt(12, 15);
        pkbDE.addStmt(13, ASSIGNMENT, "second");
        pkbDE.addStmt(14, ASSIGNMENT, "second");
        pkbDE.addStmt(15, ASSIGNMENT, "second");
        pkbDE.addStmt(16, READ, "second");
        pkbDE.addCalls("first", CALL, "second", 7);

        // procedure first
        pkbDE.addUsesS(1, ASSIGNMENT, "x");
        pkbDE.addUsesS(1, ASSIGNMENT, "y");
        pkbDE.addUsesS(3, PRINT, "c");
        pkbDE.addUsesS(4, WHILE, "x");
        pkbDE.addUsesS(4, WHILE, "y");
        pkbDE.addUsesS(4, WHILE, "k");
        pkbDE.addUsesS(4, WHILE, "done");
        pkbDE.addUsesS(5, PRINT, "k");
        pkbDE.addUsesS(6, PRINT, "done");
        pkbDE.addUsesS(8, ASSIGNMENT, "c");
        pkbDE.addUsesS(8, ASSIGNMENT, "xyz");
        pkbDE.addUsesS(8, ASSIGNMENT, "z");

        pkbDE.addModifiesS(1, ASSIGNMENT, "a");
        pkbDE.addModifiesS(2, READ, "b");
        pkbDE.addModifiesS(8, ASSIGNMENT, "s");
        pkbDE.addModifiesS(12, IF, "big");
        pkbDE.addModifiesS(12, IF, "heavy");
        pkbDE.addModifiesS(12, IF, "feather");

        // procedure second
        pkbDE.addUsesS(9, ASSIGNMENT, "g");
        pkbDE.addUsesS(9, ASSIGNMENT, "h");
        pkbDE.addUsesS(10, PRINT, "w");
        pkbDE.addUsesS(11, PRINT, "x");
        pkbDE.addUsesS(12, IF, "y");
        pkbDE.addUsesS(12, IF, "e");
        pkbDE.addUsesS(12, IF, "tough");
        pkbDE.addUsesS(12, IF, "light");
        pkbDE.addUsesS(12, IF, "heavy");
        pkbDE.addUsesS(12, IF, "bird");
        pkbDE.addUsesS(12, IF, "wings");

        pkbDE.addUsesS(13, ASSIGNMENT, "tough");
        pkbDE.addUsesS(14, ASSIGNMENT, "light");
        pkbDE.addUsesS(14, ASSIGNMENT, "heavy");
        pkbDE.addUsesS(15, ASSIGNMENT, "bird");
        pkbDE.addUsesS(15, ASSIGNMENT, "wings");
        pkbDE.addParent(12, IF, 13, ASSIGNMENT);
        pkbDE.addParentStar(12, IF, 13, ASSIGNMENT);
        pkbDE.addParent(12, IF, 14, ASSIGNMENT);
        pkbDE.addParentStar(12, IF, 14, ASSIGNMENT);
        pkbDE.addParent(12, IF, 15, ASSIGNMENT);
        pkbDE.addParentStar(12, IF, 15, ASSIGNMENT);

        pkbDE.addModifiesS(16, READ, "f");
        pkbDE.addModifiesS(9, ASSIGNMENT, "t");
        pkbDE.addModifiesS(13, ASSIGNMENT, "big");
        pkbDE.addModifiesS(14, ASSIGNMENT, "heavy");
        pkbDE.addModifiesS(15, ASSIGNMENT, "feather");


        de->populateDesignAbstractions();

        // retrieves all Uses(p,v) relationship and check
        vector<pair<string, string>> allUsesProcVar = pkbDE.getUsesPTT(NO_TYPE, NO_TYPE);
        vector<std::pair<string, string> > expectedUsesProcedures
            = { make_pair("first", "x"), make_pair("first", "y"), make_pair("first", "c"), make_pair("first", "xyz"),
            make_pair("first", "z"), make_pair("first", "k"), make_pair("first", "done"), make_pair("first", "g"), make_pair("first", "h"), make_pair("first", "w"), make_pair("first", "e"), make_pair("first", "tough"), make_pair("first", "light"), make_pair("first", "heavy"), make_pair("first", "bird"),make_pair("first", "wings"),
            make_pair("second", "g"),make_pair("second", "h"), make_pair("second", "w"), make_pair("second", "x"), make_pair("second", "y"), make_pair("second", "e"), make_pair("second", "tough"), make_pair("second", "light"), make_pair("second", "heavy"), make_pair("second", "bird"), make_pair("second", "wings") };

        REQUIRE(allUsesProcVar.size() == expectedUsesProcedures.size());
        for (int i = 0; i < allUsesProcVar.size(); i++) {
            pair<string, string> innerPair = allUsesProcVar[i];
            bool isPresent = std::find(expectedUsesProcedures.begin(), expectedUsesProcedures.end(), innerPair)
                != expectedUsesProcedures.end();
            REQUIRE(isPresent);
        }

        // retrieves all Modifies(p,v) relationship and check
        vector<pair<string, string>> allModifiesProcVar = pkbDE.getModifiesPTT(NO_TYPE, NO_TYPE);
        vector<std::pair<string, string> > expectedModifiesProcedures
            = { make_pair("first", "a"), make_pair("first", "b"), make_pair("first", "s"), make_pair("first", "f"),
            make_pair("first", "t"), make_pair("first", "big"), make_pair("first", "heavy"),
            make_pair("first", "feather"), make_pair("second", "f"), make_pair("second", "t"),
            make_pair("second", "big"), make_pair("second", "heavy"), make_pair("second", "feather") };

        REQUIRE(allModifiesProcVar.size() == expectedModifiesProcedures.size());
        for (int i = 0; i < allModifiesProcVar.size(); i++) {
            pair<string, string> innerPair = allModifiesProcVar[i];
            bool isPresent = std::find(expectedModifiesProcedures.begin(), expectedModifiesProcedures.end(), innerPair)
                != expectedModifiesProcedures.end();
            REQUIRE(isPresent);
        }

        // retrieves all Uses(c,v) and check

        vector<pair<int, string>> allUsesCallVar = pkbDE.getUsesSTT(CALL, NO_TYPE);
        vector<std::pair<int, string> > expectedUsesCalls
            = { make_pair(7, "g"), make_pair(7, "h"), make_pair(7, "w"),make_pair(7, "x"), make_pair(7, "y"),
            make_pair(7, "e"), make_pair(7, "tough"), make_pair(7, "light"), make_pair(7, "heavy"),
            make_pair(7, "bird"),make_pair(7, "wings") };

        REQUIRE(allUsesCallVar.size() == expectedUsesCalls.size());
        for (int i = 0; i < allUsesCallVar.size(); i++) {
            pair<int, string> innerPair = allUsesCallVar[i];
            bool isPresent = std::find(expectedUsesCalls.begin(), expectedUsesCalls.end(), innerPair)
                != expectedUsesCalls.end();
            REQUIRE(isPresent);
        }

        // retrieves all Modifies(c,v) and check
        vector<pair<int, string>> allModifiesCallVar = pkbDE.getModifiesSTT(CALL, NO_TYPE);
        vector<std::pair<int, string> > expectedModifiesCalls
            = { make_pair(7, "f"), make_pair(7, "t"), make_pair(7, "big"), make_pair(7, "heavy"), make_pair(7, "feather") };

        REQUIRE(allModifiesCallVar.size() == expectedModifiesCalls.size());
        for (int i = 0; i < allModifiesCallVar.size(); i++) {
            pair<int, string> innerPair = allModifiesCallVar[i];
            bool isPresent = std::find(expectedModifiesCalls.begin(), expectedModifiesCalls.end(), innerPair)
                != expectedModifiesCalls.end();
            REQUIRE(isPresent);
        }

        // check while in first procedure, should contain uses and modifies in second procedure

        // Retrieves uses to check
        vector<pair<int, string>> allUsesWhileVar = pkbDE.getUsesSTT(WHILE, NO_TYPE);
        vector<std::pair<int, string> > expectedUsesWhile
            = { make_pair(4, "x"), make_pair(4, "y"), make_pair(4, "k"),make_pair(4, "done"), make_pair(4, "g"),
            make_pair(4, "h"), make_pair(4, "w"), make_pair(4, "e"), make_pair(4, "tough"),
            make_pair(4, "light"), make_pair(4, "heavy"), make_pair(4,"bird"), make_pair(4, "wings") };

        REQUIRE(allUsesWhileVar.size() == expectedUsesWhile.size());
        for (int i = 0; i < allUsesWhileVar.size(); i++) {
            pair<int, string> innerPair = allUsesWhileVar[i];
            bool isPresent = std::find(expectedUsesWhile.begin(), expectedUsesWhile.end(), innerPair)
                != expectedUsesWhile.end();
            REQUIRE(isPresent);
        }

        // Retrieves modifies to check
        vector<pair<int, string>> allModifiesWhileVar = pkbDE.getModifiesSTT(WHILE, NO_TYPE);
        vector<std::pair<int, string> > expectedModifiesWhile
            = { make_pair(4, "f"), make_pair(4, "t"), make_pair(4, "big"),make_pair(4, "heavy"), make_pair(4, "feather") };

        REQUIRE(allModifiesWhileVar.size() == expectedModifiesWhile.size());
        for (int i = 0; i < allModifiesWhileVar.size(); i++) {
            pair<int, string> innerPair = allModifiesWhileVar[i];
            bool isPresent = std::find(expectedModifiesWhile.begin(), expectedModifiesWhile.end(), innerPair)
                != expectedModifiesWhile.end();
            REQUIRE(isPresent);
        }


    }


    SECTION("With nested if, calls statements") {
        pkbDE.clear();
        DesignExtractor* de = new DesignExtractor();
        /* Unit test case used
        procedure first {
            a = x + y; //1
            read b; // 2
            print c; // 3
            while (x < y) { //4
                print k; //5
                print done; //6
                call second //7
            }

            s = (123 + c) + (xyz + z); //8
        }

        procedure second {
            t = g * h; //9
            print w; //10
            print x; //11
            if (y < e) { //12
                big = tough + tough; //13
                heavy = light + heavy; //14
            } else {
                feather = bird + wings; //15
                while (d < m) { //16
                    call third; //17
                }
            }
            read f; //18
        }

        procedure third {
            read module; // 19
            print assignments; //20
        }

        */

        pkbDE.addProcedure("first");
        pkbDE.addProcedure("second");
        pkbDE.addProcedure("third");
        pkbDE.addStmt(1, ASSIGNMENT, "first");
        pkbDE.addStmt(2, READ, "first");
        pkbDE.addStmt(3, PRINT, "first");
        pkbDE.addStmt(4, WHILE, "first");
        pkbDE.addWhileStmt(4);
        pkbDE.addWhileBlockStmt(4, 5);
        pkbDE.addWhileBlockStmt(4, 6);
        pkbDE.addStmt(5, PRINT, "first");
        pkbDE.addStmt(6, PRINT, "first");
        pkbDE.addStmt(7, CALL, "first");
        pkbDE.addParent(4, WHILE, 5, PRINT);
        pkbDE.addParentStar(4, WHILE, 5, PRINT);
        pkbDE.addParent(4, WHILE, 6, PRINT);
        pkbDE.addParentStar(4, WHILE, 6, PRINT);
        pkbDE.addParent(4, WHILE, 7, CALL);
        pkbDE.addParentStar(4, WHILE, 7, CALL);
        pkbDE.addStmt(8, ASSIGNMENT, "first");
        pkbDE.addStmt(9, ASSIGNMENT, "second");
        pkbDE.addStmt(10, PRINT, "second");
        pkbDE.addStmt(11, PRINT, "second");
        pkbDE.addStmt(12, IF, "second");
        pkbDE.addIfStmt(12);
        pkbDE.addIfBlockStmt(12, 13);
        pkbDE.addIfBlockStmt(12, 14);
        pkbDE.addElseBlockStmt(12, 15);
        pkbDE.addElseBlockStmt(12, 16);
        pkbDE.addElseBlockStmt(12, 17);
        pkbDE.addParent(12, IF, 13, ASSIGNMENT);
        pkbDE.addParentStar(12, IF, 13, ASSIGNMENT);
        pkbDE.addParent(12, IF, 14, ASSIGNMENT);
        pkbDE.addParentStar(12, IF, 14, ASSIGNMENT);
        pkbDE.addParent(12, IF, 15, ASSIGNMENT);
        pkbDE.addParentStar(12, IF, 15, ASSIGNMENT);
        pkbDE.addParent(12, IF, 16, WHILE);
        pkbDE.addParentStar(12, IF, 16, WHILE);
        pkbDE.addParentStar(12, IF, 17, WHILE);
        pkbDE.addStmt(13, ASSIGNMENT, "second");
        pkbDE.addStmt(14, ASSIGNMENT, "second");
        pkbDE.addStmt(15, ASSIGNMENT, "second");
        pkbDE.addStmt(16, WHILE, "second");
        pkbDE.addStmt(17, CALL, "second");
        pkbDE.addWhileStmt(16);
        pkbDE.addWhileBlockStmt(16, 17);
        pkbDE.addParent(16, WHILE, 17, CALL);
        pkbDE.addParentStar(16, WHILE, 17, CALL);
        pkbDE.addStmt(18, READ, "second");

        pkbDE.addStmt(19, READ, "third");
        pkbDE.addStmt(20, PRINT, "third");

        pkbDE.addCalls("first", CALL, "second", 7);
        pkbDE.addCalls("second", CALL, "third", 17);

        // procedure first
        pkbDE.addUsesS(1, ASSIGNMENT, "x");
        pkbDE.addUsesS(1, ASSIGNMENT, "y");
        pkbDE.addUsesS(3, PRINT, "c");
        pkbDE.addUsesS(4, WHILE, "x");
        pkbDE.addUsesS(4, WHILE, "y");
        pkbDE.addUsesS(4, WHILE, "k");
        pkbDE.addUsesS(4, WHILE, "done");
        pkbDE.addUsesS(5, PRINT, "k");
        pkbDE.addUsesS(6, PRINT, "done");
        pkbDE.addUsesS(8, ASSIGNMENT, "c");
        pkbDE.addUsesS(8, ASSIGNMENT, "xyz");
        pkbDE.addUsesS(8, ASSIGNMENT, "z");

        pkbDE.addModifiesS(1, ASSIGNMENT, "a");
        pkbDE.addModifiesS(2, READ, "b");
        pkbDE.addModifiesS(8, ASSIGNMENT, "s");
        pkbDE.addModifiesS(12, IF, "big");
        pkbDE.addModifiesS(12, IF, "heavy");
        pkbDE.addModifiesS(12, IF, "feather");

        // procedure second
        pkbDE.addUsesS(9, ASSIGNMENT, "g");
        pkbDE.addUsesS(9, ASSIGNMENT, "h");
        pkbDE.addUsesS(10, PRINT, "w");
        pkbDE.addUsesS(11, PRINT, "x");
        pkbDE.addUsesS(12, IF, "y");
        pkbDE.addUsesS(12, IF, "e");
        pkbDE.addUsesS(12, IF, "tough");
        pkbDE.addUsesS(12, IF, "light");
        pkbDE.addUsesS(12, IF, "heavy");
        pkbDE.addUsesS(12, IF, "bird");
        pkbDE.addUsesS(12, IF, "wings");
        pkbDE.addUsesS(12, IF, "d");
        pkbDE.addUsesS(12, IF, "m");

        pkbDE.addUsesS(13, ASSIGNMENT, "tough");
        pkbDE.addUsesS(14, ASSIGNMENT, "light");
        pkbDE.addUsesS(14, ASSIGNMENT, "heavy");
        pkbDE.addUsesS(15, ASSIGNMENT, "bird");
        pkbDE.addUsesS(15, ASSIGNMENT, "wings");
        pkbDE.addUsesS(16, WHILE, "d");
        pkbDE.addUsesS(16, WHILE, "m");
        pkbDE.addUsesS(20, PRINT, "assignments");

        pkbDE.addModifiesS(18, READ, "f");
        pkbDE.addModifiesS(19, READ, "module");
        pkbDE.addModifiesS(9, ASSIGNMENT, "t");
        pkbDE.addModifiesS(13, ASSIGNMENT, "big");
        pkbDE.addModifiesS(14, ASSIGNMENT, "heavy");
        pkbDE.addModifiesS(15, ASSIGNMENT, "feather");


        // procedure third
        pkbDE.addUsesS(20, PRINT, "assignments");
        pkbDE.addModifiesS(19, READ, "module");


        de->populateDesignAbstractions();

        // retrieves all Uses(p,v) relationship and check
        vector<pair<string, string>> allUsesProcVar = pkbDE.getUsesPTT(NO_TYPE, NO_TYPE);
        vector<std::pair<string, string> > expectedUsesProcedures
            = { make_pair("first", "x"), make_pair("first", "y"), make_pair("first", "c"), make_pair("first", "xyz"),
            make_pair("first", "z"), make_pair("first", "k"), make_pair("first", "done"), make_pair("first", "g"), make_pair("first", "h"), make_pair("first", "w"), make_pair("first", "e"), make_pair("first", "tough"), make_pair("first", "light"), make_pair("first", "heavy"), make_pair("first", "bird"),make_pair("first", "wings"), make_pair("first", "d"), make_pair("first", "m"), make_pair("first", "assignments"), make_pair("second", "g"),make_pair("second", "h"), make_pair("second", "w"), make_pair("second", "x"), make_pair("second", "y"), make_pair("second", "e"), make_pair("second", "tough"), make_pair("second", "light"), make_pair("second", "heavy"), make_pair("second", "bird"), make_pair("second", "wings"), make_pair("second", "d"), make_pair("second", "m"), make_pair("second", "assignments"), make_pair("third", "assignments") };

        REQUIRE(allUsesProcVar.size() == expectedUsesProcedures.size());
        for (int i = 0; i < allUsesProcVar.size(); i++) {
            pair<string, string> innerPair = allUsesProcVar[i];
            bool isPresent = std::find(expectedUsesProcedures.begin(), expectedUsesProcedures.end(), innerPair)
                != expectedUsesProcedures.end();
            REQUIRE(isPresent);
        }


        // retrieves all Modifies(p,v) relationship and check
        vector<pair<string, string>> allModifiesProcVar = pkbDE.getModifiesPTT(NO_TYPE, NO_TYPE);
        vector<std::pair<string, string> > expectedModifiesProcedures
            = { make_pair("first", "a"), make_pair("first", "b"), make_pair("first", "s"), make_pair("first", "f"),
            make_pair("first", "t"), make_pair("first", "big"), make_pair("first", "heavy"),
            make_pair("first", "feather"),make_pair("first", "module"), make_pair("second", "f"), make_pair("second", "t"),
            make_pair("second", "big"), make_pair("second", "heavy"), make_pair("second", "feather"), make_pair("second", "module"), make_pair("third", "module") };

        REQUIRE(allModifiesProcVar.size() == expectedModifiesProcedures.size());
        for (int i = 0; i < allModifiesProcVar.size(); i++) {
            pair<string, string> innerPair = allModifiesProcVar[i];
            bool isPresent = std::find(expectedModifiesProcedures.begin(), expectedModifiesProcedures.end(), innerPair)
                != expectedModifiesProcedures.end();
            REQUIRE(isPresent);
        }

        // retrieves all Uses(c,v) and check

        vector<pair<int, string>> allUsesCallVar = pkbDE.getUsesSTT(CALL, NO_TYPE);
        vector<std::pair<int, string> > expectedUsesCalls
            = { make_pair(7, "g"), make_pair(7, "h"), make_pair(7, "w"),make_pair(7, "x"), make_pair(7, "y"),
            make_pair(7, "e"), make_pair(7, "tough"), make_pair(7, "light"), make_pair(7, "heavy"),
            make_pair(7, "bird"),make_pair(7, "wings"), make_pair(7, "d"), make_pair(7, "m"),
            make_pair(7, "assignments"), make_pair(17, "assignments") };

        REQUIRE(allUsesCallVar.size() == expectedUsesCalls.size());
        for (int i = 0; i < allUsesCallVar.size(); i++) {
            pair<int, string> innerPair = allUsesCallVar[i];
            bool isPresent = std::find(expectedUsesCalls.begin(), expectedUsesCalls.end(), innerPair)
                != expectedUsesCalls.end();
            REQUIRE(isPresent);
        }

        // retrieves all Modifies(c,v) and check
        vector<pair<int, string>> allModifiesCallVar = pkbDE.getModifiesSTT(CALL, NO_TYPE);
        vector<std::pair<int, string> > expectedModifiesCalls
            = { make_pair(7, "f"), make_pair(7, "t"), make_pair(7, "big"), make_pair(7, "heavy"),
            make_pair(7, "feather"), make_pair(7, "module"), make_pair(17, "module") };

        REQUIRE(allModifiesCallVar.size() == expectedModifiesCalls.size());
        for (int i = 0; i < allModifiesCallVar.size(); i++) {
            pair<int, string> innerPair = allModifiesCallVar[i];
            bool isPresent = std::find(expectedModifiesCalls.begin(), expectedModifiesCalls.end(), innerPair)
                != expectedModifiesCalls.end();
            REQUIRE(isPresent);
        }

        // check while in first procedure, should contain uses and modifies in second procedure

        // Retrieves uses to check
        vector<pair<int, string>> allUsesWhileVar = pkbDE.getUsesSTT(WHILE, NO_TYPE);
        vector<std::pair<int, string> > expectedUsesWhile
            = { make_pair(4, "x"), make_pair(4, "y"), make_pair(4, "k"),make_pair(4, "done"), make_pair(4, "g"),
            make_pair(4, "h"), make_pair(4, "w"), make_pair(4, "e"), make_pair(4, "tough"),
            make_pair(4, "light"), make_pair(4, "heavy"), make_pair(4,"bird"), make_pair(4, "wings"),
            make_pair(4, "d"), make_pair(4, "m"), make_pair(4, "assignments"), make_pair(16, "d"),
            make_pair(16, "m"), make_pair(16, "assignments") };

        REQUIRE(allUsesWhileVar.size() == expectedUsesWhile.size());
        for (int i = 0; i < allUsesWhileVar.size(); i++) {
            pair<int, string> innerPair = allUsesWhileVar[i];
            bool isPresent = std::find(expectedUsesWhile.begin(), expectedUsesWhile.end(), innerPair)
                != expectedUsesWhile.end();
            REQUIRE(isPresent);
        }

        // Retrieves modifies to check
        vector<pair<int, string>> allModifiesWhileVar = pkbDE.getModifiesSTT(WHILE, NO_TYPE);
        vector<std::pair<int, string> > expectedModifiesWhile
            = { make_pair(4, "f"), make_pair(4, "t"), make_pair(4, "big"),make_pair(4, "heavy"), make_pair(4, "feather"),
                make_pair(4, "module"), make_pair(16, "module") };

        REQUIRE(allModifiesWhileVar.size() == expectedModifiesWhile.size());
        for (int i = 0; i < allModifiesWhileVar.size(); i++) {
            pair<int, string> innerPair = allModifiesWhileVar[i];
            bool isPresent = std::find(expectedModifiesWhile.begin(), expectedModifiesWhile.end(), innerPair)
                != expectedModifiesWhile.end();
            REQUIRE(isPresent);
        }


    }
}

TEST_CASE("Assignment subexpr") {
    


}




