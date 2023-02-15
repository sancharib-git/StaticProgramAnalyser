#ifndef AUTOTESTER_DOUBLECOLUMNRESULT_H
#define AUTOTESTER_DOUBLECOLUMNRESULT_H

#include "QueryElement.h"
#include "SetProcessor.h"
#include "CrossProductStruct.h"
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;
class DoubleColumnResult {
private:
    typedef unordered_set<QueryElement> column;
    typedef unordered_map<QueryElement, column> double_column_table;
    unordered_map<string, double_column_table*> tables = {};
    unordered_map<string, column> keySets = {};
    bool used = false;
    bool isFine = true;
    int _size = 0;

    DoubleColumnResult();

    bool addSingleSide(string syn1, string syn2, QueryElement key1, QueryElement key2);
    bool addToKey(string syn, QueryElement key);
    bool eraseSingleSide(string syn1, string syn2, QueryElement key1, QueryElement key2);
    bool eraseKey(string syn, QueryElement key);

public:
    pair<string, string> synonymPair;

    DoubleColumnResult(string syn1, string syn2);

    DoubleColumnResult(string syn1, string syn2, vector<pair<QueryElement, QueryElement>> list);

    bool inPair(string a);

    string otherInPair(string a);

    double_column_table* getTable(string syn);

    column getColumnFromTable(double_column_table* d, QueryElement key);

    column getKeySet(string syn);

    bool isNew();

    bool isValid();

    int size();

    bool add(string syn1, string syn2, QueryElement key1, QueryElement key2);

    bool exists(string syn1, string syn2, QueryElement key1, QueryElement key2);

    bool exists(string syn, QueryElement key);

    column getAllAssociatedWith(string syn, QueryElement key);

    bool erase(string syn1, string syn2, QueryElement key1, QueryElement key2);

    bool erase(string syn, QueryElement key);

    string table_to_string(string syn);

    string keyset_to_string(string syn);

    string to_string();

    CrossProductStruct<QueryElement> convertToCrossProductStruct();

    CrossProductStruct<QueryElement> convertToCrossProductStruct(string syn1, string syn2);

    CrossProductStruct<QueryElement>* convertToCrossProductStructPtr(string syn1, string syn2);

    static DoubleColumnResult* doubleColumnCrossProductStructToDoubleColumnResult(CrossProductStruct<QueryElement> cps);

    static DoubleColumnResult intersectOf(DoubleColumnResult table1, DoubleColumnResult table2);
};
#endif //AUTOTESTER_DOUBLECOLUMNRESULT_H