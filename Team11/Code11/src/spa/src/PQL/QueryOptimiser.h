#ifndef AUTOTESTER_QUERYOPTIMISER_H
#define AUTOTESTER_QUERYOPTIMISER_H
#include "Query.h"
#include "QueryClause.h"
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>

class QueryOptimiser {
public:
    static const int HIGH_AFFECTS_PERCENTAGE = 75;
    QueryOptimiser();
    static bool optimiseQuery();
    static QueryOptimiser getOptimiser();
    bool canBeNonExact(string syn);
    bool canBeNonExact(string syn1, string syn2);
    bool clear();
    bool canBeNonExact(QueryClause clause);

    string to_string();

private:
    struct clauseInfo {
        QueryElement arg1 = QueryElement("");
        QueryElement arg2 = QueryElement("");
        bool isLeftDec = false;
        bool isRightDec = false;
        bool isLeftAll = false;
        bool isRightAll = false;

        string to_string() {
            return arg1.to_string() + arg2.to_string() + ::to_string(isLeftDec) + ::to_string(isRightDec) + ::to_string(isLeftAll) + ::to_string(isRightAll);
        }

    };
    bool empty = true;
    unordered_map<string, unordered_set<string>> clauseCluster = {};
    unordered_set<string> resultCluster = {};
    unordered_map<string, int> freqMap = {};
    bool addToCluster(QueryClause clause);
    bool addToCluster(string syn1, string syn2);
    clauseInfo extractClauseInfo(QueryClause clause);
    clauseInfo extractClauseInfoWith(QueryClause::with with);
    clauseInfo extractClauseInfoSuchThat(QueryClause::suchThat st);
    clauseInfo extractClauseInfoPattern(QueryClause::pattern pattern);
};


#endif //AUTOTESTER_QUERYOPTIMISER_H
