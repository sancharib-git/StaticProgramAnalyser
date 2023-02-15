#ifndef AUTOTESTER_QUERYEVALUATOR_H
#define AUTOTESTER_QUERYEVALUATOR_H
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
#include "Query.h"
#include "QueryOptimiser.h"
#include "QueryClauseResult.h"
#include "CrossProductStruct.h"

class QueryEvaluator {
public:
    typedef CrossProductStruct<QueryElement> resultStruct;
    static QueryEvaluator *instance;

    static QueryEvaluator& getInstance() {
        if (!(instance)) {
            instance = new QueryEvaluator();
        }
        return *instance;
    };

    bool evaluate();

    bool reset();

private:
    Query query;
    QueryOptimiser optimiser;
    IntermediateResult* intermediateResult = IntermediateResult::getPtr();
    bool evaluateDeclaration(string syn);
    bool evaluateDeclarations(vector<string> decs);
    bool evaluateQueryClauses();
    QueryClauseResult* evaluateQueryClause(QueryClause clause);

    QueryElement handleSelectFlag(string syn, Query::selectFlag flag, QueryElement value);

    bool sendResult();
    bool sendResultToQRP(resultStruct* result);
    bool sendEmptyResultToQRP();
    bool sendBooleanResultToQRP(bool boolean);
    bool sendEmptyOrFalseResultToQRP();
};


#endif //AUTOTESTER_QUERYEVALUATOR_H