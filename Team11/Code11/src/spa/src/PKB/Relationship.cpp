#include "Relationship.h"

template<typename V, typename W>
typename Relationship<V, W>::list_of_pairs Relationship<V, W>::getNN(V key1, W key2) {
    return TableNN.get(key1, key2);
}

template<typename V, typename W>
typename Relationship<V, W>::list_of_pairs Relationship<V, W>::getTT(StatementTypes key1, StatementTypes key2) {
    if (key1 == NO_TYPE && key2 == NO_TYPE) {
        return getAll();
    } else if (key1 == NO_TYPE) {
        vector<StatementTypes> innerKeys = TableTTMirrored.getAllInnerKeysOf(key2);
        list_of_pairs to_return;
        for (StatementTypes i : innerKeys) {
            vector<pair<V, W>> temp = TableTTMirrored.get(key2, i);
            to_return.insert(to_return.end(), temp.begin(), temp.end());
        }
        return to_return;
    } else if (key2 == NO_TYPE) {
        vector<StatementTypes> innerKeys = TableTT.getAllInnerKeysOf(key1);
        list_of_pairs to_return;
        for (StatementTypes i : innerKeys) {
            vector<pair<V, W>> temp = TableTT.get(key1, i);
            to_return.insert(to_return.end(), temp.begin(), temp.end());
        }
        return to_return;
    } else {
        return TableTT.get(key1, key2);
    }
}

template<typename V, typename W>
typename Relationship<V, W>::list_of_pairs Relationship<V, W>::getNT(V key1, StatementTypes key2) {
    if (key2 == NO_TYPE) {
        vector<StatementTypes> innerKeys = TableNT.getAllInnerKeysOf(key1);
        list_of_pairs to_return;
        for (StatementTypes i : innerKeys) {
            vector<pair<V, W>> temp = TableNT.get(key1, i);
            to_return.insert(to_return.end(), temp.begin(), temp.end());
        }
        return to_return;
    } else {
        return TableNT.get(key1, key2);
    }
}

template<typename V, typename W>
typename Relationship<V, W>::list_of_pairs Relationship<V, W>::getTN(StatementTypes key1, W key2) {
    if (key1 == NO_TYPE) {
        vector<StatementTypes> innerKeys = TableTNMirrored.getAllInnerKeysOf(key2);
        list_of_pairs to_return;
        for (StatementTypes i : innerKeys) {
            vector<pair<V, W>> temp = TableTNMirrored.get(key2, i);
            to_return.insert(to_return.end(), temp.begin(), temp.end());
        }
        return to_return;
    } else {
        return TableTN.get(key1, key2);
    }
}

template<typename V, typename W>
typename Relationship<V, W>::list_of_pairs Relationship<V, W>::getAll() {
    return TableNN.getAllKeyPairs();
}

template<typename V, typename W>
bool Relationship<V, W>::add(V stmt1, StatementTypes type1, W stmt2, StatementTypes type2) {
    //checks if a pair has been added, if so, don't add again
    if (TableNN.contains(stmt1, stmt2) && TableTN.contains(type1, stmt2)) {
        return true;
    } else if (TableNN.contains(stmt1, stmt2) && !TableTN.contains(type1, stmt2)) {
        return TableTN.add(type1, stmt2, stmt1, stmt2)
              && TableTNMirrored.add(stmt2, type1, stmt1, stmt2)
              && TableTT.add(type1, type2, stmt1, stmt2)
              && TableTTMirrored.add(type2, type1, stmt1, stmt2);
    } else if (TableNN.contains(stmt1, stmt2) && !TableNT.contains(stmt1, type2)) {
        //should not enter by right
        return TableNT.add(stmt1, type2, stmt1, stmt2)
               && TableNTMirrored.add(type2, stmt1, stmt1, stmt2)
               && TableTT.add(type1, type2, stmt1, stmt2)
               && TableTTMirrored.add(type2, type1, stmt1, stmt2);
    } else {
        return TableNN.add(stmt1, stmt2, stmt1, stmt2)
               && TableNNMirrored.add(stmt2, stmt1, stmt1, stmt2)
               && TableNT.add(stmt1, type2, stmt1, stmt2)
               && TableNTMirrored.add(type2, stmt1, stmt1, stmt2)
               && TableTN.add(type1, stmt2, stmt1, stmt2)
               && TableTNMirrored.add(stmt2, type1, stmt1, stmt2)
               && TableTT.add(type1, type2, stmt1, stmt2)
               && TableTTMirrored.add(type2, type1, stmt1, stmt2);

    }
}

template<typename V, typename W>
void Relationship<V, W>::clear() {
    TableNN.clear();
    TableNNMirrored.clear();
    TableTT.clear();
    TableTTMirrored.clear();
    TableNT.clear();
    TableNTMirrored.clear();
    TableTN.clear();
    TableTNMirrored.clear();
}
