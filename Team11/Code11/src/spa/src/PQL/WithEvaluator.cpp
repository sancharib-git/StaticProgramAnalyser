#include "WithEvaluator.h"

QueryClauseResult* WithEvaluator::evaluateWith(QueryClause::with w) {
    bool isLeftDec = Query::getInstance().isVarDeclared(w.lhs);
    bool isRightDec = Query::getInstance().isVarDeclared(w.rhs);
    bool isLeftCase = isLeftDec &&
                      (Query::getInstance().getDeclarationType(w.lhs) == Query::entityType::read && w.flagLeft == QueryClause::withFlag::isVarName
                       || Query::getInstance().getDeclarationType(w.lhs) == Query::entityType::call && w.flagLeft == QueryClause::withFlag::isProcName
                       || Query::getInstance().getDeclarationType(w.lhs) == Query::entityType::print && w.flagLeft == QueryClause::withFlag::isVarName);
    bool isRightCase = isRightDec &&
                       (Query::getInstance().getDeclarationType(w.rhs) == Query::entityType::read && w.flagRight == QueryClause::withFlag::isVarName
                        || Query::getInstance().getDeclarationType(w.rhs) == Query::entityType::call && w.flagRight == QueryClause::withFlag::isProcName
                        || Query::getInstance().getDeclarationType(w.rhs) == Query::entityType::print && w.flagRight == QueryClause::withFlag::isVarName);
    if (isLeftCase || isRightCase) {
        return evaluateWithDecSpecialCase(w, isLeftCase, isRightCase);
    }
    return evaluateWithNonSpecialCase(w);
}

QueryClauseResult* WithEvaluator::evaluateWithNonSpecialCase(QueryClause::with w) {
    int dec_count = 0;
    vector<string> header = {};
    unordered_set<QueryElement> left = getWithList(w.lhs, w.flagLeft);
    unordered_set<QueryElement> right = getWithList(w.rhs, w.flagRight);
    if (Query::getInstance().isVarDeclared(w.lhs)) {
        header.push_back(w.lhs);
        dec_count++;
    }
    if (Query::getInstance().isVarDeclared(w.rhs)) {
        header.push_back(w.rhs);
        dec_count++;
    }
    unordered_set<QueryElement> intersect = SetProcessor::intersectOf<QueryElement>({left, right});
    QueryClauseResult* res = new QueryClauseResult(header);
    if (intersect.empty()) {
        return QueryClauseResult::getInvalidAsPtr();
    } else if (dec_count == 0) {
        return new QueryClauseResult(true);
    } else if (dec_count == 1) {
        for (auto a : intersect) {
            res->add(a);
        }
        return res;
    } else if (dec_count == 2) {
        for (auto a : intersect) {
            res->add(a, a);
        }
        return res;
    }
    return QueryClauseResult::getInvalidAsPtr();
}

//helper function
unordered_map<QueryElement, vector<QueryElement>> WithEvaluator::getReverseMap(string syn, QueryClause::withFlag flag, unordered_set<QueryElement> currentMatch) {
    if (flag == QueryClause::withFlag::isString || flag == QueryClause::withFlag::isNumber) {
        return {};
    }
    assert(Query::getInstance().isVarDeclared(syn));
    unordered_map<QueryElement, vector<QueryElement>> map = {};
    Query::entityType type = Query::getInstance().getEntityTypeBySynName(syn);
    auto list = IntermediateResult::getPtr()->getSingleColumn(syn);
    for (QueryElement a : list) {
        QueryElement temp = handleWithFlag(syn, flag, a);
        if (currentMatch.find(temp) == currentMatch.end()) {
            continue;
        }
        vector<QueryElement> temp_list = {a};
        if (map.find(temp) != map.end()) {
            temp_list = map.at(temp);
            temp_list.push_back(a);
        }
        map.insert_or_assign(temp, temp_list);
    }
    return map;
}


//helper
QueryClauseResult* helperLeft(QueryClauseResult* res, unordered_set<QueryElement> intersect, unordered_map<QueryElement, vector<QueryElement>> leftMap) {
    for (auto a : intersect) {
        vector<QueryElement> leftReverse = leftMap.at(a);
        for (auto a1 : leftReverse) {
            res->add(a1, a); // add the converted back
        }
    }
    return res;
}

//helper
QueryClauseResult* helperRight(QueryClauseResult* res, unordered_set<QueryElement> intersect, unordered_map<QueryElement, vector<QueryElement>> rightMap) {
    for (auto a : intersect) {
        vector<QueryElement> rightReverse = rightMap.at(a);
        for (auto a2 : rightReverse) {
            res->add(a, a2); // add the converted back
        }
    }
    return res;
}

//helper
QueryClauseResult* helperBoth(QueryClauseResult* res, unordered_set<QueryElement> intersect, unordered_map<QueryElement, vector<QueryElement>> leftMap, unordered_map<QueryElement, vector<QueryElement>> rightMap) {
    for (auto a : intersect) {
        vector<QueryElement> leftReverse = leftMap.at(a);
        vector<QueryElement> rightReverse = rightMap.at(a);
        for (auto a1 : leftReverse) {
            for (auto a2 : rightReverse) {
                res->add(a1, a2); // add the converted back
            }
        }
    }
    return res;
}

