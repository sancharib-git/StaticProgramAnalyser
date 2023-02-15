#include "DoubleColumnResult.h"
#include "CrossProductStruct.cpp"

DoubleColumnResult::DoubleColumnResult(string syn1, string syn2) {
    synonymPair = make_pair(syn1, syn2);
    tables.insert_or_assign(syn1, new double_column_table());
    tables.insert_or_assign(syn2, new double_column_table());
    keySets.insert_or_assign(syn1, column());
    keySets.insert_or_assign(syn2, column());
}

DoubleColumnResult::DoubleColumnResult(string syn1, string syn2, vector<pair<QueryElement, QueryElement>> list) {
    synonymPair = make_pair(syn1, syn2);
    tables.insert_or_assign(syn1, new double_column_table());
    tables.insert_or_assign(syn2, new double_column_table());
    keySets.insert_or_assign(syn1, column());
    keySets.insert_or_assign(syn2, column());
    for (auto a : list) {
        this->add(syn1, syn2, a.first, a.second);
    }
}

DoubleColumnResult::DoubleColumnResult() {
    isFine = false;
}

bool DoubleColumnResult::inPair(string a) {
    return (a == synonymPair.first || a == synonymPair.second);
}

string DoubleColumnResult::otherInPair(string a) {
    assert(inPair(a));
    if (a == synonymPair.first) {
        return synonymPair.second;
    } else {
        return synonymPair.first;
    }
}

DoubleColumnResult::double_column_table *DoubleColumnResult::getTable(string syn) {
    if (tables.find(syn) != tables.end()) {
        return tables.at(syn);
    }
    return new double_column_table();
}

DoubleColumnResult::column DoubleColumnResult::getColumnFromTable(DoubleColumnResult::double_column_table *d, QueryElement key) {
    if (d->find(key) != d->end()) {
        return d->at(key);
    }
    return column();
}

DoubleColumnResult::column DoubleColumnResult::getKeySet(string syn) {
    if (keySets.find(syn) != keySets.end()) {
        return keySets.at(syn);
    }

    return column();
}

bool DoubleColumnResult::isNew() {
    return !used;
}

bool DoubleColumnResult::isValid() {
    return isFine;
}

int DoubleColumnResult::size() {
    return _size;
}

bool DoubleColumnResult::add(string syn1, string syn2, QueryElement key1, QueryElement key2) {
    used = true;
    if (!inPair(syn1) || !inPair(syn2)) {
        return false;
    }
    assert(tables.find(syn1) != tables.end() && tables.find(syn2) != tables.end());
    //add to table
    addSingleSide(syn1, syn2, key1, key2);
    addSingleSide(syn2, syn1, key2, key1);

    //update key set
    addToKey(syn1, key1);
    addToKey(syn2, key2);

    _size++;

    return true;
}

bool DoubleColumnResult::exists(string syn1, string syn2, QueryElement key1, QueryElement key2) {
    column localColumnSyn1 = getColumnFromTable(getTable(syn1), key1);
    column localColumnSyn2 = getColumnFromTable(getTable(syn2), key2);
    return localColumnSyn1.find(key2) != localColumnSyn1.end()
           && localColumnSyn2.find(key1) != localColumnSyn2.end();
}

bool DoubleColumnResult::exists(string syn, QueryElement key) {
    return (keySets.find(syn) != keySets.end() && keySets.at(syn).find(key) != keySets.at(syn).end());
}

DoubleColumnResult::column DoubleColumnResult::getAllAssociatedWith(string syn, QueryElement key) {
    return getColumnFromTable(getTable(syn), key);
}

bool DoubleColumnResult::erase(string syn1, string syn2, QueryElement key1, QueryElement key2) {
    if (!inPair(syn1) || !inPair(syn2)) {
        return false;
    }
    //erase from table
    eraseSingleSide(syn1, syn2, key1, key2);
    eraseSingleSide(syn2, syn1, key2, key1);

    //update key set
    eraseKey(syn1, key1);
    eraseKey(syn2, key2);

    _size--;

    return true;
}

bool DoubleColumnResult::erase(string syn, QueryElement key) {
    if (!inPair(syn)) {
        return false;
    }
    double_column_table* table = getTable(syn);
    column col = getColumnFromTable(table, key);
    for (QueryElement q : col) {
        erase(syn, otherInPair(syn), key, q);
    }
    return true;
}

string DoubleColumnResult::table_to_string(string syn) {
    string header = "|" + syn + "\t|" + otherInPair(syn) + "\t|\n";
    string divider = "/////////\n";
    double_column_table table = *getTable(syn);
    string body = "";
    for (auto& a : table) {
        for (QueryElement e : a.second) {
            body += "|" + a.first.to_string() + "\t|" + e.to_string() + "\t|\n";
        }
    }
    return divider + header + body + divider;
}

