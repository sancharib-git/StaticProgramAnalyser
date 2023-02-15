//
// Created by Xuan Zhi on 15/9/21.
//
#include <stdio.h>
#include <string>
#include <utility>
#include "catch.hpp"
#include "PQL/Query.h"
#include "PQL/QueryPreprocessor.h"

using namespace std;

string assign = "assign assign";
string assign1 = "assign a";
string var = "variable v";
string whiles = "while w";
string ifs = "if ifs";
string w = "while w1";
string print = "print p";
string stmt = "stmt s";
string many = "assign a1, a2, a3";
string wrongType = "var v1";
string wrongName1 = "assign 12a";
string wrongName2 = "stmt $tmt";
string procedure = "procedure proc";
string call = "call call";
string constant = "constant c";
string prog_line = "prog_line n1";

/*
TEST_CASE("parseDeclaration") {
    QueryPreprocessor::resetQE();
    SECTION("add correct declarations") {
        QueryPreprocessor::declarations = new queue<string>();
        QueryPreprocessor::declarations->push(assign1);
        QueryPreprocessor::declarations->push(var);
        QueryPreprocessor::declarations->push(whiles);
        QueryPreprocessor::declarations->push(print);
        QueryPreprocessor::declarations->push(stmt);
        QueryPreprocessor::parseDeclaration();
        REQUIRE(QueryPreprocessor::parseDeclaration());
    }

    SECTION("multiple declarations in the same line") {
        QueryPreprocessor::declarations = new queue<string>();
        QueryPreprocessor::declarations->push(many);
        QueryPreprocessor::declarations->push(w);
        REQUIRE(QueryPreprocessor::parseDeclaration());
    }

    SECTION("wrong type") {
        QueryPreprocessor::declarations = new queue<string>();
        QueryPreprocessor::declarations->push(assign);
        QueryPreprocessor::declarations->push(wrongType);
        REQUIRE_FALSE(QueryPreprocessor::parseDeclaration());
    }
    SECTION("wrong name") {
        QueryPreprocessor::declarations = new queue<string>();
        QueryPreprocessor::declarations->push(wrongName1);
        REQUIRE_FALSE(QueryPreprocessor::parseDeclaration());
        QueryPreprocessor::declarations->push(wrongName2);
        REQUIRE_FALSE(QueryPreprocessor::parseDeclaration());
        QueryPreprocessor::declarations->push("while ");
        REQUIRE_FALSE(QueryPreprocessor::parseDeclaration());
    }
}

TEST_CASE("parse select variable") {
    QueryPreprocessor::resetQE();
    QueryPreprocessor::declarations = new queue<string>();
    QueryPreprocessor::declarations->push(assign);
    QueryPreprocessor::declarations->push(var);
    QueryPreprocessor::declarations->push(whiles);
    QueryPreprocessor::declarations->push(print);
    QueryPreprocessor::declarations->push(procedure);
    QueryPreprocessor::declarations->push(call);
    QueryPreprocessor::declarations->push(constant);
    QueryPreprocessor::declarations->push(stmt);
    QueryPreprocessor::parseDeclaration();

    SECTION("existing variables") {
        string selectQuery1 = "Select assign";
        string selectQuery2 = "Select v";
        string selectQuery3 = "Select w";
        string selectQuery4 = "Select            assign   ";
        string selectQuery5 = "Select s";
        string selectQuery6 = "Select assign.stmt# ";
        string selectQuery7 = "Select c.value ";
        string selectQuery8 = "Select proc.procName ";
        string selectQuery9 = "Select v.varName";
        string selectQuery10 = "Select p.varName";
        string selectQuery11 = "Select BOOLEAN";
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery1));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery2));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery3));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery4));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery5));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery6));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery7));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery8));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery9));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery10));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery11));
    }

    SECTION("tuples") {
        string selectQuery1 = "Select <s, w>";
        string selectQuery2 = "Select < v    >";
        string selectQuery3 = "Select BOOLEAN";
        string selectQuery4 = "Select            <s, w, assign, v>   ";
        string selectQuery5 = "Select <w, assign, v>";
        string selectQuery6 = "Select <w, w.stmt#, proc.procName, call.procName    >";

        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery1));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery2));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery3));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery4));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery5));
        REQUIRE(QueryPreprocessor::parseSelectVar(selectQuery6));

    }

    SECTION("synonyms not declared") {
        string selectQuery1 = "Select a such that";
        string selectQuery2 = "Select ";
        string selectQuery3 = "Select varIable";
        REQUIRE_FALSE(QueryPreprocessor::parseSelectVar(selectQuery1));
        REQUIRE_FALSE(QueryPreprocessor::parseSelectVar(selectQuery2));
        REQUIRE_FALSE(QueryPreprocessor::parseSelectVar(selectQuery3));
    }

    SECTION("invalid attributes") {
        string selectQuery1 = "Select <s.varName, w>";
        string selectQuery2 = "Select < v.value    >";
        string selectQuery3 = "Select assign.procName";
        string selectQuery4 = "Select            <s, w, assign, v.stmt#>   ";
        string selectQuery5 = "Select <w.value, assign, v>";
        string selectQuery6 = "Select <w, w.stmt#, proc.procName,  c.procName    >";

        REQUIRE_FALSE(QueryPreprocessor::parseSelectVar(selectQuery1));
        REQUIRE_FALSE(QueryPreprocessor::parseSelectVar(selectQuery2));
        REQUIRE_FALSE(QueryPreprocessor::parseSelectVar(selectQuery3));
        REQUIRE_FALSE(QueryPreprocessor::parseSelectVar(selectQuery4));
        REQUIRE_FALSE(QueryPreprocessor::parseSelectVar(selectQuery5));
        REQUIRE_FALSE(QueryPreprocessor::parseSelectVar(selectQuery6));
    }
}

TEST_CASE("parseBracket") {
    QueryPreprocessor::resetQE();
    string bracket1 = "w, a";
    string bracket2 = "w,         a";
    string bracket3 = "a, w       ";
    string bracket4 = "a,w";
    vector<string> result1 = QueryPreprocessor::parseBracket(bracket1);
    REQUIRE(result1.at(0) == "w");
    REQUIRE(result1.at(1) == "a");
    vector<string> result2 = QueryPreprocessor::parseBracket(bracket2);
    REQUIRE(result2.at(0) == "w");
    REQUIRE(result2.at(1) == "a");
    vector<string> result3 = QueryPreprocessor::parseBracket(bracket3);
    REQUIRE(result3.at(0) == "a");
    REQUIRE(result3.at(1) == "w");
    vector<string> result4 = QueryPreprocessor::parseBracket(bracket4);
    REQUIRE(result4.at(0) == "a");
    REQUIRE(result4.at(1) == "w");
}

TEST_CASE("parse such that") {
    QueryPreprocessor::resetQE();
    QueryPreprocessor::declarations = new queue<string>();
    QueryPreprocessor::declarations->push(assign);
    QueryPreprocessor::declarations->push(assign1);
    QueryPreprocessor::declarations->push(var);
    QueryPreprocessor::declarations->push(whiles);
    QueryPreprocessor::declarations->push(print);
    QueryPreprocessor::declarations->push(stmt);
    QueryPreprocessor::declarations->push(procedure);
    QueryPreprocessor::declarations->push(call);
    QueryPreprocessor::declarations->push(prog_line);
    QueryPreprocessor::declarations->push(constant);
    QueryPreprocessor::parseDeclaration();

    SECTION("valid such that") {
        string parent = "Parent (w, assign)";
        string parentStar = "Parent* (s, assign)";
        string follow = "Follows (1, w)";
        string followStar = "Follows* (s, 3)";
        string modifies = "Modifies (2, \"v\")";
        string uses = "Uses (w, _)";
        string next = "Next(n1, 5)";
        string nextStar = "Next* (_, n1)";
        string affects = "Affects (assign, a)";
        string affectStar = "Affects* (assign, _)";
        string calls = "Calls (\"trying\", \"   jiayous  \")";

        REQUIRE(QueryPreprocessor::parseSuchThat(parent));
        REQUIRE(QueryPreprocessor::parseSuchThat(parentStar));
        REQUIRE(QueryPreprocessor::parseSuchThat(follow));
        REQUIRE(QueryPreprocessor::parseSuchThat(followStar));
        REQUIRE(QueryPreprocessor::parseSuchThat(modifies));
        REQUIRE(QueryPreprocessor::parseSuchThat(uses));
        REQUIRE(QueryPreprocessor::parseSuchThat(next));
        REQUIRE(QueryPreprocessor::parseSuchThat(nextStar));
        REQUIRE(QueryPreprocessor::parseSuchThat(affects));
        REQUIRE(QueryPreprocessor::parseSuchThat(affectStar));
        REQUIRE(QueryPreprocessor::parseSuchThat(calls));
    }

    SECTION("invalid relationships") {
        string parent = "Parents (w, assign)";
        string parentStar = "Parents* (s, assign)";
        string follow = "Follow (1, w)";
        string followStar = "Follow* (s, 3)";
        string modifies = "Modify (2, \"v\")";
        string uses = "Use (w, _)";
        string noSpace = "Parents(w, assign)";

        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(parent));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(parentStar));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(follow));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(followStar));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(modifies));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(uses));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(noSpace));
    }

    SECTION("insufficient arguments") {
        string parent = "Parent (w, )";
        string parentStar = "Parent* (, assign)";
        string follow = "Follows (, )";
        string followStar = "Follows* ()";
        string modifies = "Modifies (,)";
        string uses = "Uses (w,)";

        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(parent));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(parentStar));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(follow));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(followStar));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(modifies));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(uses));
    }

    SECTION("invalid arguments") {
        string parent = "Parent (w, \"_\")";
        string parentStar = "Parent* (assign, whiles)";
        string follow = "Follows (w, 0012)";
        string followStar = "Follows* (\"w\", _)";
        string modifies = "Modifies (_, \"count\")";
        string uses = "Uses (w, 12)";
        string affects = "Affects (w, s)";
        string next = "Next* (v, w)";
        string call = "Calls* (v, proc)";

        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(parent));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(parentStar));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(follow));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(followStar));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(modifies));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(uses));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(affects));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(next));
        REQUIRE_FALSE(QueryPreprocessor::parseSuchThat(call));
    }
}

TEST_CASE("parse pattern") {
    QueryPreprocessor::resetQE();
    QueryPreprocessor::declarations = new queue<string>();
    QueryPreprocessor::declarations->push(assign);
    QueryPreprocessor::declarations->push(var);
    QueryPreprocessor::declarations->push(whiles);
    QueryPreprocessor::declarations->push(ifs);
    QueryPreprocessor::declarations->push(print);
    QueryPreprocessor::declarations->push(stmt);
    QueryPreprocessor::parseDeclaration();

    SECTION("valid pattern") {
        string pattern1 = "assign (v, \"x\")";
        string pattern2 = "assign (_, \"    x   \")";
        string pattern3 = "assign (_, _)";
        string pattern4 = "assign (\"w\", _)";
        string pattern5 = "assign (v, _\"x\"_)";
        string pattern6 = "assign (\"w\", _\"x\"_)";
        string pattern7 = "ifs(v, _, _)";
        string pattern8 = "ifs (_, _, _)";
        string pattern9 = "w(_,     _)";
        string pattern10 = "w (v, _)";
        string pattern11 = "w (\"x\" , _)";
        string pattern12 = "w (_, _)";
        string pattern13 = "ifs (\"mehhh\", _, _)";

        REQUIRE(QueryPreprocessor::parsePattern(pattern1));
        REQUIRE(QueryPreprocessor::parsePattern(pattern2));
        REQUIRE(QueryPreprocessor::parsePattern(pattern3));
        REQUIRE(QueryPreprocessor::parsePattern(pattern4));
        REQUIRE(QueryPreprocessor::parsePattern(pattern5));
        REQUIRE(QueryPreprocessor::parsePattern(pattern6));
        REQUIRE(QueryPreprocessor::parsePattern(pattern7));
        REQUIRE(QueryPreprocessor::parsePattern(pattern8));
        REQUIRE(QueryPreprocessor::parsePattern(pattern9));
        REQUIRE(QueryPreprocessor::parsePattern(pattern10));
        REQUIRE(QueryPreprocessor::parsePattern(pattern11));
        REQUIRE(QueryPreprocessor::parsePattern(pattern12));
        REQUIRE(QueryPreprocessor::parsePattern(pattern13));
    }

    SECTION("invalid patterns or arguments") {
        string pattern1 = "assign (v, x\")";
        string pattern2 = "assign (_, \"x\"_)";
        string pattern3 = "assign (_, v)";
        string pattern4 = "assign (\"3\", _)";
        string pattern5 = "assign (w, _\"x\")";
        string pattern6 = "a (_, _)";
        string pattern7 = "a (\"w, _)";
        string pattern8 = "ifs (v, _)";
        string pattern9 = "if (_, _, _)";
        string pattern10 = "ifs(\"3\", _, _)";
        string pattern11 = "w (_, _, _)";
        string pattern12 = "w (_, v)";
        string pattern13 = "w(ifs, _)";

        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern1));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern2));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern3));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern4));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern5));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern6));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern7));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern8));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern9));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern10));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern11));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern12));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern13));
    }

    SECTION("insufficient arguments") {
        string pattern1 = "assign (, \"x\")";
        string pattern2 = "assign (_, )";
        string pattern3 = "a (,)";
        string pattern4 = "assign (,_)";
        string pattern5 = "assign (_\"x\"_)";
        string pattern6 = "ifs (_, _)";
        string pattern7 = "ifs (, _, _)";
        string pattern8 = "w(,)";
        string pattern9 = "w (_,)";

        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern1));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern2));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern3));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern4));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern5));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern6));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern7));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern8));
        REQUIRE_FALSE(QueryPreprocessor::parsePattern(pattern9));
    }
}


TEST_CASE("parse clause") {
    QueryPreprocessor::resetQE();
    QueryPreprocessor::declarations->push(assign);
    QueryPreprocessor::declarations->push(assign1);
    QueryPreprocessor::declarations->push(var);
    QueryPreprocessor::declarations->push(whiles);
    QueryPreprocessor::declarations->push(print);
    QueryPreprocessor::declarations->push(stmt);
    QueryPreprocessor::declarations->push(procedure);
    QueryPreprocessor::declarations->push(call);
    QueryPreprocessor::declarations->push(prog_line);
    QueryPreprocessor::declarations->push(constant);
    QueryPreprocessor::declarations->push(ifs);
    QueryPreprocessor::parseDeclaration();

    SECTION("correct queries") {
        string query1 = "Select assign";
        string query2 = "Select     v";
        string query3 = "Select w such that Parent(w, s)";
        string query4 = "Select s such that Parent*(      w  , assign)";
        string query5 = "Select assign pattern assign (v, _\"x\"_)";
        string query6 = "Select w such that Follows (w, s) pattern assign(_ ,_\"x\"_)";
        string query7 = "Select w pattern assign(_, _\"(x + y *   z)\"_) such that Follows (w, s)";
        string query8 = "Select <assign, v, w>";
        string query9 = "Select BOOLEAN such that Follows (1, 2) and Parent (3, 4) and Next (4, n1)";
        string query10 = "Select <assign.stmt#, proc.procName, v.varName> such that Next (s, n1) and Calls (\"hello\", \"bye\") pattern ifs (_, _, _) and pattern assign (\"extra\", _) with assign.stmt# = c.value";
        string query11 = "Select <p.varName, p.stmt#> such that Parent* (p, _) and Affects(assign, assign)";
        string query12 = "Select v pattern assign (_, \"x + y\") and pattern assign (v, _\"(x + y) * z\"_) with assign.stmt# = 12";
        string query13 = "Select assign.stmt# pattern assign (v, _\"y * z -g\"_) and pattern assign (v, \"x\") and pattern assign (_, _)";
//        QueryPreprocessor::parseClauses(query10);

        REQUIRE(QueryPreprocessor::parseClauses(query1));
        REQUIRE(QueryPreprocessor::parseClauses(query2));
        REQUIRE(QueryPreprocessor::parseClauses(query3));
        REQUIRE(QueryPreprocessor::parseClauses(query4));
        REQUIRE(QueryPreprocessor::parseClauses(query5));
        REQUIRE(QueryPreprocessor::parseClauses(query6));
        REQUIRE(QueryPreprocessor::parseClauses(query7));
        REQUIRE(QueryPreprocessor::parseClauses(query8));
        REQUIRE(QueryPreprocessor::parseClauses(query9));
        REQUIRE(QueryPreprocessor::parseClauses(query10));
        REQUIRE(QueryPreprocessor::parseClauses(query11));
        REQUIRE(QueryPreprocessor::parseClauses(query12));
        REQUIRE(QueryPreprocessor::parseClauses(query13));
    }

    SECTION("wrong select or var") {
        string query1 = "Selects v";
        string query2 = "Select v1";
        string query3 = "select w such that Parent (w, s)";
        string query4 = "Select such that Parent (w, s)";
        string query5 = "Select <p.procName> such that Parent (p, s)";
        string query6 = "Select assign such that Affects (p, assign)";
        string query7 = "Select proc.varName such that Calls (proc, proc)";
        string query8 = "Select s.value";
        string query9 = "Select n1 such that Next* (proc, n1)";
        string query10 = "Select <v.procName>";

        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query1));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query2));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query3));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query4));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query5));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query6));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query7));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query8));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query9));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query10));
    }

    SECTION("wrong types in arguments") {
        string query1 = "Selects v such that Follows (v, _)";
        string query2 = "Select assign pattern assign(w, \"x\")";
        string query3 = "Select w pattern assign(_, w)";
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query1));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query2));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query3));
    }

    SECTION("single clause faults") {
        string query1 = "Select assign Parent (w, s)";
        string query2 = "Select v patterns (w, s)";
        string query3 = "Select w such that Parents (w, s)";
        string query4 = "Select s such that Parent* (w, \"assign\")";
        string query5 = "Select assign pattern assign (v, \"x\"_)";
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query1));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query2));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query3));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query4));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query5));
    }

    SECTION("double clause faults") {
        string query1 = "Select assign pattern (w, _) such that Parent (w, s)";
        string query2 = "Select v patterns (w, s)";
        string query3 = "Select w such that Parents (w, s)";
        string query4 = "Select s such that Modifies (_, _) pattern assign (_, _)";
        string query5 = "Select assign pattern assign (v, _\"x\"_) Parent (w, assign)";

        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query1));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query2));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query3));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query4));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query5));
    }

    SECTION("multiple clauses madness") {
        string query1 = "Select assign pattern (w, _) and Parent (w, s)";
        string query2 = "Select v pattern (w, s) with Parent (w, s)";
        string query3 = "Select w such that Parents (w, s) and assign (_, _)";
        string query4 = "Select s such that Modifies (_, _) and s.stmt# = 12";
        string query5 = "Select assign with w.stmt# = pattern assign (v, _\"x\"_) Parent (w, assign)";

        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query1));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query2));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query3));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query4));
        REQUIRE_FALSE(QueryPreprocessor::parseClauses(query5));
    }
}

TEST_CASE("sub expressions") {

    QueryPreprocessor::resetQE();
    QueryPreprocessor::declarations = new queue<string>();
    QueryPreprocessor::declarations->push(assign);
    QueryPreprocessor::declarations->push(var);
    QueryPreprocessor::declarations->push(whiles);
    QueryPreprocessor::declarations->push(print);
    QueryPreprocessor::declarations->push(stmt);
    QueryPreprocessor::parseDeclaration();

    string subexpr1 = "assign(_, \"(x + helium)\")";
    string subexpr2 = "assign(_, \"(x+y)*z\")";
    string subexpr3 = "assign(_, \"(x-5+y*z/j-8*t)*g*z-(y*v)/(6-((h*(k-s))/(d*i)-(a/2)))\")";
    string subexpr4 = "assign(_, \"(x-5+y*z/j-8*t)*g*z-(y+v)/(6-((h*(k-s))/(d*i)-(a/2)))\")";
    string subexpr5 = "assign(_, \"(x-5+y*z/jump-8*t)*g*z-(y*v)/(6-((h*(k-s))/(d*i)-(a/2)))\")";
    string subexpr6 = "assign(_, \"(b+c*(b-q/(abc%d)))/e%a\")";
    string subexpr7 = "assign(_, \"p * (((r - t)) + b * pop / (dar - (pap - dab)))\")";
    string subexpr8 = "assign(_, \"x+(y*zposter/b)+tester+2\")";
    //string subexpr1 = "assign(_, \"(x + helium)\")";

    REQUIRE(QueryPreprocessor::parsePattern(subexpr1));
    REQUIRE(QueryPreprocessor::parsePattern(subexpr2));
    REQUIRE(QueryPreprocessor::parsePattern(subexpr3));
    REQUIRE(QueryPreprocessor::parsePattern(subexpr4));
    REQUIRE(QueryPreprocessor::parsePattern(subexpr5));
    REQUIRE(QueryPreprocessor::parsePattern(subexpr6));
    REQUIRE(QueryPreprocessor::parsePattern(subexpr7));
    REQUIRE(QueryPreprocessor::parsePattern(subexpr8));
}*/