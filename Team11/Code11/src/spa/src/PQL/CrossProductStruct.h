#ifndef AUTOTESTER_CROSSPRODUCTSTRUCT_H
#define AUTOTESTER_CROSSPRODUCTSTRUCT_H
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
////////////////////////////////////// custom structs //////////////////////////////////////
template <class T>
struct CrossProductStruct {
public:
    vector<string> header;
    vector<vector<T>> body;
    unordered_map<string, int> columnIndices = {};
    T INVALID;
    bool isValidStruct = true;

    //return if struct is not empty and header length matches body[0] length
    bool isValid();

    int size();

    static CrossProductStruct<T> getInvalid();

    CrossProductStruct(vector<string> header);

    CrossProductStruct(vector<string> header, vector<vector<T>> body);

    bool addRow(vector<T> row);

    bool replaceBody(vector<vector<T>> body);

    //should be constant time
    T getElementOfColumnFromRow(string colHeader, vector<T> row);
    T getElementOfColumnFromRow(string colHeader, int index);

    //will return duplicate
    CrossProductStruct<T> limitToColumns(vector<string> columnHeaders);

    string getColumnHeader(int index);

    string to_string_header();

    string to_string_body();

    string to_string();

private:
    CrossProductStruct();
};

#endif //AUTOTESTER_CROSSPRODUCTSTRUCT_H
