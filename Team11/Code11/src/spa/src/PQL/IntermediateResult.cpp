#include "IntermediateResult.h"
IntermediateResult *IntermediateResult::instance = 0;

struct compareRow {
    inline bool operator() (vector<QueryElement> row1, vector<QueryElement> row2)
    {
        int index = 0;
        int size = ::min(row1.size(), row2.size());
        vector<QueryElement>::iterator row1It = row1.begin();
        vector<QueryElement>::iterator row2It = row2.begin();
        if ((*(row1It + index)).TYPE != (*(row2It + index)).TYPE) {
            return false;
        }
        while (index < size) {
            if ((*(row1It + index)).lessThan(*(row2It + index))) {
                return true;
            } else if ((*(row1It + index)) == (*(row2It + index))) {
                index++;
                continue;
            } else {
                return false;
            }
        }
        return false;
    }
};

IntermediateResult::element_set IntermediateResult::getSingleColumn(string syn) {
    if (singleColumnResults.find(syn) != singleColumnResults.end()) {
        return singleColumnResults.at(syn);
    }
    return {};
}

DoubleColumnResult *IntermediateResult::getDoubleColumn(string syn1, string syn2) {
    if (doubleColumnResults.find(syn1) != doubleColumnResults.end()) {
        if (doubleColumnResults.at(syn1).find(syn2) != doubleColumnResults.at(syn1).end()) {
            return doubleColumnResults.at(syn1).at(syn2);
        }
    }
    return new DoubleColumnResult(syn1, syn2);
}

CrossProductStruct<QueryElement> IntermediateResult::getDoubleColumnCPS(string syn1, string syn2) {
    return getDoubleColumn(syn1, syn2)->convertToCrossProductStruct(syn1, syn2);
}

CrossProductStruct<QueryElement> *IntermediateResult::getDoubleColumnCPSPtr(string syn1, string syn2) {
    return getDoubleColumn(syn1, syn2)->convertToCrossProductStructPtr(syn1, syn2);
}

unordered_map<string, DoubleColumnResult *> IntermediateResult::getDoubleColumnSetOf(string syn) {
    if (doubleColumnResults.find(syn) != doubleColumnResults.end()) {
        return doubleColumnResults.at(syn);
    }
    return {};
}

unordered_set<string> *IntermediateResult::getArgCluster(string syn) {
    if (argClusters.find(syn) != argClusters.end()) {
        return argClusters.at(syn);
    }
    return new unordered_set<string>();
}

bool IntermediateResult::addOrReplaceSingleColumn(string syn, IntermediateResult::element_set set) {
    if (singleColumnResults.find(syn) == singleColumnResults.end()) {
        //add
        singleColumnResults.insert_or_assign(syn, set);
    } else {
        //replace
        intersectSingleColumnWith(syn, set);

    }
    return true;
}

bool IntermediateResult::addToSingleColumn(string syn, QueryElement elem) {
    element_set set = getSingleColumn(syn);
    set.insert(elem);
    singleColumnResults.insert_or_assign(syn, set);
    return true;}

bool IntermediateResult::intersectSingleColumnWith(string syn, IntermediateResult::element_set newSet) {
    singleColumnResults.insert_or_assign(syn,
                                         SetProcessor::intersectOf<QueryElement>(
                                                 {getSingleColumn(syn), newSet}));
    return true;
}

bool IntermediateResult::existsInSingleColumn(string syn, QueryElement elem) {
    element_set set = getSingleColumn(syn);
    return set.find(elem) != set.end();}

bool IntermediateResult::existsInAnyDoubleColumn(string syn, QueryElement elem) {
    bool anyExists = false;
    for (auto& d : getDoubleColumnSetOf(syn)) {
        anyExists = anyExists || d.second->exists(syn, elem);
        if (anyExists) {
            return true;
        }
    }
    return anyExists;
}

bool IntermediateResult::existsInAllDoubleColumn(string syn, QueryElement elem) {
    bool allExists = true;
    for (auto& d : getDoubleColumnSetOf(syn)) {
        allExists = allExists && d.second->exists(syn, elem);
        if (!allExists) {
            return false;
        }
    }
    return allExists;
}

