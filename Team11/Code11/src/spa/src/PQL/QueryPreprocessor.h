#ifndef AUTOTESTER_QUERYPREPROCESSOR_H
#define AUTOTESTER_QUERYPREPROCESSOR_H
#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include "Query.h"
#include "TreeProcessor.h"
#include "QueryClause.h"

class QueryPreprocessor {

private:
    enum class clauseType { suchThat, pattern, with, noType};
    static clauseType getClauseType(string clause);
    static bool splitQuery(std::string query);

public:
    /////////////// SET UP PARSING ///////////////
    static queue<string>* declarations;
    static string queryStmt;
    static bool addDeclaration(std::string declaration);
    static bool setQuery(std::string& queryStmt);

    /////////////// PARSING CLAUSES METHODS ///////////////
    static bool parse(std::string query);
    static bool parseDeclaration();
    static bool parseResultClause(std::string &query);
    static bool parseSelectVar(std::string& query);
    static bool parseClauses(std::string& query);

    /////////////// PARSING HELPER METHOD ///////////////
    static bool parseTuple(std::string& query);
    static bool parseNonTuple(std::string& query);
    static bool parseVarSelect(std::string name);
    static bool parseSuchThat(std::string& query);
    static bool parsePattern(std::string& query);
    static bool parseWith(std::string first, std::string second);
    static pair<string, string> parseWithContent(std::string& query);
    static bool parseAnd(std::string& query, clauseType prevClause);
    static vector<string> parseBracket(std::string query);

    static void processSuchThat(string &query);
    static void processPattern(string &query);
    static void processWith(string &query);
    static void processAnd(string &query, clauseType prevClause);

    static bool isAttrExist(string query);
    static pair<string, QueryClause::attribute> splitAttr(string attr);
    static bool resetQE();

    QueryPreprocessor();
};


#endif //AUTOTESTER_QUERYPREPROCESSOR_H
