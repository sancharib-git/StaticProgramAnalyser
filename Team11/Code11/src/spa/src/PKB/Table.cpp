#include "Table.h"
#include <vector>
#include <unordered_map>
using namespace std;

template <typename T, typename U, typename V, typename W>
Table<T,U,V,W>::Table() {
    table = new unordered_map<T, unordered_map<U, vector<pair<V, W> > > >();
    outerKeys = new vector<T>();
    keyPairs = new vector<pair<T, U> >();
    innerKeysTable = new unordered_map<T, vector<U> >();
};

template <typename T, typename U, typename V, typename W>
Table<T, U, V, W> Table<T, U, V, W>::create() {
    return new Table<T, U, V, W>();
}

template<typename T, typename U, typename V, typename W>
bool Table<T, U, V, W>::contains(T outerKey, U innerKey) {
    if (table->find(outerKey) != table->end()) {
        return table->at(outerKey).find(innerKey) != table->at(outerKey).end();
    }
    return false;
}

template<typename T, typename U, typename V, typename W>
vector<std::pair<V, W> > Table<T, U, V, W>::get(T outerKey, U innerKey) {
    if (table->find(outerKey) != table->end()) {
        if (table->at(outerKey).find(innerKey) != table->at(outerKey).end()) {
            return table->at(outerKey).at(innerKey);
        }
    }
    return vector<pair<V,W> >();
}

template<typename T, typename U, typename V, typename W>
bool Table<T, U, V, W>::add(T outerKey, U innerKey, V firstInPair, W secondInPair) {
    vector<pair<V,W> > list = get(outerKey, innerKey);
    list.push_back(pair<V,W>(firstInPair, secondInPair));
    if (table->find(outerKey) == table->end()) {
        outerKeys->push_back(outerKey);
        keyPairs->push_back(pair<T, U>(outerKey, innerKey));
        vector<U> innerKeyList;
        innerKeyList.push_back(innerKey);
        innerKeysTable->insert_or_assign(outerKey, innerKeyList);
        table->insert_or_assign(outerKey, unordered_map<U, vector<pair<V,W> > >());
    } else if (table->at(outerKey).find(innerKey) == table->at(outerKey).end()){
        keyPairs->push_back(pair<T, U>(outerKey, innerKey));
        vector<U> innerKeyList = innerKeysTable->at(outerKey);
        innerKeyList.push_back(innerKey);
        innerKeysTable->insert_or_assign(outerKey, innerKeyList);
    }
    table->at(outerKey).insert_or_assign(innerKey, list);
    return true;
}

template<typename T, typename U, typename V, typename W>
bool Table<T, U, V, W>::addPair(T outerKey, U innerKey, pair<V, W> pair) {
    return add(outerKey, innerKey, pair.first, pair.second);
}

template<typename T, typename U, typename V, typename W>
vector<T> Table<T, U, V, W>::getAllOuterKeys() {
    return *outerKeys;
}

template<typename T, typename U, typename V, typename W>
vector<pair<T, U> > Table<T, U, V, W>::getAllKeyPairs() {
    return *keyPairs;
}

template<typename T, typename U, typename V, typename W>
vector<U> Table<T, U, V, W>::getAllInnerKeysOf(T outerKey) {
    return innerKeysTable->find(outerKey) != innerKeysTable->end() ? innerKeysTable->at(outerKey) : vector<U>();
}

template <typename T, typename U, typename V, typename W>
void Table<T, U, V, W>::clear() {
    table->clear();
    outerKeys->clear();
    keyPairs->clear();
    innerKeysTable->clear();
}
