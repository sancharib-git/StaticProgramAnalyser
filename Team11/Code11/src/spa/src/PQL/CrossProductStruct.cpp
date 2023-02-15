#include "CrossProductStruct.h"

using namespace std;
template <typename T>
CrossProductStruct<T>::CrossProductStruct() {
    isValidStruct = false;
}


//return if struct is not empty and header length matches body[0] length
template <typename T>
bool CrossProductStruct<T>::isValid() {
    return isValidStruct && (body.empty() || header.size() == body.front().size());
}


template<class T>
int CrossProductStruct<T>::size() {
    return header.size() * body.size();
}

template<class T>
CrossProductStruct<T> CrossProductStruct<T>::getInvalid() {
    return CrossProductStruct<T>();
}

template<class T>
CrossProductStruct<T>::CrossProductStruct(vector<string> header) {
    vector<string> unique = {};
    int index = 0;
    for (string s : header) {
        if (columnIndices.find(s) == columnIndices.end()) {
            unique.push_back(s);
            columnIndices.insert_or_assign(s, index);
            index++;
        }
    }
    this->header = unique;
}

template<class T>
CrossProductStruct<T>::CrossProductStruct(vector <string> header, vector <vector<T>> body) {
    vector<string> unique = {};
    int index = 0;
    for (string s : header) {
        if (columnIndices.find(s) == columnIndices.end()) {
            unique.push_back(s);
            columnIndices.insert_or_assign(s, index);
            index++;
        }
    }
    this->header = unique;
    this->body = body;
}

template<class T>
bool CrossProductStruct<T>::addRow(vector <T> row) {
    if (!isValid()) {
        return false;
    }
    if (row.size() != header.size()) {
        return false;
    }
    body.push_back(row);
    return true;
}

template<class T>
bool CrossProductStruct<T>::replaceBody(vector<vector<T>> body) {
    if (!isValid()) {
        return false;
    }
    if (!body.empty() && body.front().size() == header.size()) {
        this->body = body;
        return true;
    }
    return false;
}

template<class T>
T CrossProductStruct<T>::getElementOfColumnFromRow(string colHeader, vector<T> row) {
    if (!isValid() || row.size() != header.size() || columnIndices.find(colHeader) == columnIndices.end()) {
        return INVALID;
    }
    int colIndex = columnIndices.at(colHeader);
    return *(row.begin() + colIndex);
}


template<class T>
T CrossProductStruct<T>::getElementOfColumnFromRow(string colHeader, int index) {
    if (!isValid() || columnIndices.find(colHeader) == columnIndices.end() || index >= body.size()) {
        return INVALID;
    }
    vector<T> row = *(body.begin() + index);
    int colIndex = columnIndices.at(colHeader);
    return *(row.begin() + colIndex);
}

template<class T>
CrossProductStruct<T> CrossProductStruct<T>::limitToColumns(vector <string> columnHeaders) {
    if (!isValid()) {
        return *this;
    }
    vector<string> headers = {};
    for (string s : columnHeaders) {
        if (columnIndices.find(s) != columnIndices.end()) {
            headers.push_back(s);
        }
    }
    CrossProductStruct<T> res = CrossProductStruct<T>(headers);
    for (auto& row : body) {
        vector<T> newRow = {};
        for (string s : res.header) {
            newRow.push_back(getElementOfColumnFromRow(s, row));
        }
        res.addRow(newRow);
    }
    return res;
}

template<class T>
string CrossProductStruct<T>::getColumnHeader(int index) {
    if (!isValid() || index >= header.size()) {
        return "";
    }
    return *(header.begin() + index);
}

template<class T>
string CrossProductStruct<T>::to_string_header() {
    string h = "|";
    for (string s : header) {
        h += s + "\t" + "|";
    }
    h += "\n";
    return h;
}

template<class T>
string CrossProductStruct<T>::to_string_body() {
    string b = "";
    for (auto a : body) {
        b += "|";
        for (auto e : a) {
            b += e.to_string() + "\t" + "|";
        }
        b += "\n";
    }
    return b;
}

template<class T>
string CrossProductStruct<T>::to_string() {
    string divider = "/////////////////\n";
    string h = to_string_header();
    string b = to_string_body();
    return divider + h + b + divider;
}


