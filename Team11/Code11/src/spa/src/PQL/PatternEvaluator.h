#ifndef AUTOTESTER_PATTERNEVALUATOR_H
#define AUTOTESTER_PATTERNEVALUATOR_H
#include "Query.h"
#include "QueryClause.h"
#include "QueryClauseResult.h"
#include "QueryOptimiser.h"
#include "IntermediateResult.h"

class PatternEvaluator {
public:
    PatternEvaluator();

    QueryClauseResult* evaluatePattern(QueryClause::pattern p);
    QueryClauseResult* getMatchedAssignStmtLHSDec(string syn, string lhs, unordered_set<QueryElement> currentMatch);
    QueryClauseResult* getMatchedIfWhileStmtLHSDec(string syn, string lhs, Query::entityType type);
    unordered_set<QueryElement> getMatchedAssignStmtRHS(string syn, string rhs, QueryClause::patternFlag flag);
    unordered_set<QueryElement> getMatchedAssignStmtLHS(string syn, string lhs, QueryClause::patternFlag flag);
    unordered_set<QueryElement> getMatchedWhileIfStmtLHS(string syn, string lhs, QueryClause::patternFlag flag);
};


#endif //AUTOTESTER_PATTERNEVALUATOR_H
