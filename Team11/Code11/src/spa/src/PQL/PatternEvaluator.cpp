#include "PatternEvaluator.h"

QueryClauseResult* PatternEvaluator::evaluatePattern(QueryClause::pattern p) {
    Query::entityType type = Query::getInstance().getDeclarationType(p.synonym);
    if (p.leftFlag == QueryClause::patternFlag::isDeclaration) {
        if (type == Query::entityType::assign) {
            return getMatchedAssignStmtLHSDec(p.synonym,p.lhs, getMatchedAssignStmtRHS(p.synonym, p.rhs, p.rightFlag));
        }
        assert(type == Query::entityType::whiles || type == Query::entityType::ifl);
        return getMatchedIfWhileStmtLHSDec(p.synonym, p.lhs, type);
    } else {
        assert(p.leftFlag == QueryClause::patternFlag::isAll || p.leftFlag == QueryClause::patternFlag::isExactMatch);
        QueryClauseResult* res = new QueryClauseResult(p.synonym);
        unordered_set<QueryElement> set = {};
        if (type == Query::entityType::assign) {
            set = SetProcessor::intersectOf<QueryElement>({getMatchedAssignStmtRHS(p.synonym, p.rhs, p.rightFlag),
                                                           getMatchedAssignStmtLHS(p.synonym, p.lhs, p.leftFlag)});
        } else if (type == Query::entityType::whiles || type == Query::entityType::ifl) {
            set = getMatchedWhileIfStmtLHS(p.synonym, p.lhs, p.leftFlag);
        }
        for (auto a : set) {
            res->add(a);
        }
        return res;
    }
}

QueryClauseResult* PatternEvaluator::getMatchedAssignStmtLHSDec(string syn, string lhs, unordered_set<QueryElement> currentMatch) {
    QueryClauseResult* res = new QueryClauseResult(syn, lhs);
    for (auto s : currentMatch) {
        string leftVar = PKB::getInstance().getAssignLHSVar(s.INTEGER);
        if (IntermediateResult::getPtr()->existsInSingleColumn(lhs, leftVar)) {
            res->add(s, QueryElement(leftVar));
        }
    }
    return res;
}

QueryClauseResult* PatternEvaluator::getMatchedIfWhileStmtLHSDec(string syn, string lhs, Query::entityType type) {
    QueryClauseResult* res = new QueryClauseResult(syn, lhs);
    unordered_set<QueryElement> vars = IntermediateResult::getPtr()->getSingleColumn(lhs);
    for (auto a : vars) {
        unordered_set<int> assocStmt = {};
        if (type == Query::entityType::whiles) {
            assocStmt = PKB::getInstance().getWhileStmtsWithCV(a.STRING);
        } else if (type == Query::entityType::ifl) {
            assocStmt = PKB::getInstance().getIfStmtsWithCV(a.STRING);
        }
        for (int i : assocStmt) {
            if (IntermediateResult::getPtr()->existsInSingleColumn(syn, QueryElement(i))) {
                res->add(QueryElement(i), a);
            }
        }
    }
    return res;
}

unordered_set<QueryElement> PatternEvaluator::getMatchedAssignStmtRHS(string syn, string rhs, QueryClause::patternFlag flag) {
    Query::entityType type = Query::getInstance().getDeclarationType(syn);
    assert(type == Query::entityType::assign);
    unordered_set<QueryElement> current = IntermediateResult::getPtr()->getSingleColumn(syn);
    if (flag == QueryClause::patternFlag::isAll) {
        return current;
    }
    unordered_set<int> raw = PKB::getInstance().getSubExpStmtNums(rhs);
    unordered_set<QueryElement> matched = {};
    for (int stmt : raw) {
        if (current.find(QueryElement(stmt)) != current.end()) {
            matched.insert(QueryElement(stmt));
        }
    }
    if (flag == QueryClause::patternFlag::isContains) {
        return matched;
    } else if (flag == QueryClause::patternFlag::isExactMatch) {
        unordered_set<QueryElement> res = {};
        for (QueryElement e : matched) {
            if (type == Query::entityType::assign && PKB::getInstance().getAssignPattern(e.INTEGER) == rhs) {
                res.insert(e);
            }
        }
        return res;
    }
    return {};
}

unordered_set<QueryElement> PatternEvaluator::getMatchedAssignStmtLHS(string syn, string lhs, QueryClause::patternFlag flag) {
    unordered_set<QueryElement> res = IntermediateResult::getPtr()->getSingleColumn(syn);
    if (flag == QueryClause::patternFlag::isAll) {
        return res;
    }
    unordered_set<QueryElement> newRes = {};
    for (auto a : res) {
        if (PKB::getInstance().getAssignLHSVar(a.INTEGER) == lhs) {
            newRes.insert(a);
        }
    }
    return newRes;
}

unordered_set<QueryElement> PatternEvaluator::getMatchedWhileIfStmtLHS(string syn, string lhs, QueryClause::patternFlag flag) {
    bool isWhile = (Query::getInstance().getEntityTypeBySynName(syn) == Query::entityType::whiles);
    assert(isWhile || Query::getInstance().getEntityTypeBySynName(syn) == Query::entityType::ifl);
    unordered_set<QueryElement> res = IntermediateResult::getPtr()->getSingleColumn(syn);
    unordered_set<QueryElement> newRes = {};
    auto pkbSet = isWhile ? PKB::getInstance().getWhileStmtsWithCV(lhs) : PKB::getInstance().getIfStmtsWithCV(lhs);
    for (auto a : res) {
        if (flag != QueryClause::patternFlag::isAll && pkbSet.find(a.INTEGER) != pkbSet.end()) {
            newRes.insert(a);
        } else if (flag == QueryClause::patternFlag::isAll) {
            if (isWhile && !PKB::getInstance().getAllCVInWhileStmt(a.INTEGER).empty()) {
                newRes.insert(a);
            } else if (!isWhile && !PKB::getInstance().getAllCVInIfStmt(a.INTEGER).empty()) {
                newRes.insert(a);
            }
        }
    }
    return newRes;
}

PatternEvaluator::PatternEvaluator() {

}
