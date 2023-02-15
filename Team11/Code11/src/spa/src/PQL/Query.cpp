#include "PKB/PKB.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include "Query.h"
#include "SetProcessor.h"

Query *Query::instance = 0;

///////////////////////////// Helper functions /////////////////////////////
bool isInteger(std::string s) {
    std::string::iterator it;
    bool isConst = true;
    for (it = s.begin(); it < s.end() && isConst; it++) {
        isConst = isConst && isdigit(*it);
    }
    return isConst;
}

///////////////////////////// Used by QPP and QE /////////////////////////////

QueryClause::suchThatFlag Query::getSuchThatFlagFromString(string arg) {
    if (arg == "_") {
        return QueryClause::suchThatFlag::isAll;
    }
    if (isInteger(arg)) {
        return QueryClause::suchThatFlag::isNumber;
    }
    //TODO: check if it starts and end with "
    if (arg.find("\"") != std::string::npos) {
        return QueryClause::suchThatFlag::isString;
    }
    if (Query::getInstance().isVarDeclared(arg)) {
        return QueryClause::suchThatFlag::isDeclaration;
    }
    return QueryClause::suchThatFlag::isNone;
}

StatementTypes Query::getStatementTypeOfDec(string arg) {
    if (arg == "_") {
        return NO_TYPE;
    }
    if (declarations.find(arg) == declarations.end()) {
        return ERROR;
    }
    switch(declarations.at(arg)) {
        case entityType::prog_line: {
            return NO_TYPE;
        }
        case entityType::stmt: {
            return NO_TYPE;
        }
        case entityType::procedure: {
            return NO_TYPE;
        }
        case entityType::var: {
            return NO_TYPE;
        }
        case entityType::assign: {
            return ASSIGNMENT;
        }
        case entityType::print: {
            return PRINT;
        }
        case entityType::read: {
            return READ;
        }
        case entityType::ifl: {
            return IF;
        }
        case entityType::whiles: {
            return WHILE;
        }
        case entityType::call: {
            return CALL;
        }
    }
    return ERROR;
}

bool Query::isVarDeclared(string var) {
    if (Query::declarations.find(var) != Query::declarations.end()) {
        // var is found
        return true;
    }
    return false;
}

///////////////////////////// USED BY QPP /////////////////////////////

bool Query::addDeclarationSynonym(std::string synonymName, entityType statementType) {
    if (declarations.find(synonymName) == declarations.end()) {
        declarationList.push_back(synonymName);
    }
    declarations.insert_or_assign(synonymName, statementType);
    return true;
}

bool Query::addSelectSynonym(string synonymName, Query::selectFlag flag) {
    if (flag != selectFlag::isBoolean) {
        usedDeclarations.insert(synonymName);
    }
    selects.emplace_back(make_pair(synonymName, flag));
    return true;
}

bool Query::addSuchThat(QueryClause::relationshipType rType, string arg1, QueryClause::suchThatFlag flag1,
                                 string arg2, QueryClause::suchThatFlag flag2) {
    if (flag1 == QueryClause::suchThatFlag::isDeclaration) {
        usedDeclarations.insert(arg1);
    }
    if (flag2 == QueryClause::suchThatFlag::isDeclaration) {
        usedDeclarations.insert(arg2);
    }
    clauses.push_back(QueryClause(QueryClause::suchThat(rType, arg1, flag1, arg2, flag2)));
    return true;
}

bool Query::addWith(string lhs, QueryClause::withFlag attr1, string rhs, QueryClause::withFlag attr3) {
    if (attr1 != QueryClause::withFlag::isString && attr1 != QueryClause::withFlag::isNumber) {
        usedDeclarations.insert(lhs);
    }
    if (attr3 != QueryClause::withFlag::isString && attr3 != QueryClause::withFlag::isNumber) {
        usedDeclarations.insert(rhs);
    }
    clauses.push_back(QueryClause(QueryClause::with(lhs, attr1, rhs, attr3)));
    return true;
}

bool Query::addPattern(string stmt_synonym, string lhs, QueryClause::patternFlag leftFlag, string rhs,
                                QueryClause::patternFlag rightFlag) {
    usedDeclarations.insert(stmt_synonym);
    if (leftFlag == QueryClause::patternFlag::isDeclaration) {
        usedDeclarations.insert(lhs);
    }
    clauses.push_back(QueryClause(QueryClause::pattern(stmt_synonym, lhs,rhs, leftFlag, rightFlag)));
    return true;
}

bool Query::setInvalidQuery() {
    //TODO: check if this is correct
    isValidQuery = false;
    return true;
}

bool Query::isQueryValid() {
    //TODO: check if this is correct
    return isValidQuery;
}

// TODO: add in intermediates
bool Query::reset() {
    isValidQuery = true;
    _isSemanticError = false;
    _isSyntaxError = false;
    declarations.clear();
    declarationList = vector<string>();
    selects = vector<pair<string, selectFlag>>();
    usedDeclarations = unordered_set<string>();
    clauses = {};
    _isOptimised = false;
    return true;
}

Query::entityType Query::getDeclarationType(string synonym) {
    if (declarations.find(synonym) != declarations.end()) {
        return declarations.at(synonym);
    }
    return entityType::wrong_type;
}

Query::entityType Query::getEntityType(string entityType) {
    if (entityType.compare("procedure") == 0) {
        return entityType::procedure;
    } else if (entityType.compare("stmt") == 0) {
        return entityType::stmt;
    } else if (entityType.compare("assign") == 0) {
        return entityType::assign;
    } else if (entityType.compare("while") == 0) {
        return entityType::whiles;
    } else if (entityType.compare("read") == 0) {
        return entityType::read;
    } else if (entityType.compare("call") == 0) {
        return entityType::call;
    } else if (entityType.compare("print") == 0) {
        return entityType::print;
    } else if (entityType.compare("constant") == 0) {
        return entityType::constant;
    } else if (entityType.compare("variable") == 0) {
        return entityType::var;
    } else if (entityType.compare("if") == 0) {
        return entityType::ifl;
    } else if (entityType.compare("prog_line") == 0) {
        return entityType::prog_line;
    } else if (entityType.compare("_") == 0) {
        return entityType::no_type;
    } else {
        return entityType::wrong_type;
    }
}

Query::entityType Query::getEntityTypeBySynName(string synonym) {
    // check that syn is declared before this
    if (declarations.find(synonym) == declarations.end()) {
        return Query::entityType::wrong_type;
    }
    entityType entityType = declarations.at(synonym);
    return entityType;
}

QueryClause::withFlag Query::getWithFlagFromString(string arg) {
    if (isInteger(arg)) {
        return QueryClause::withFlag::isNumber;
    } else if (arg.find("\"") != std::string::npos) {
        return QueryClause::withFlag::isString;
    } else {
        return QueryClause::withFlag::isDeclaration;
    }
}

QueryClause::withFlag Query::getWithFlagFromAttribute(QueryClause::attribute attr) {
    if (attr == QueryClause::attribute::procName) {
        return QueryClause::withFlag::isProcName;
    } else if (attr == QueryClause::attribute::varName) {
        return QueryClause::withFlag::isVarName;
    } else if (attr == QueryClause::attribute::value) {
        return QueryClause::withFlag::isValue;
    } else if (attr == QueryClause::attribute::stmtNum) {
        return QueryClause::withFlag::isStmtNum;
    }
}




