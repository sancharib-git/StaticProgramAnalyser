#include <string>
#include <vector>
#include <list>
#include <any>
#include <exception>
#include "QueryResultProjector.h"
#include <iostream>


using namespace std;

//initialising QueryResultProjector
QueryResultProjector* QueryResultProjector::resultProjector = 0;

QueryResultProjector::QueryResultProjector() {
    _processedResult = new list<string>();
}

list<string>* QueryResultProjector::getProcessedResult() {
    return _processedResult;
}

bool QueryResultProjector::setProcessedResult(vector<any> finalResult, CastType typeToCast) {
    resetResultProjector();
    bool isSuccess = false;
    for (int i = 0; i < finalResult.size(); i++) {
        any resultValue = finalResult[i];
        isSuccess = populateProcessedResult(resultValue, typeToCast);
    }
    return isSuccess;
}

bool QueryResultProjector::populateProcessedResult(any resultValue, CastType typeToCast) {
    if (typeToCast == CastType::str) {
        string resultString = any_cast<string>(resultValue);
        _processedResult->push_back(resultString);
        return true;
    }
    else if (typeToCast == CastType::integer) {
        int resultInteger = any_cast<int>(resultValue);
        string resultString = to_string(resultInteger);
        _processedResult->push_back(resultString);
        return true;
    } else if (typeToCast == CastType::boolean) {
        bool resultBool = any_cast<bool>(resultValue);
        resultBool? _processedResult->push_back("TRUE") : _processedResult->push_back("FALSE");
    } else if (typeToCast == CastType::tuple) {
        //todo - figure out how to make a tuple a string
    }
    return false;
}

bool QueryResultProjector::resetResultProjector() {
    _processedResult->clear();
    return true;
};