bool IntermediateResult::existsInDoubleColumn(string syn1, string syn2, QueryElement elem1, QueryElement elem2) {
    DoubleColumnResult* dbcol = getDoubleColumn(syn1, syn2);
    return dbcol->exists(syn1, syn2, elem1, elem2);}

bool IntermediateResult::hasDoubleColumn(string syn1, string syn2) {
    return !getDoubleColumn(syn1, syn2)->isNew();
}

int IntermediateResult::doubleColumnSize(string syn1, string syn2) {
    return hasDoubleColumn(syn1, syn2) ? getDoubleColumn(syn1, syn2)->size() : 0;
}

bool IntermediateResult::hasSingleColumn(string syn) {
    if (singleColumnResults.find(syn) != singleColumnResults.end()) {
        return true;
    }
    return false;
}

int IntermediateResult::singleColumnSize(string syn) {
    return hasSingleColumn(syn) ? getSingleColumn(syn).size() : 0;
}

bool IntermediateResult::addToCluster(string syn1, string syn2) {
    unordered_set<string>* argClusterResultSyn1 = getArgCluster(syn1);
    unordered_set<string>* argClusterResultSyn2 = getArgCluster(syn2);
    if (argClusterResultSyn1->find(syn2) == argClusterResultSyn1->end()
        || argClusterResultSyn2->find(syn1) == argClusterResultSyn2->end()) {
        unordered_set<string> *argsGroup1 = getArgCluster(syn1);
        unordered_set<string> *argsGroup2 = getArgCluster(syn2);
        argsGroup1->insert(syn2);
        argsGroup2->insert(syn1);
        unordered_set<string>* argsUnion = new unordered_set<string>();
        for (string s: *argsGroup1) {
            argsUnion->insert(s);
        }
        for (string s: *argsGroup2) {
            argsUnion->insert(s);
        }
        for (string s: *argsUnion) {
            argClusters.insert_or_assign(s, argsUnion);
        }
    }
    return true;
}

bool IntermediateResult::addOrReplaceDoubleColumn(DoubleColumnResult *dbcol, bool isDirectReplace) {
    string syn1 = dbcol->synonymPair.first;
    string syn2 = dbcol->synonymPair.second;
    addToCluster(syn1, syn2);
    if (!hasDoubleColumn(syn1, syn2)) {
        unordered_map<string, DoubleColumnResult*> temp = getDoubleColumnSetOf(syn1);
        temp.insert_or_assign(syn2, dbcol);
        doubleColumnResults.insert_or_assign(syn1, temp);

        unordered_map<string, DoubleColumnResult*> temp2 = getDoubleColumnSetOf(syn2);
        temp2.insert_or_assign(syn1, dbcol);
        doubleColumnResults.insert_or_assign(syn2, temp2);
        return true;
    } else {
        if (isDirectReplace) {
            *getDoubleColumn(syn1, syn2) = *dbcol;
            *getDoubleColumn(syn2, syn1) = *dbcol;
            return true;
        }
        DoubleColumnResult* db = getDoubleColumn(syn1, syn2);
        DoubleColumnResult intersect = DoubleColumnResult::intersectOf(*db, *dbcol);
        *getDoubleColumn(syn1, syn2) = intersect;
        *getDoubleColumn(syn2, syn1) = intersect;
        return true;
    }
}

bool IntermediateResult::addOrReplaceDoubleColumn(CrossProductStruct<QueryElement> doubleColumnCPS, bool isDirectReplace) {
    string syn1 = doubleColumnCPS.getColumnHeader(0);
    string syn2 = doubleColumnCPS.getColumnHeader(1);
    if (!doubleColumnCPS.isValid() || doubleColumnCPS.header.size() != 2 || syn1 == syn2 || syn1 == "" || syn2 == "") {
        return false;
    }
    DoubleColumnResult* dbcol = DoubleColumnResult::doubleColumnCrossProductStructToDoubleColumnResult(doubleColumnCPS);
    return addOrReplaceDoubleColumn(dbcol, isDirectReplace);
}

