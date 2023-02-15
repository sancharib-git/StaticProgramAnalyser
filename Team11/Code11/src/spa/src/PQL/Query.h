#ifndef AUTOTESTER_QUERY_H
#define AUTOTESTER_QUERY_H
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include <list>
#include <tuple>
#include "IntermediateResult.h"
#include "DoubleColumnResult.h"
#include "PKB/PKB.h"
#include "QueryResultProjector.h"
#include "QueryElement.h"
#include "QueryClause.h"

struct Query {
public:
    static Query *instance;

    static Query& getInstance() {
        if (!(instance)) {
            instance = new Query();
        }
        return *instance;
    };

    enum class entityType { procedure, stmt, assign, whiles, var, constant, print, call, ifl, read, prog_line, wrong_type, no_type };
    enum class selectFlag { isSynonymName, isBoolean, isProcName, isVarName, isInvalid };
    static entityType getEntityType(string entityType);
    entityType getEntityTypeBySynName(string synonym);
    QueryClause::suchThatFlag getSuchThatFlagFromString(string arg);
    QueryClause::withFlag getWithFlagFromAttribute(QueryClause::attribute attr);
    QueryClause::withFlag getWithFlagFromString(string arg);


    // for QPP to add declaration synonyms to QE
    bool addDeclarationSynonym(string synonymName, entityType type);

    // from string of declaration, get the type
    entityType getDeclarationType(string synonym);

    // set synonym name used in current query to be selected
    bool setSelectSynonym(std::string synonymName);
    bool addSelectSynonym(string synonymName, selectFlag flag);
    // new addSuchThat order served as requested
    bool addSuchThat(QueryClause::relationshipType rType, string arg1, QueryClause::suchThatFlag leftFlag, string arg2, QueryClause::suchThatFlag rightFlag);
    // called by QPP to add pattern - including the various pattern flags
    bool addPattern(string stmt_synonym, string lhs, QueryClause::patternFlag leftFlag, string rhs, QueryClause::patternFlag rightFlag);
    // TODO: decide on type of refType and implement
    bool addWith(string lhs, QueryClause::withFlag leftFlag, string rhs, QueryClause::withFlag rightFlag);
    // called by QPP - when query is invalid
    bool setInvalidQuery();
    bool setSyntaxError() {
        _isSyntaxError = true;
        return true;
    }
    bool setSemanticError() {
        _isSemanticError = true;
        return true;
    }
    bool setAsOptimised() {
        _isOptimised = true;
        return true;
    }
    bool isQueryValid();
    bool isSyntaxError() {
        return _isSyntaxError;
    }
    bool isSemanticError() {
        return _isSemanticError;
    }
    bool isOptimised() {
        return _isOptimised;
    }

    bool reset();

    // used by QPP to check that whatever var used in query is declared previously/duplicated ones
    bool isVarDeclared(string var);

    // for each declaration, get the statement type
    StatementTypes getStatementTypeOfDec(string arg);

    bool isBooleanResult() {
        return (selects.size() == 1 && selects.front().second == selectFlag::isBoolean);
    }

    bool isValidQuery = true;
    bool _isSemanticError = false;
    bool _isSyntaxError = false;
    bool _isOptimised = false;

    // stores all the current declarations made so far, declaration name and type
    unordered_map<std::string, entityType> declarations;
    unordered_set<string> usedDeclarations;
    // stores all the declarations name/string only
    vector<string> declarationList;
    vector<pair<string, selectFlag>> selects;
    vector<QueryClause> clauses;
};



#endif //AUTOTESTER_QUERY_H