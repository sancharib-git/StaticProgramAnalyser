#include "QueryOptimiser.h"
#include "PKB/PKB.h"
#include "SetProcessor.h"
PKB pkb = PKB::getInstance();

QueryOptimiser::QueryOptimiser() {
    empty = false;
    for (auto n : Query::getInstance().selects) {
        if (n.second != Query::selectFlag::isBoolean) {
            resultCluster.insert(n.first);
        }
    }
    for (auto n : Query::getInstance().clauses) {
        addToCluster(n);
    }
}


bool QueryOptimiser::optimiseQuery() {
    if (Query::getInstance().isOptimised() || Query::getInstance().clauses.size() == 0) {
        return true;
    }
    Query::getInstance().setAsOptimised();
    int affectsNum = PKB::getInstance().getAssignList()->size();
    int stmtTotal = PKB::getInstance().getTotalNumberOfStatements();
    bool manyAffects = stmtTotal != 0 &&
        (100 * affectsNum / PKB::getInstance().getTotalNumberOfStatements() > HIGH_AFFECTS_PERCENTAGE);
    bool manyVars = stmtTotal != 0 &&
        PKB::getInstance().getTotalNumOfVars() > PKB::getInstance().getTotalNumberOfStatements();
    if (manyAffects && manyVars) {
        sort(Query::getInstance().clauses.begin(), Query::getInstance().clauses.end(), QueryClause::compareQueryClauseManyVarsAndAffects());
    } else if (manyAffects) {
        sort(Query::getInstance().clauses.begin(), Query::getInstance().clauses.end(), QueryClause::compareQueryClauseManyAffectsStmt());
    } else if (manyVars) {
        sort(Query::getInstance().clauses.begin(), Query::getInstance().clauses.end(), QueryClause::compareQueryClauseManyVars());
    } else {
        sort(Query::getInstance().clauses.begin(), Query::getInstance().clauses.end(), QueryClause::compareQueryClause());
    }
    vector<QueryClause> duplicatesRemoved = {};
    auto clauses = Query::getInstance().clauses;
    QueryClause prev = clauses.front();
    bool first = true;
    for (QueryClause a : clauses) {
        if (first || !QueryClause::isSameClause(a, prev)) {
            duplicatesRemoved.push_back(a);
        }
        first = false;
        prev = a;
    }
    Query::getInstance().clauses = duplicatesRemoved;
    return true;
}

QueryOptimiser QueryOptimiser::getOptimiser() {
    return QueryOptimiser();
}

bool QueryOptimiser::addToCluster(QueryClause clause) {
    clauseInfo info = extractClauseInfo(clause);
    string syn;
    if (info.isLeftDec && info.isRightDec) {
       return addToCluster(info.arg1.STRING, info.arg2.STRING);
    } else if (info.isLeftDec) {
        syn = info.arg1.STRING;
    } else if (info.isRightDec) {
        syn = info.arg2.STRING;
    } else {
        return true;
    }
    int freq = SetProcessor::getOrDefaultFrom<string, int>(freqMap, syn, 0);
    freqMap.insert_or_assign(syn, freq + 1);
    return true;
}

bool QueryOptimiser::addToCluster(string syn1, string syn2) {
    auto left = SetProcessor::getOrDefaultFrom<string, unordered_set<string>>(clauseCluster, syn1, unordered_set<string>());
    auto right = SetProcessor::getOrDefaultFrom<string, unordered_set<string>>(clauseCluster, syn2, unordered_set<string>());
    auto newCluster = SetProcessor::unionOf<string>({left, right, {syn1, syn2}});
    for (auto a : newCluster) {
        clauseCluster.insert_or_assign(a, newCluster);
    }
    int freq1 = SetProcessor::getOrDefaultFrom<string, int>(freqMap, syn1, 0);
    freqMap.insert_or_assign(syn1, freq1 + 1);
    int freq2 = SetProcessor::getOrDefaultFrom<string, int>(freqMap, syn2, 0);
    freqMap.insert_or_assign(syn2, freq2 + 1);
    return true;
}

bool QueryOptimiser::canBeNonExact(string syn) {
    if (empty) {
        return false;
    }
    return resultCluster.find(syn) == resultCluster.end()
    && SetProcessor::getOrDefaultFrom<string,unordered_set<string>>(clauseCluster, syn, unordered_set<string>()).size() <= 1
    && SetProcessor::getOrDefaultFrom<string, int>(freqMap, syn, 0) <= 1;
}

bool QueryOptimiser::canBeNonExact(string syn1, string syn2) {
    if (empty || syn1 == syn2 || resultCluster.find(syn1) != resultCluster.end() || resultCluster.find(syn2) != resultCluster.end()) {
        return false;
    }
    auto left = SetProcessor::getOrDefaultFrom<string, unordered_set<string>>(clauseCluster, syn1, unordered_set<string>());
    auto right = SetProcessor::getOrDefaultFrom<string, unordered_set<string>>(clauseCluster, syn2, unordered_set<string>());
    auto diff1 = SetProcessor::difference<string>(left, {syn1, syn2});
    auto diff2 = SetProcessor::difference<string>(right, {syn1, syn2});
    return diff1.empty() && diff2.empty()
    && SetProcessor::getOrDefaultFrom<string, int>(freqMap, syn1, 0) <= 1
            && SetProcessor::getOrDefaultFrom<string, int>(freqMap, syn2, 0) <= 1;
}

