#ifndef AUTOTESTER_INTERMEDIATERESULT_H
#define AUTOTESTER_INTERMEDIATERESULT_H
#include "QueryElement.h"
#include "DoubleColumnResult.h"
#include "SetProcessor.h"
#include "CrossProductStruct.h"
#include "CrossProductStruct.cpp"
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>

//doing the actual work of query evaluator
class IntermediateResult {
private:
    typedef unordered_set<QueryElement> element_set;
    unordered_map<string, element_set> singleColumnResults;
    unordered_map<string, unordered_map<string, DoubleColumnResult *>> doubleColumnResults;
    unordered_map<string, unordered_set<string> *> argClusters;
    static IntermediateResult* instance;
public:

    static IntermediateResult* getPtr() {
        if (!instance) {
            instance = new IntermediateResult();
        }
        return instance;
    }

    element_set getSingleColumn(string syn);

    DoubleColumnResult *getDoubleColumn(string syn1, string syn2);

    CrossProductStruct<QueryElement> getDoubleColumnCPS(string syn1, string syn2);

    CrossProductStruct<QueryElement>* getDoubleColumnCPSPtr(string syn1, string syn2);

    unordered_map<string, DoubleColumnResult*> getDoubleColumnSetOf(string syn);

    unordered_set<string>* getArgCluster(string syn);

    bool addOrReplaceSingleColumn(string syn, element_set set);

    bool addToSingleColumn(string syn, QueryElement elem);

    bool intersectSingleColumnWith(string syn, element_set newSet);

    bool existsInSingleColumn(string syn, QueryElement elem);

    bool existsInAnyDoubleColumn(string syn, QueryElement elem);

    bool existsInAllDoubleColumn(string syn, QueryElement elem);

    bool existsInDoubleColumn(string syn1, string syn2, QueryElement elem1, QueryElement elem2);

    bool hasDoubleColumn(string syn1, string syn2);

    int doubleColumnSize(string syn1, string syn2);

    bool hasSingleColumn(string syn);

    int singleColumnSize(string syn);

    bool addToCluster(string syn1, string syn2);

    bool addOrReplaceDoubleColumn(DoubleColumnResult* dbcol, bool isDirectReplace);

    bool addOrReplaceDoubleColumn(CrossProductStruct<QueryElement> doubleColumnCPS, bool isDirectReplace);

    bool hasResultBeforeReduction();

    bool getResultBoolean();

    CrossProductStruct<QueryElement>* getResultSingle(string syn);

    static bool areEqualRows(vector<QueryElement> row1, vector<QueryElement> row2);

    CrossProductStruct<QueryElement>* crossProductWithinCluster(vector<string> header);

    CrossProductStruct<QueryElement>* getResultTuple(vector<string> header);

    bool eraseWithCascade(string syn1, QueryElement elem1, string syn2);

    bool reduceResult(string syn);

    bool reduceAllResults();

    ////////////////////////////////////// to_string functions //////////////////////////////////////
    string to_string_single_column();

    string to_string_arg_clusters();

    string to_string_double_columns_of(string syn);

    string to_string_arg_cluster_of(string syn);

    string to_string();

    bool clear();
};
#endif //AUTOTESTER_INTERMEDIATERESULT_H
