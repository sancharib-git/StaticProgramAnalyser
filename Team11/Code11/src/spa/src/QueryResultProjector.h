#pragma once

#include <string>
#include <vector>
#include <list>
#include <any>
#include <exception>
#include "PQL/QueryElement.h"
using namespace std;
enum class CastType { str, integer, boolean, tuple };
class QueryResultProjector {
private:
    list<string>* _processedResult;
    static QueryResultProjector* resultProjector;

public:
    QueryResultProjector();
    static QueryResultProjector& getInstance() {
        if (!(resultProjector)) {
            resultProjector = new QueryResultProjector();
        }
        return *resultProjector;
    };
    list<string>* getProcessedResult();
    bool setProcessedResult(vector<any> finalResult, CastType typeToCast);
    bool populateProcessedResult(any resultValue, CastType typeToCast);
    bool resetResultProjector();
    bool setBooleanFalse() {
        *_processedResult = {"FALSE"};
        return true;
    }
    bool setBooleanTrue() {
        *_processedResult = {"TRUE"};
        return true;
    }
    bool setEmptyResult() {
        *_processedResult = {};
        return true;
    }

    string tupleToString(vector<QueryElement> tuple) {
        string s = "";
        for (auto a : tuple) {
            s.append(a.to_string()).append(" ");
        }
        s = s.substr(0, s.find_last_of(" "));
        return s;
    }

    bool setTupleResult(vector<vector<QueryElement>>* list_of_tuples) {
        *_processedResult = {};
        for (auto a : *list_of_tuples) {
           _processedResult->push_back(tupleToString(a));
        }
        return true;
    }
};