bool IntermediateResult::hasResultBeforeReduction() {
    bool hasResult = true;
    for (auto& a : singleColumnResults) {
        hasResult &= !a.second.empty();
        if (!hasResult) {
            return false;
        }
    }
    return singleColumnResults.empty() && doubleColumnResults.empty() && argClusters.empty() || hasResult;
}

bool IntermediateResult::getResultBoolean() {
    reduceAllResults();
    bool hasResult = true;
    for (auto& a : singleColumnResults) {
        hasResult &= !a.second.empty();
        if (!hasResult) {
            return false;
        }
    }
    return singleColumnResults.empty() && doubleColumnResults.empty() && argClusters.empty() || hasResult;}

CrossProductStruct<QueryElement> *IntermediateResult::getResultSingle(string syn) {
    if (!getResultBoolean()) {
        return new CrossProductStruct<QueryElement>({syn}, {{}});
    }
    vector<vector<QueryElement>> body = {};
    auto set = getSingleColumn(syn);
    for (auto a : set) {
        body.push_back({a});
    }
    return new CrossProductStruct<QueryElement>({syn}, body);
}

bool IntermediateResult::areEqualRows(vector<QueryElement> row1, vector<QueryElement> row2) {
    if (row1.size() != row2.size()) {
        return false;
    }
    int index = 0;
    int size = ::min(row1.size(), row2.size());
    vector<QueryElement>::iterator row1It = row1.begin();
    vector<QueryElement>::iterator row2It = row2.begin();
    while (index < size) {
        row1It += index;
        row2It += index;
        if ((*row1It).TYPE != (*row2It).TYPE) {
            return false;
        }
        assert((*row1It).TYPE == (*row2It).TYPE);
        if ((*row1It) == (*row2It)) {
            index++;
            continue;
        } else {
            return false;
        }
    }
    return true;
}

CrossProductStruct<QueryElement> *IntermediateResult::crossProductWithinCluster(vector<string> header) {
    if (header.size() == 0) {
        return new CrossProductStruct<QueryElement>({""});
    }
    vector<CrossProductStruct<QueryElement>*> tablesToCross = vector<CrossProductStruct<QueryElement>*>();
    unordered_set<string> cluster = *getArgCluster(header.front());
    assert(!cluster.empty());
    for (string s : header) {
        assert(cluster.find(s) != cluster.end());
    }
    unordered_set<string> added = {};
    unordered_set<string> erased = {};
    erased = {};
    for (string s1 : cluster) {
        erased.insert(s1);
        for (string s2 : cluster) {
            if (erased.find(s2) == erased.end() && s1 != s2 && hasDoubleColumn(s1, s2)) {
                tablesToCross.emplace_back(getDoubleColumnCPSPtr(s1,s2));
            }
        }
    }
    CrossProductStruct<QueryElement>* res = new CrossProductStruct<QueryElement>({""});
    *res = SetProcessor::crossProductPtr<QueryElement>(tablesToCross)->limitToColumns(header);
    return res;
}

CrossProductStruct<QueryElement> *IntermediateResult::getResultTuple(vector<string> header) {
    unordered_set<string> headerSet = SetProcessor::vector_to_set<string>(header);
    CrossProductStruct<QueryElement>* res = new CrossProductStruct<QueryElement>(header);
    if (!getResultBoolean()) {
        return res;
    }
    vector<CrossProductStruct<QueryElement>*> tablesToCross = {};
    //add single column results first
    unordered_set<string> added = {};
    for (string s : headerSet) {
        if (getArgCluster(s)->empty()) {
            tablesToCross.emplace_back(getResultSingle(s));
            added.insert(s);
        }
    }
    headerSet = SetProcessor::difference<string>(headerSet, added);
    //clustering
    vector<vector<string>> clusters;
    added = {};
    for (string s : headerSet) {
        vector<string> temp = {};
        unordered_set<string> cluster = *getArgCluster(s);
        for (string s2 : headerSet) {
            if (added.find(s2) == added.end() && cluster.find(s2) != cluster.end()) {
                temp.push_back(s2);
                added.insert(s2);
            }
        }
        if (temp.empty()) {
            continue;
        }
        clusters.push_back(temp);
    }
    //evaluating by cluster
    for (vector<string> cluster : clusters) {
        tablesToCross.push_back(crossProductWithinCluster(cluster));
    }
    return SetProcessor::crossProductPtr<QueryElement>(tablesToCross);
}