bool QueryOptimiser::clear() {
    clauseCluster.clear();
    resultCluster.clear();
    empty = true;
    return false;
}

bool QueryOptimiser::canBeNonExact(QueryClause clause) {
    clauseInfo info = extractClauseInfo(clause);
    if (clause.type == QueryClause::clauseType::suchThat) {
        if (info.isLeftDec && info.isRightDec) {
            return canBeNonExact(info.arg1.STRING, info.arg2.STRING);
        } else if (info.isLeftAll && info.isRightAll) {
            return true;
        } else if (info.isLeftDec && !info.isRightDec) {
            return canBeNonExact(info.arg1.STRING);
        } else if (info.isRightDec && !info.isLeftDec) {
            return canBeNonExact(info.arg2.STRING);
        } else if ((info.isRightAll || info.isLeftAll) && !(info.isLeftDec || info.isRightDec)) {
            //one wildcard one number
            return true;
        }
    }
    return false;
}

QueryOptimiser::clauseInfo QueryOptimiser::extractClauseInfo(QueryClause clause) {
    clauseInfo res = clauseInfo();
    if (clause.type == QueryClause::clauseType::with) {
        return extractClauseInfoWith(clause.withStruct);
    } else if (clause.type == QueryClause::clauseType::pattern) {
        return extractClauseInfoPattern(clause.patternStruct);
    } else if (clause.type == QueryClause::clauseType::suchThat) {
        return extractClauseInfoSuchThat(clause.suchThatStruct);
    }
    return res;
}

QueryOptimiser::clauseInfo QueryOptimiser::extractClauseInfoWith(QueryClause::with with) {
    clauseInfo res = clauseInfo();
    if (with.flagLeft == QueryClause::withFlag::isNumber) {
        res.arg1 = QueryElement(stoi(with.lhs));
    } else if (with.flagLeft == QueryClause::withFlag::isString) {
        res.arg1 = QueryElement(with.lhs);
    } else {
        res.isLeftDec = true;
        res.arg1 = QueryElement(with.lhs);
    }
    if (with.flagRight == QueryClause::withFlag::isNumber) {
        res.arg2 = QueryElement(stoi(with.rhs));
    } else if (with.flagRight == QueryClause::withFlag::isString) {
        res.arg2 = QueryElement(with.rhs);
    } else {
        res.isRightDec = true;
        res.arg2 = QueryElement(with.rhs);
    }
    return res;
}

QueryOptimiser::clauseInfo QueryOptimiser::extractClauseInfoSuchThat(QueryClause::suchThat st) {
    clauseInfo res = clauseInfo();
    if(st.flag1 == QueryClause::suchThatFlag::isDeclaration) {
        res.isLeftDec = true;
        res.arg1 = QueryElement(st.arg1);
    } else if (st.flag1 == QueryClause::suchThatFlag::isAll) {
        res.isLeftAll = true;
        res.arg1 = QueryElement::WILDCARD();
    } else if (st.flag1 == QueryClause::suchThatFlag::isNumber) {
        res.arg1 = QueryElement(stoi(st.arg1));
    } else {
        res.arg1 = QueryElement(st.arg1);
    }
    if(st.flag2 == QueryClause::suchThatFlag::isDeclaration) {
        res.isRightDec = true;
        res.arg2 = QueryElement(st.arg2);
    } else if (st.flag2 == QueryClause::suchThatFlag::isAll) {
        res.isRightAll = true;
        res.arg2 = QueryElement::WILDCARD();
    } else if (st.flag2 == QueryClause::suchThatFlag::isNumber) {
        res.arg2 = QueryElement(stoi(st.arg2));
    } else {
        res.arg2 = QueryElement(st.arg2);
    }
    return res;
}

QueryOptimiser::clauseInfo QueryOptimiser::extractClauseInfoPattern(QueryClause::pattern pattern) {
    clauseInfo res = clauseInfo();
    res.isLeftDec = true;
    res.arg1 = QueryElement(pattern.synonym);
    res.isLeftDec = true;
    if (pattern.leftFlag == QueryClause::patternFlag::isDeclaration) {
        res.isRightDec = true;
        res.arg2 = QueryElement(pattern.lhs);
        res.isRightDec = true;
    }
    return res;
}

string QueryOptimiser::to_string() {
    string s = "result cluster: " + SetProcessor::to_string(SetProcessor::set_to_vector<string>(resultCluster)) + "\n";
    for (auto a : clauseCluster) {
        s += a.first + ": ";
        for (auto a1 : a.second) {
            s += a1 + ", ";
        }
        s += "\n";
    }
    s += "frequencies: ";
    for (auto a : freqMap) {
        s += a.first + " " + ::to_string(a.second) + ", ";
    }
    s += "\n";
    return s;
}



