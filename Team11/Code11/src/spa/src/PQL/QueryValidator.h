#ifndef AUTOTESTER_QUERYVALIDATOR_H
#define AUTOTESTER_QUERYVALIDATOR_H

#include "Query.h"
#include "QueryClause.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

class QueryValidator {
private:

public:
    static bool isDeclarationValid(Query::entityType entityType, string synonym);
    static bool isSelectExist(string query);
    static bool isSelectVarDeclared(string selectVar);
    static Query::selectFlag isSelectVarValid(string selectVar, string attrRef);
    static bool isValidTuple(string var);

    static bool isValidSynRegex(string syn);
    static bool isSufficientAndSyntacticalArguments(string patternSyn, vector<string> arguments);
    static bool isValidSuchThat(QueryClause::relationshipType relationshipType, vector<string> argument);
    static bool isValidFolParentNext(vector<string> argument);
    static bool isValidModifiesUses(QueryClause::relationshipType relationshipType, vector<string> argument);
    static bool isValidAffects(vector<string> argument);
    static bool isValidCalls(vector<string> argument);
    static bool isValidPattern(string assign, vector<string> argument);
    static bool isValidPatternAssign(vector<string> arguments);
    static bool isValidPatternWhile(vector<string> arguments);
    static bool isValidPatternIf(vector<string> arguments);
    static bool isValidWith(string lhs, QueryClause::attribute attr1, string rhs, QueryClause::attribute attr2);

    static bool isBracketValid(string bracket);
    static bool isContentExist(string content);
    enum class assignType { isOperator, isFactor, isOpenBracket, isCloseBracket, isNone };
    static bool isValidAssignment(string assignment);
    static bool areArgumentsValid(string leftHand, string rightHand);
};


#endif //AUTOTESTER_QUERYVALIDATOR_H