//helper
QueryClauseResult* helperSingle(QueryClauseResult* res, unordered_set<QueryElement> intersect, unordered_map<QueryElement, vector<QueryElement>> map) {
    for (auto a : intersect) {
        vector<QueryElement> temp = map.at(a);
        for (auto a1 : temp) {
            res->add(a1); // add the converted back
        }
    }
    return res;
}

QueryClauseResult* WithEvaluator::evaluateWithDecSpecialCase(QueryClause::with w, bool isLeftCase, bool isRightCase) {
    //for read.varName, call.procName, print.varName
    int dec_count = 0;
    vector<string> header = {};
    unordered_set<QueryElement> left = getWithList(w.lhs, w.flagLeft);
    unordered_set<QueryElement> right = getWithList(w.rhs, w.flagRight);
    if (Query::getInstance().isVarDeclared(w.lhs)) {
        header.push_back(w.lhs);
        dec_count++;
    }
    if (Query::getInstance().isVarDeclared(w.rhs)) {
        header.push_back(w.rhs);
        dec_count++;
    }
    assert(isLeftCase || isRightCase);
    QueryClauseResult* res = new QueryClauseResult(header);
    unordered_set<QueryElement> intersect = SetProcessor::intersectOf<QueryElement>({left, right});
    if (intersect.empty()) {
        return QueryClauseResult::getInvalidAsPtr();
    }
    unordered_map<QueryElement, vector<QueryElement>> leftMap = getReverseMap(w.lhs, w.flagLeft, intersect);
    unordered_map<QueryElement, vector<QueryElement>> rightMap = getReverseMap(w.rhs, w.flagRight, intersect);
    if (dec_count == 1) {
        assert(intersect.size() == 1 && (leftMap.empty() || rightMap.empty()));
        if (isLeftCase) {
            return helperSingle(res, intersect, leftMap);
        } else {
            return helperSingle(res, intersect, rightMap);
        }
    }
    if (isLeftCase && isRightCase) {
        return helperBoth(res, intersect, leftMap, rightMap);
    } else if (isLeftCase) {
        return helperLeft(res, intersect, leftMap);
    } else if (isRightCase) {
        return helperRight(res, intersect, rightMap);
    }
    return res;
}


unordered_set<QueryElement> WithEvaluator::getWithList(string val, QueryClause::withFlag flag) {
    unordered_set<QueryElement> list = {};
    if (flag == QueryClause::withFlag::isString) {
        list = {QueryElement(val)};
    } else if (flag == QueryClause::withFlag::isNumber) {
        list = {QueryElement(stoi(val))};
    } else {
        assert(Query::getInstance().isVarDeclared(val));
        auto temp = IntermediateResult::getPtr()->getSingleColumn(val);
        for (auto a : temp) {
            list.insert(handleWithFlag(val, flag, a));
        }
    }
    return list;
}

WithEvaluator::WithEvaluator() {

}

QueryElement WithEvaluator::handleWithFlag(string syn, QueryClause::withFlag flag, QueryElement value) {
    assert((Query::getInstance().usedDeclarations.find(syn) != Query::getInstance().usedDeclarations.end()));
    Query::entityType type = Query::getInstance().declarations.at(syn);
    if (flag == QueryClause::withFlag::isDeclaration) {
        return value;
    }
    bool hasError = false;
    switch (type) {
        case Query::entityType::procedure: {
            if (flag == QueryClause::withFlag::isProcName) {
                return value;
            }
        }
        case Query::entityType::var: {
            if (flag == QueryClause::withFlag::isVarName) {
                return value;
            }
        }
        case Query::entityType::constant: {
            if (flag == QueryClause::withFlag::isValue) {
                return value;
            }
        }
        case Query::entityType::stmt: {
            if (flag == QueryClause::withFlag::isStmtNum) {
                return value;
            }
        }
        case Query::entityType::prog_line: {
            if (flag == QueryClause::withFlag::isStmtNum) {
                return value;
            }
        }
        case Query::entityType::assign: {
            if (flag == QueryClause::withFlag::isStmtNum) {
                return value;
            }
        }
        case Query::entityType::whiles: {
            if (flag == QueryClause::withFlag::isStmtNum) {
                return value;
            }
        }
        case Query::entityType::ifl: {
            if (flag == QueryClause::withFlag::isStmtNum) {
                return value;
            }
        }
        case Query::entityType::call: {
            if (flag == QueryClause::withFlag::isStmtNum) {
                return value;
            } else if (flag == QueryClause::withFlag::isProcName) {
                vector<pair<int, string>> list =
                        PKB::getInstance().getCallsSST(value.INTEGER, StatementTypes::NO_TYPE);
                assert(list.size() == 1);
                return QueryElement(list.front().second);
            }
        }
        case Query::entityType::print: {
            if (flag == QueryClause::withFlag::isStmtNum) {
                return value;
            } else if (flag == QueryClause::withFlag::isVarName) {
                return QueryElement(PKB::getInstance().getPrintVar(value.INTEGER));
            }
        }
        case Query::entityType::read: {
            if (flag == QueryClause::withFlag::isStmtNum) {
                return value;
            } else if (flag == QueryClause::withFlag::isVarName) {
                return QueryElement(PKB::getInstance().getReadVar(value.INTEGER));
            }
        }
        default: {
            //error
            hasError = true;
        }
    }
    return QueryElement::INVALID();//or should return value?
}