string DoubleColumnResult::keyset_to_string(string syn) {
    string res = syn + ": ";
    for (QueryElement e : getKeySet(syn)) {
        res += e.to_string() + ", ";
    }
    return res + "\n";
}

string DoubleColumnResult::to_string() {
    return keyset_to_string(synonymPair.first) + keyset_to_string(synonymPair.second)
           + table_to_string(synonymPair.first) + table_to_string(synonymPair.second);
}

CrossProductStruct<QueryElement> DoubleColumnResult::convertToCrossProductStruct() {
    return convertToCrossProductStruct(synonymPair.first, synonymPair.second);
}

CrossProductStruct<QueryElement> DoubleColumnResult::convertToCrossProductStruct(string syn1, string syn2) {
    CrossProductStruct<QueryElement> res =
            CrossProductStruct<QueryElement>({syn1, syn2});
    for (auto& a1 : *getTable(syn1)) {
        for (auto& a2 : a1.second) {
            res.addRow({a1.first, a2});
        }
    }
    return res;
}

CrossProductStruct<QueryElement> *DoubleColumnResult::convertToCrossProductStructPtr(string syn1, string syn2) {

    CrossProductStruct<QueryElement>* res =
            new CrossProductStruct<QueryElement>({syn1, syn2});
    for (auto& a1 : *getTable(syn1)) {
        for (auto& a2 : a1.second) {
            res->addRow({a1.first, a2});
        }
    }
    return res;
}

DoubleColumnResult * DoubleColumnResult::doubleColumnCrossProductStructToDoubleColumnResult(CrossProductStruct<QueryElement> cps) {
    if (!cps.isValid() || cps.header.size() != 2) {
        return new DoubleColumnResult();
    }
    assert(cps.header.size() == 2);
    string syn1 = *(cps.header.begin());
    string syn2 = *(cps.header.begin() + 1);
    DoubleColumnResult* res = new DoubleColumnResult(syn1, syn2);
    for (vector<QueryElement> row : cps.body) {
        assert(row.size() == 2);
        res->add(syn1, syn2, *(row.begin()), *(row.begin() + 1));
    }
    return res;
}

DoubleColumnResult DoubleColumnResult::intersectOf(DoubleColumnResult table1, DoubleColumnResult table2) {
    string syn1 = table1.synonymPair.first;
    string syn2 = table1.synonymPair.second;
    DoubleColumnResult intersect = DoubleColumnResult(syn1, syn2);
    unordered_set<QueryElement> intersectKey1 =
            SetProcessor::intersectOf<QueryElement>(
                    {table1.getKeySet(syn1),
                     table2.getKeySet(syn1)});
    unordered_set<QueryElement> intersectKey2 =
            SetProcessor::intersectOf<QueryElement>(
                    {table1.getKeySet(syn2),
                     table2.getKeySet(syn2)});
    for (auto e1 : intersectKey1) {
        unordered_set<QueryElement> associatedElem = table1.getAllAssociatedWith(syn1, e1);
        for (auto e2 : associatedElem) {
            if (table2.exists(syn1, syn2, e1, e2)) {
                intersect.add(syn1, syn2, e1, e2);
            }
        }
    }
    return intersect;
}

bool DoubleColumnResult::addSingleSide(string syn1, string syn2, QueryElement key1, QueryElement key2) {
    if (!inPair(syn1) || !inPair(syn2)) {
        return false;
    }
    assert(tables.find(syn1) != tables.end() && tables.find(syn2) != tables.end());
    //add to table
    double_column_table* table = getTable(syn1);
    column col = getColumnFromTable(table, key1);
    col.insert(key2);
    table->insert_or_assign(key1, col);
    tables.insert_or_assign(syn1, table);
    return true;
}

bool DoubleColumnResult::addToKey(string syn, QueryElement key) {
    column keySet1 = getKeySet(syn);
    keySet1.insert(key);
    keySets.insert_or_assign(syn, keySet1);
    return true;
}

bool DoubleColumnResult::eraseSingleSide(string syn1, string syn2, QueryElement key1, QueryElement key2) {
    if (!inPair(syn1) || !inPair(syn2)) {
        return false;
    }
    double_column_table* table = getTable(syn1);
    column col = getColumnFromTable(table, key1);
    col.erase(key2);
    table->insert_or_assign(key1, col);
    tables.insert_or_assign(syn1, table);
    return true;
}

bool DoubleColumnResult::eraseKey(string syn, QueryElement key) {
    double_column_table* table = getTable(syn);
    if (getColumnFromTable(table, key).empty()) {
        table->erase(key);
        tables.insert_or_assign(syn, table);
        column keySet = getKeySet(syn);
        keySet.erase(key);
        keySets.insert_or_assign(syn, keySet);
    }
    return true;
}

