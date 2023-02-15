#ifndef AUTOTESTER_WITHEVALUATOR_H
#define AUTOTESTER_WITHEVALUATOR_H
#include "Query.h"
#include "QueryClause.h"
#include "QueryClauseResult.h"
#include "IntermediateResult.h"

class WithEvaluator {
public:
    WithEvaluator();
    QueryClauseResult* evaluateWith(QueryClause::with w);
    QueryClauseResult* evaluateWithNonSpecialCase(QueryClause::with w);
    QueryClauseResult* evaluateWithDecSpecialCase(QueryClause::with w, bool isLeftCase, bool isRightCase);
    unordered_set<QueryElement> getWithList(string val, QueryClause::withFlag flag);
    unordered_map<QueryElement, vector<QueryElement>> getReverseMap(string syn, QueryClause::withFlag flag, unordered_set<QueryElement> currentMatch);

    QueryElement handleWithFlag(string syn, QueryClause::withFlag flag, QueryElement value);
};


#endif //AUTOTESTER_WITHEVALUATOR_H
