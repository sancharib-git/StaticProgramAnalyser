#ifndef AUTOTESTER_TABLE_H
#define AUTOTESTER_TABLE_H

#include<stdio.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include <functional>

using namespace std;

template <typename T, typename U, typename V, typename W>
class Table {
public:
    static Table create();
    bool contains(T outerKey, U innerKey);
    vector<pair<V, W> > get(T outerKey, U innerKey);
    bool add(T outerKey, U innerKey, V firstInPair, W secondInPair);
    bool addPair(T outerKey, U innerKey, pair<V, W> pair);
    vector<T> getAllOuterKeys();
    vector<pair<T, U> > getAllKeyPairs();
    vector<U> getAllInnerKeysOf(T outerKey);
    Table();
    void clear();


private:
    unordered_map<T, unordered_map<U, vector<pair<V,W> > > >* table;
    vector<T>* outerKeys;
    vector<pair<T, U> >* keyPairs;
    unordered_map<T, vector<U> >* innerKeysTable;
};

#endif //AUTOTESTER_TABLE_H
