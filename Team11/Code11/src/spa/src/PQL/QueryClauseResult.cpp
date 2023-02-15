#include "QueryClauseResult.h"

QueryClauseResult QueryClauseResult::getInvalid() {
    return QueryClauseResult(false);
}

bool QueryClauseResult::unpackAndAdd(QueryClauseResult* clauseResult, IntermediateResult* intermediateResult) {
    bool res = true;
    if (clauseResult->type == clauseResultType::single) {
        if (clauseResult->singleArg != "") {
            res = !clauseResult->singles.empty()
                  &&  intermediateResult->addOrReplaceSingleColumn(clauseResult->singleArg, clauseResult->singles);
        }
    } else if (clauseResult->type == clauseResultType::pair) {
        res = intermediateResult->addOrReplaceDoubleColumn(*(clauseResult->pairs), clauseResult->canSkipIntersect);
    } else if (clauseResult->type == clauseResultType::has_result) {
        res = true;
    } else if (clauseResult->type == clauseResultType::no_result) {
        res = false;
    }
    return res;
}

QueryClauseResult::QueryClauseResult(vector<string> args) {
    if (args.size() == 1) {
        type = clauseResultType::single;
        singleArg = args.front();
    } else if (args.size() == 2) {
        string syn1 = *(args.begin());
        string syn2 = *(args.begin() + 1);
        if (syn1 == syn2) {
            type = clauseResultType::single;
            singleArg = syn1;
            pairArgs = make_pair(syn1, syn2);
        } else {
            type = clauseResultType::pair;
            pairArgs = make_pair(syn1, syn2);
            pairs = new CrossProductStruct<QueryElement>({syn1, syn2});
        }
    } else if (args.size() == 0) {
        type = clauseResultType::has_result;
    } else {
        type = clauseResultType::no_result;
    }
}

QueryClauseResult::QueryClauseResult(string syn1, string syn2) {
    if (syn1 == syn2) {
        type = clauseResultType::single;
        singleArg = syn1;
        pairArgs = make_pair(syn1, syn2);
    } else {
        type = clauseResultType::pair;
        pairArgs = make_pair(syn1, syn2);
        pairs = new CrossProductStruct<QueryElement>({syn1, syn2});
    }
}

QueryClauseResult::QueryClauseResult(string syn) {
    type = clauseResultType::single;
    singleArg = syn;
}

QueryClauseResult::QueryClauseResult(bool has_result) {
    if (has_result) {
        type = clauseResultType::has_result;
    } else {
        type = clauseResultType::no_result;
    }
}

bool QueryClauseResult::add(vector<QueryElement> vals) {
    if (vals.size() == 1) {
        add(vals.front());
    } else if (vals.size() == 2) {
        QueryElement val1 = *(vals.begin());
        QueryElement val2 = *(vals.begin() + 1);
        add(val1, val2);
    }
    return vals.size() == 0;
}

bool QueryClauseResult::add(QueryElement val) {
    singles.insert(val);
    return true;
}

bool QueryClauseResult::add(QueryElement val1, QueryElement val2) {
    if (pairArgs.first == pairArgs.second) {
        return (val1 == val2) && add(val1);
    }
    return pairs->addRow({val1, val2});;
}

string QueryClauseResult::to_string() {
    string s = "\nQueryClauseResult ";
    if (type == clauseResultType::single) {
        s += "type: single\n";
        s += singleArg + ": ";
        s += SetProcessor::to_string(SetProcessor::set_to_vector(singles));
    } else if (type == clauseResultType::pair) {
        s += "type: pair\n";
        s += pairs->to_string();
    } else if (type == clauseResultType::has_result) {
        s += "type: has_result\n";
    } else if (type == clauseResultType::no_result) {
        s += "type: no_result\n";
    }
    return s + "\n";
}

bool QueryClauseResult::setCanSkipIntersect() {
    canSkipIntersect = true;
    return true;
}

QueryClauseResult *QueryClauseResult::getInvalidAsPtr() {
    return new QueryClauseResult(false);
}
