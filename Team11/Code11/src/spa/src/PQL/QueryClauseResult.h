#ifndef AUTOTESTER_QUERYCLAUSERESULT_H
#define AUTOTESTER_QUERYCLAUSERESULT_H
#include "QueryElement.h"
#include "DoubleColumnResult.h"
#include "IntermediateResult.h"
#include "SetProcessor.h"
#include "CrossProductStruct.h"
#include <unordered_set>

class QueryClauseResult {
public:
    static enum clauseResultType {single, pair, has_result, no_result};
    static QueryClauseResult getInvalid();
    static QueryClauseResult* getInvalidAsPtr();
    static bool unpackAndAdd(QueryClauseResult* clauseResult, IntermediateResult* intermediateResult);
    QueryClauseResult(vector<string> args);
    QueryClauseResult(string syn1, string syn2);
    QueryClauseResult(string syn);
    QueryClauseResult(bool has_result);

    bool add(vector<QueryElement> vals);
    bool add(QueryElement val);
    bool add(QueryElement val1, QueryElement val2);

    bool setCanSkipIntersect();

    string to_string();

private:
    clauseResultType type;
    ::pair<string, string> pairArgs;
    CrossProductStruct<QueryElement>* pairs = new CrossProductStruct<QueryElement>({"", ""});
    string singleArg;
    unordered_set<QueryElement> singles;
    bool canSkipIntersect = false;
};


#endif //AUTOTESTER_QUERYCLAUSERESULT_H
