
#ifndef AUTOTESTER_PKBSTUB_H
#define AUTOTESTER_PKBSTUB_H

#pragma once

#include "PKB/PKB.h"

/*
procedure simple {
1   p = 4;
2   a = p * 2;
3   if (c == k) then {
4      pattern = p * a * a + p * ifs + p * w;
5 	while(w > 0) {
6 		print c;
7 		w = w - 1;}
8 	ifs = ifs -1;}
else {
9   read p;
10  print pattern;}}
*/

unordered_map<int, string> expected_assignLHSMap = {
    {1,"4"}, {2,"a"}, {4,"pattern"}, {7,"w"}, {8,"ifs"}
};
unordered_map<string, int> expected_mAssignLHSMap = {
    {"4",1}, {"a",2}, {"pattern",4}, {"w",7}, {"ifs",8}
};
unordered_map<int, std::unordered_set<string>> expected_assignRHSMap = {
    {2,{"p"}}, {4,{"p","a","ifs","w"}}, {7,{"w"}}, {8,{"ifs"}}
};
unordered_map<int, std::unordered_set<int>> expected_assignConstMap = {
    {1,{4}}, {2,{2}}, {7,{1}}, {8,{1}}
};
unordered_map<int, vector<string>> expected_assignRHSMapVector = {
    {2,{"p"}}, {4,{"p","a","ifs","w"}}, {7,{"w"}}, {8,{"ifs"}}
};
std::unordered_set<int> expected_assignList = {
    1,2,4,7,8
};
std::unordered_map<int, string> expected_assignPatternMap = {
    {1,"4"}, {2,"p * 2"}, {4, "p * a * a + p * ifs + p * w"}, {7,"w - 1"}, {8,"ifs - 1"}
};
std::unordered_set<int> expected_ifList = {
    3
};
std::vector<int> expected_ifBlockList = {
    4,5,6,7,8
};
std::vector<int> expected_elseBlockList = {
    9,10
};
std::unordered_set<int> expected_whileList = {
    5
};
std::vector<int> expected_whileBlockList = {
    6,7
};

std::unordered_set<std::string> expected_procedureList = {
    "simple"
};
std::unordered_set<std::string> expected_varList = {
    "p","a","pattern","w","ifs","c","k"
};
std::unordered_set<int> expected_constList = {
    4,2,0,1
};

std::unordered_map<int, string> expected_readMap = {
    {9,"p"}
};
std::unordered_set<int> expected_readStmtList = {
    9
};

std::unordered_map<int, string> expected_printMap = {
    {6,"c"}, {10, "pattern"}
};
std::unordered_set<int> expected_printStmtList = {
    6,10
};


