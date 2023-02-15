#include "PKB/PKB.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "QueryEvaluator.h"
#include "SetProcessor.h"
#include "SuchThatEvaluator.h"
#include "WithEvaluator.h"
#include "PatternEvaluator.h"

QueryEvaluator *QueryEvaluator::instance = 0;

///////////////////////////// QueryClause Evaluator Methods /////////////////////////////
bool QueryEvaluator::evaluate() {
    this->query = Query::getInstance();
    if (query.isSyntaxError()) {
        return sendEmptyResultToQRP();
    } else if (query.isSemanticError()) {
        return sendEmptyOrFalseResultToQRP();
    }
    if (!evaluateDeclarations(query.declarationList)) {
        return sendEmptyResultToQRP();
    }
    QueryOptimiser::optimiseQuery();
    optimiser = QueryOptimiser::getOptimiser();
    query = Query::getInstance();
    if (!evaluateQueryClauses()) {
        return sendEmptyOrFalseResultToQRP();
    }
    return sendResult();
}

///////////////////////////// Evaluate Declarations /////////////////////////////
bool QueryEvaluator::evaluateDeclaration(string syn) {
    bool hasError = false;
    Query::entityType type = query.declarations.at(syn);
    unordered_set<QueryElement> set = {};
    switch (type) {
        case Query::entityType::procedure: {
            auto raw = PKB::getInstance().getProcList();
            for (auto s : *raw) {
                set.insert(QueryElement(s));
            }
            break;
        }
        case Query::entityType::var: {
            auto raw = PKB::getInstance().getAllVars();
            for (auto s : *raw) {
                set.insert(QueryElement(s));
            }
            break;
        }
        case Query::entityType::constant: {
            auto raw = PKB::getInstance().getAllConsts();
            for (auto s : *raw) {
                set.insert(QueryElement(s));
            }
            break;
        }
        case Query::entityType::stmt: {
            int numStmts = PKB::getInstance().getTotalNumberOfStatements();
            for (int i = 0; i < numStmts; i++) {
                set.insert(QueryElement(i + 1));
            }
            break;
        }
        case Query::entityType::prog_line: {
            int numStmts = PKB::getInstance().getTotalNumberOfStatements();
            for (int i = 0; i < numStmts; i++) {
                set.insert(QueryElement(i + 1));
            }
            break;
        }
        case Query::entityType::assign: {
            auto raw = PKB::getInstance().getAssignList();
            for (auto s : *raw) {
                set.insert(QueryElement(s));
            }
            break;
        }
        case Query::entityType::whiles: {
            auto raw = PKB::getInstance().getWhileList();
            for (auto s : *raw) {
                set.insert(QueryElement(s));
            }
            break;
        }
        case Query::entityType::ifl: {
            auto raw = PKB::getInstance().getIfList();
            for (auto s : *raw) {
                set.insert(QueryElement(s));
            }
            break;
        }
        case Query::entityType::call: {
            auto raw = PKB::getInstance().getCallStmtList();
            for (auto s : *raw) {
                set.insert(QueryElement(s));
            }
            break;
        }
        case Query::entityType::print: {
            auto raw = PKB::getInstance().getPrintStmtList();
            for (auto s : *raw) {
                set.insert(QueryElement(s));
            }
            break;
        }
        case Query::entityType::read: {
            auto raw = PKB::getInstance().getReadStmtList();
            for (auto s : *raw) {
                set.insert(QueryElement(s));
            }
            break;
        }
        default: {
            //error
            hasError = true;
        }
    }
    intermediateResult->addOrReplaceSingleColumn(syn, set);
    return !hasError;
}

bool QueryEvaluator::evaluateDeclarations(vector<string> decs) {
    for (string s : decs) {
        if (query.declarations.find(s) == query.declarations.end()) {
            return false;
        }
    }
    for (string s : decs) {
        if (query.usedDeclarations.find(s) == query.usedDeclarations.end()) {
            continue;
        }
        bool hasError = !evaluateDeclaration(s);
        if (hasError) {
            return false;
        }
    }
    return true;
}

///////////////////////////// Evaluate Query Clause /////////////////////////////

bool QueryEvaluator::evaluateQueryClauses() {
    for (auto a : query.clauses) {
        QueryClauseResult* res = evaluateQueryClause(a);
        if (!QueryClauseResult::unpackAndAdd(res, intermediateResult)) {
            return false;
        }
        if (!intermediateResult->getResultBoolean()) {
            return false;
        }
    }
    return true;
}