bool IntermediateResult::eraseWithCascade(string syn1, QueryElement elem1, string syn2) {
    DoubleColumnResult* dbcol = getDoubleColumn(syn1, syn2);
    element_set deletedElem = dbcol->getAllAssociatedWith(syn1, elem1);
    dbcol->erase(syn1, elem1);
    for (QueryElement to_delete : deletedElem) {
        if (!dbcol->exists(syn2, to_delete)) {
            for (auto& e : getDoubleColumnSetOf(syn2)) {
                if(e.first != syn1)  {
                    eraseWithCascade(syn2, to_delete, e.first);
                }
            }
        }
    }
    return true;
}

bool IntermediateResult::reduceResult(string syn) {
    vector<element_set> ks = {getSingleColumn(syn)};
    for(auto& a : getDoubleColumnSetOf(syn)) {
        ks.push_back(a.second->getKeySet(syn));
    }
    element_set intersect = SetProcessor::intersectOf<QueryElement>(ks);
    for(auto& a : getDoubleColumnSetOf(syn)) {
        for(QueryElement e : a.second->getKeySet(syn)) {
            if (intersect.find(e) == intersect.end()) {
                eraseWithCascade(syn, e, a.first);
            }
        }
    }
    singleColumnResults.insert_or_assign(syn, intersect);
    return true;
}

bool IntermediateResult::reduceAllResults() {
    for(auto& a : doubleColumnResults) {
        reduceResult(a.first);
    }
    //run second time to make sure single column results are correct
    for(auto& a : doubleColumnResults) {
        reduceResult(a.first);
    }
    return true;
}

string IntermediateResult::to_string_single_column() {
    string singleColumns = "";
    for (auto& a : singleColumnResults) {
        string s = a.first + ": ";
        for (QueryElement e : a.second) {
            s += e.to_string() + ", ";
        }
        singleColumns += s + "\n";
    }
    return singleColumns;
}

string IntermediateResult::to_string_arg_clusters() {
    string ag = "";
    for (auto& a : argClusters) {
        ag += a.first + ": ";
        for (auto& a2 : *a.second) {
            ag += a2 + ", ";
        }
        ag += "\n";
    }
    return ag;
}

string IntermediateResult::to_string_double_columns_of(string syn) {
    string res = "";
    for (auto& a : getDoubleColumnSetOf(syn)) {
        res += "table of (" + syn + ", " + a.first + ")\n";
        res += getDoubleColumn(syn, a.first)->table_to_string(syn);
    }
    return res;
}

string IntermediateResult::to_string_arg_cluster_of(string syn) {
    unordered_set<string>* argGroup = getArgCluster(syn);
    string ag = "argument cluster: ";
    for (string a : *argGroup) {
        ag += a + ", ";
    }
    ag += "\n";
    for (string s : *argGroup) {
        ag += to_string_double_columns_of(s);
    }
    return ag;
}

string IntermediateResult::to_string() {
    string argGroups = "argument clusters: \n" + to_string_arg_clusters();
    string singleColumns = "single column results: \n" + to_string_single_column();
    string divider = "";
    string doubleColumns = "";
    unordered_set<string> visited = {};
    for (auto& a : argClusters) {
        if (visited.find(a.first) == visited.end()) {
            for (string s : *a.second) {
                visited.insert(s);
            }
            doubleColumns += to_string_arg_cluster_of(a.first);
        }
    }
    return argGroups + divider + singleColumns + divider + doubleColumns + divider;
}

bool IntermediateResult::clear() {
    singleColumnResults.clear();
    doubleColumnResults.clear();
    argClusters.clear();
    return true;
}
