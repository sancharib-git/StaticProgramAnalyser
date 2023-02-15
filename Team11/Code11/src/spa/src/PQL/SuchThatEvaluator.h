#ifndef AUTOTESTER_SUCHTHATEVALUATOR_H
#define AUTOTESTER_SUCHTHATEVALUATOR_H
#include "Query.h"
#include "QueryClause.h"
#include "QueryClauseResult.h"
#include "QueryOptimiser.h"
#include "IntermediateResult.h"

class SuchThatEvaluator {
public:
    QueryOptimiser optimiser;
    SuchThatEvaluator(QueryOptimiser op);

    static int const THRESHOLD_DIFFERENCE = 250;
    static int const THRESHOLD_DIFFERENCE_2 = 50;

    QueryClauseResult* evaluateSuchThat(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatNN(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatNT(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatTN(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatTT(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatNTOptimised(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatTNOptimised(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatTTOptimised(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatNNExtension(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatNTExtension(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatTNExtension(QueryClause::suchThat st);
    QueryClauseResult* evaluateSuchThatTTExtension(QueryClause::suchThat st);
};


#endif //AUTOTESTER_SUCHTHATEVALUATOR_H