/*
 pkb.addProcedure("simple");

 pkb.addVar("p");
 pkb.addVar("a");
 pkb.addVar("w");
 pkb.addVar("pattern");
 pkb.addVar("ifs");
 pkb.addVar("c");
 pkb.addVar("k");

 pkb.addConst(2);
 pkb.addConst(0);
 pkb.addConst(-1);

 pkb.addAssignLHSVar("p");
 pkb.addAssignLHSVar("a");
 pkb.addAssignLHSVar("pattern");
 pkb.addAssignLHSVar("w");
 pkb.addAssignLHSVar("ifs");

 pkb.addAssignPattern("4");
 pkb.addAssignPattern("p*2");
 pkb.addAssignPattern("p*a*a+p*ifs+p*w");
 pkb.addAssignPattern("w-1");
 pkb.addAssignPattern("ifs-1");
 
 pkb.addAssignRHSVar(2, "p");
 pkb.addAssignRHSVar(4, "p");
 pkb.addAssignRHSVar(4, "a");
 pkb.addAssignRHSVar(4, "ifs");
 pkb.addAssignRHSVar(4, "w");
 pkb.addAssignRHSVar(7, "w");
 pkb.addAssignRHSVar(8, "ifs");

 pkb.addAssignRHSConst(1, 4);
 pkb.addAssignRHSConst(2, 2);
 pkb.addAssignRHSConst(7, 1);
 pkb.addAssignRHSConst(8, 1);
 
 pkb.addReadStmt(9, "p")

 pkb.addPrintStmt(10, "pattern");
 pkb.addPrintStmt(6, "c");

 pkb.addIfStmt(3);
 pkb.addIfStmt(8);

 pkb.addIfBlockStmt(3, 4);
 pkb.addIfBlockStmt(3, 5);
 pkb.addIfBlockStmt(3, 6);
 pkb.addIfBlockStmt(3, 7);
 pkb.addIfBlockStmt(3, 8);

 pkb.addElseBlockStmt(3, 9);
 pkb.addElseBlockStmt(3, 10);

 pkb.addWhileStmt(5);

 pkb.addWhileBlockStmt(5, 6);
 pkb.addWhileBlockStmt(5, 7);

 pkb.addFollows(1, ASSIGNMENT, 2, ASSIGNMENT);
 pkb.addFollows(2, ASSIGNMENT, 3, IF);
 pkb.addFollows(4, ASSIGNMENT, 5, WHILE);
 pkb.addFollows(6, CALL, 7, ASSIGNMENT);
 pkb.addFollows(9, READ, 10, PRINT);

 pkb.addFollowsStar(1, ASSIGNMENT, 2, ASSIGNMENT);
 pkb.addFollowsStar(1, ASSIGNMENT, 3, IF);
 pkb.addFollowsStar(2, ASSIGNMENT, 3, IF);
 pkb.addFollowsStar(4, ASSIGNMENT, 5, WHILE);
 pkb.addFollowsStar(4, ASSIGNMENT, 8, ASSIGNMENT);
 pkb.addFollowsStar(6, CALL, 7, ASSIGNMENT);
 pkb.addFollowsStar(9, READ, 10, PRINT);
 
 pkb.addParent(3, IF, 4, ASSIGNMENT);
 pkb.addParent(3, IF, 5, WHILE);
 pkb.addParent(3, IF, 8, ASSIGNMENT);
 pkb.addParent(3, IF, 9, READ);
 pkb.addParent(3, IF, 10, PRINT);
 pkb.addParent(5, WHILE, 6, CALL);
 pkb.addParent(5, WHILE, 7, ASSIGNMENT);

 pkb.addParentStar(3, IF, 4, ASSIGNMENT);
 pkb.addParentStar(3, IF, 5, WHILE);
 pkb.addParentStar(3, IF, 6, CALL);
 pkb.addParentStar(3, IF, 7, ASSIGNMENT);
 pkb.addParentStar(3, IF, 8, ASSIGNMENT);
 pkb.addParentStar(3, IF, 9, READ);
 pkb.addParentStar(3, IF, 10, PRINT);
 pkb.addParentStar(5, WHILE, 6, CALL);
 pkb.addParentStar(5, WHILE, 7, ASSIGNMENT);
 
 pkb.addUsesS(2, ASSIGNMENT, "p");
 pkb.addUsesS(3, IF, "c");
 pkb.addUsesS(3, IF, "k");
 pkb.addUsesS(4, ASSIGNMENT, "p");
 pkb.addUsesS(4, ASSIGNMENT, "a");
 pkb.addUsesS(4, ASSIGNMENT, "ifs");
 pkb.addUsesS(4, ASSIGNMENT, "p");
 pkb.addUsesS(4, ASSIGNMENT, "w");
 pkb.addUsesS(5, WHILE, "w");
 pkb.addUsesS(6, PRINT, "c");
 pkb.addUsesS(7, ASSIGNMENT, "w");
 pkb.addUsesS(5, WHILE, "ifs");
 pkb.addUsesS(9, PRINT, "pattern");

 pkb.addUsesP("simple", ASSIGNMENT, "p");
 pkb.addUsesP("simple", IF, "c");
 pkb.addUsesP("simple", PRINT, "c");
 pkb.addUsesP("simple", IF, "k");
 pkb.addUsesP("simple", ASSIGNMENT, "a");
 pkb.addUsesP("simple", ASSIGNMENT, "ifs");
 pkb.addUsesP("simple", ASSIGNMENT, "w");
 pkb.addUsesP("simple", PRINT, "pattern");
 
 pkb.addModifiesS(1, ASSIGNMENT, "p");
 pkb.addModifiesS(2, ASSIGNMENT, "a");
 pkb.addModifiesS(4, ASSIGNMENT, "pattern");
 pkb.addModifiesS(7, ASSIGNMENT, "w");
 pkb.addModifiesS(8, ASSIGNMENT, "ifs");
 pkb.addModifiesS(9, READ, "p");
 
 pkb.addModifiesP("simple", ASSIGNMENT, "p");
 pkb.addModifiesP("simple", ASSIGNMENT, "a");
 pkb.addModifiesP("simple", ASSIGNMENT, "pattern");
 pkb.addModifiesP("simple", ASSIGNMENT, "w"));
 pkb.addModifiesP("simple", ASSIGNMENT, "ifs");
 pkb.addModifiesP("simple", READ, "p");
 
 */


#endif //AUTOTESTER_PKBSTUB_H
