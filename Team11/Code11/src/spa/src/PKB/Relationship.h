#ifndef AUTOTESTER_RELATIONSHIP_H
#define AUTOTESTER_RELATIONSHIP_H

#include "Table.h"
#include "StatementTypes.h"
using namespace std;

template <typename V, typename W>
class Relationship {
public:
    typedef vector<pair<V, W>> list_of_pairs;
    bool add(V stmt1, StatementTypes type1, W stmt2, StatementTypes type2);
    list_of_pairs getNN(V key1, W key2);
    list_of_pairs getTT(StatementTypes key1, StatementTypes key2);
    list_of_pairs getNT(V key1, StatementTypes key2);
    list_of_pairs getTN(StatementTypes key1, W key2);
    list_of_pairs getAll();
    void clear();

private:
    Table<V, W, V, W> TableNN;
    Table<W, V, V, W> TableNNMirrored;
    Table<StatementTypes, StatementTypes, V, W> TableTT;
    Table<StatementTypes, StatementTypes, V, W> TableTTMirrored;
    Table<V, StatementTypes, V, W> TableNT;
    Table<StatementTypes, V, V, W> TableNTMirrored;
    Table<StatementTypes, W, V, W> TableTN;
    Table<W, StatementTypes, V, W> TableTNMirrored;
};

#endif //AUTOTESTER_RELATIONSHIP_H