QueryClauseResult* QueryEvaluator::evaluateQueryClause(QueryClause clause) {
    if (clause.type == QueryClause::clauseType::with) {
        WithEvaluator withEvaluator = WithEvaluator();
        return withEvaluator.evaluateWith(clause.withStruct);
    } else if (clause.type == QueryClause::clauseType::suchThat) {
        SuchThatEvaluator stEvaluator = SuchThatEvaluator(optimiser);
        return stEvaluator.evaluateSuchThat(clause.suchThatStruct);
    } else if (clause.type == QueryClause::clauseType::pattern) {
        PatternEvaluator patternEvaluator = PatternEvaluator();
        return patternEvaluator.evaluatePattern(clause.patternStruct);
    }
    return QueryClauseResult::getInvalidAsPtr();
}

///////////////////////////// Send results to QRP /////////////////////////////
bool QueryEvaluator::reset() {
    intermediateResult->clear();
    Query::getInstance().reset();
    return true;
}

bool QueryEvaluator::sendResultToQRP(resultStruct* result) {
    int index = 0;
    vector<vector<QueryElement>>* to_send = new vector<vector<QueryElement>>;
    while (index < result->body.size()) {
        vector<QueryElement> row = {};
        for (auto a : query.selects) {
            QueryElement e = result->getElementOfColumnFromRow(a.first, index);
            row.push_back(handleSelectFlag(a.first, a.second, e));
        }
        index++;
        to_send->push_back(row);
    }
    reset();
    return QueryResultProjector::getInstance().setTupleResult(to_send);
}

bool QueryEvaluator::sendBooleanResultToQRP(bool boolean) {
    if (query.isBooleanResult()) {
        reset();
        return boolean ? QueryResultProjector::getInstance().setBooleanTrue()
                       : QueryResultProjector::getInstance().setBooleanFalse();
    } else {
        reset();
        return QueryResultProjector::getInstance().setEmptyResult();
    }
}

bool QueryEvaluator::sendEmptyResultToQRP() {
    reset();
    return QueryResultProjector::getInstance().setEmptyResult();
}

bool QueryEvaluator::sendEmptyOrFalseResultToQRP() {
    if (query.isBooleanResult()) {
        reset();
        return QueryResultProjector::getInstance().setBooleanFalse();
    }
    reset();
    return QueryResultProjector::getInstance().setEmptyResult();
}

QueryElement QueryEvaluator::handleSelectFlag(string syn, Query::Query::selectFlag flag, QueryElement value) {
    assert((query.usedDeclarations.find(syn) != query.usedDeclarations.end()));
    assert(flag != Query::selectFlag::isBoolean);
    bool hasError = false;
    Query::entityType type = query.declarations.at(syn);
    if (flag == Query::selectFlag::isSynonymName) {
        return value;
    }
    switch (type) {
        case Query::entityType::procedure: {
            if (flag == Query::selectFlag::isProcName) {
                return value;
            }
        }
        case Query::entityType::var: {
            if (flag == Query::selectFlag::isVarName) {
                return value;
            }
        }
        case Query::entityType::call: {
            if (flag == Query::selectFlag::isProcName) {
                vector<pair<int, string>> list =
                        PKB::getInstance().getCallsSST(value.INTEGER, StatementTypes::NO_TYPE);
                assert(list.size() == 1);
                return QueryElement(list.front().second);
            }
        }
        case Query::entityType::print: {
            if (flag == Query::selectFlag::isVarName) {
                return QueryElement(PKB::getInstance().getPrintVar(value.INTEGER));
            }
        }
        case Query::entityType::read: {
            if (flag == Query::Query::selectFlag::isVarName) {
                return QueryElement(PKB::getInstance().getReadVar(value.INTEGER));
            }
        }
    }
    return QueryElement::INVALID();//or should return value?
}

bool QueryEvaluator::sendResult() {
    if (query.isBooleanResult()) {
        return sendBooleanResultToQRP(intermediateResult->getResultBoolean());
    }
    vector<string> select_vars = {};
    for (auto a : query.selects) {
        select_vars.push_back(a.first);
    }
    intermediateResult->reduceAllResults();
    return sendResultToQRP(intermediateResult->getResultTuple(select_vars));
}
