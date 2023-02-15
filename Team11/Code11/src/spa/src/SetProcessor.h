#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "PQL/QueryElement.h"
#include "PQL/CrossProductStruct.h"
#ifndef AUTOTESTER_SETPROCESSOR_H
#define AUTOTESTER_SETPROCESSOR_H

using namespace std;
class SetProcessor {
public:
    ////////////////////////////////////// helper functions //////////////////////////////////////
    template<class T>
    static unordered_set<T> intersectOf(vector<unordered_set<T>> list) {
        int min_length = -1;
        unordered_set<T> shortest;
        for (unordered_set<T> set : list) {
            if (min_length == -1) {
                shortest = set;
                min_length = set.size();
            }
            if (set.size() < min_length) {
                shortest = set;
                min_length = set.size();
            }
        }
        unordered_set<T> intersect = {};
        for(T e : shortest) {
            bool allMatch = true;
            for (unordered_set<T> set : list) {
                allMatch = allMatch && set.find(e) != set.end();
            }
            if (allMatch) {
                intersect.insert(e);
            }
        }
        return intersect;
    }

    template<class T>
    static vector<T> intersectOf(vector<vector<T>> list) {
        vector<unordered_set<T>> intermediate = {};
        for (vector<T> v : list) {
            intermediate.push_back(vector_to_set(v));
        }
        return set_to_vector(intersectOf<T>(intermediate));
    }

    template<class T>
    static unordered_set<T> unionOf(vector<unordered_set<T>> list) {
        unordered_set<T> un = {};
        for (auto& a : list) {
            for (T t : a) {
                un.insert(t);
            }
        }
        return un;
    }

    template<class T>
    static vector<T> unionOf(vector<vector<T>> list) {
        unordered_set<T> un = {};
        vector<T> res = {};
        for (auto& a : list) {
            for (T t : a) {
                un.insert(t);
            }
        }
        for (T t : un) {
            res.push_back(t);
        }
        return res;
    }

    //returns elements only in set1 i.e. set1 - set2
    template<class T>
    static unordered_set<T> difference(unordered_set<T> set1, unordered_set<T> set2) {
        unordered_set<T> intersect = intersectOf<T>({set1, set2});
        unordered_set<T> diff = {};
        for (T t : set1) {
            if (intersect.find(t) == intersect.end()) {
                diff.insert(t);
            }
        }
        return diff;
    }

    //returns elements only in vector1 i.e. vector1 - vector2
    template<class T>
    static vector<T> difference(vector<T> vector1, vector<T> vector2) {
        return set_to_vector(difference(vector_to_set(vector1), vector_to_set(vector2)));
    }

    template<class T>
    static unordered_set<T> vector_to_set(vector<T> vector) {
        unordered_set<T> res = {};
        for (T t : vector) {
            res.insert(t);
        }
        return res;
    }

    template<class T>
    static vector<T> set_to_vector(unordered_set<T> set) {
        vector<T> res = {};
        for (T t : set) {
            res.push_back(t);
        }
        return res;
    }

    template<typename T, typename S>
    static S getOrDefaultFrom(unordered_map<T, S> set, T key, S defaultVal) {
        if (set.find(key) != set.end()) {
            return set.at(key);
        }
        return defaultVal;
    }

    template<class T>
    static CrossProductStruct<T> crossProduct(CrossProductStruct<T> group1, CrossProductStruct<T> group2) {
        if (!group1.isValid() || !group2.isValid()) {
            return CrossProductStruct<T>(vector<string>());
        }
        vector<string> commonColumns = SetProcessor::intersectOf<string>({group1.header, group2.header});
        vector<string> inGroup1Only = SetProcessor::difference<string>(group1.header, group2.header);
        vector<string> inGroup2Only = SetProcessor::difference<string>(group2.header, group1.header);
        vector<string> unionColumns = commonColumns;
        unionColumns.insert(unionColumns.end(), inGroup1Only.begin(), inGroup1Only.end());
        unionColumns.insert(unionColumns.end(), inGroup2Only.begin(), inGroup2Only.end());
        CrossProductStruct<T> res = CrossProductStruct<T>(unionColumns);
        for (auto a1 : group1.body) {
            for (auto a2 : group2.body) {
                bool allMatch = true;
                for (string s : commonColumns) {
                    allMatch &= group1.getElementOfColumnFromRow(s, a1) == group2.getElementOfColumnFromRow(s, a2);
                    if (!allMatch) {
                        break;
                    }
                }
                if (allMatch) {
                    vector<T> temp = {};
                    for (string s : commonColumns) {
                        temp.push_back(group1.getElementOfColumnFromRow(s, a1));
                    }
                    for (string s : inGroup1Only) {
                        temp.push_back(group1.getElementOfColumnFromRow(s, a1));
                    }
                    for (string s : inGroup2Only) {
                        temp.push_back(group2.getElementOfColumnFromRow(s, a2));
                    }
                    res.addRow(temp);
                }
            }
        }
        return res;
    }

    template<class T>
    static CrossProductStruct<T>* crossProductPtr(CrossProductStruct<T>* group1, CrossProductStruct<T>* group2) {
        if (!group1->isValid() || !group2->isValid()) {
            return new CrossProductStruct<T>(vector<string>());
        }
        vector<string> commonColumns = SetProcessor::intersectOf<string>({group1->header, group2->header});
        vector<string> inGroup1Only = SetProcessor::difference<string>(group1->header, group2->header);
        vector<string> inGroup2Only = SetProcessor::difference<string>(group2->header, group1->header);
        vector<string> unionColumns = commonColumns;
        unionColumns.insert(unionColumns.end(), inGroup1Only.begin(), inGroup1Only.end());
        unionColumns.insert(unionColumns.end(), inGroup2Only.begin(), inGroup2Only.end());
        CrossProductStruct<T>* res = new CrossProductStruct<T>(unionColumns);
        for (auto a1 : group1->body) {
            for (auto a2 : group2->body) {
                bool allMatch = true;
                for (string s : commonColumns) {
                    allMatch &= group1->getElementOfColumnFromRow(s, a1) == group2->getElementOfColumnFromRow(s, a2);
                    if (!allMatch) {
                        break;
                    }
                }
                if (allMatch) {
                    vector<T> temp = {};
                    for (string s : commonColumns) {
                        temp.push_back(group1->getElementOfColumnFromRow(s, a1));
                    }
                    for (string s : inGroup1Only) {
                        temp.push_back(group1->getElementOfColumnFromRow(s, a1));
                    }
                    for (string s : inGroup2Only) {
                        temp.push_back(group2->getElementOfColumnFromRow(s, a2));
                    }
                    res->addRow(temp);
                }
            }
        }
        return res;
    }

    template<class T>
    static CrossProductStruct<T> crossProduct(vector<CrossProductStruct<T>> toCross) {
        if (toCross.empty()) {
            return CrossProductStruct<T>(vector<string>());
        }
        sort(toCross.begin(), toCross.end(), [](CrossProductStruct<T> cps1, CrossProductStruct<T> cps2) {return cps1.size() < cps2.size();});
        CrossProductStruct<T> res = toCross.front();
        bool atFront = true;
        for (auto a : toCross) {
            if (atFront) {
                atFront = false;
                continue;
            }
            res = crossProduct(res, a);
        }
        return res;
    }

    template<class T>
    static CrossProductStruct<T>* crossProductPtr(vector<CrossProductStruct<T>*> toCross) {
        if (toCross.empty()) {
            return new CrossProductStruct<T>(vector<string>());
        }
        sort(toCross.begin(), toCross.end(), [](CrossProductStruct<T>* cps1, CrossProductStruct<T>* cps2) {return cps1->size() < cps2->size();});
        CrossProductStruct<T>* res = toCross.front();
        bool atFront = true;
        for (auto a : toCross) {
            if (atFront) {
                atFront = false;
                continue;
            }
            res = crossProductPtr(res, a);
        }
        return res;
    }

    //for testing
    template<class T>
    static CrossProductStruct<T> crossProduct(vector<string> header1, vector<vector<T>> body1,
                                              vector<string> header2, vector<vector<T>> body2) {
        return crossProduct(CrossProductStruct(header1, body1),
                            CrossProductStruct(header2, body2));
    }

    static string to_string(vector<QueryElement> list) {
        string s;
        for (auto a : list) {
            s += a.to_string() + " ";
        }
        s += "\n";
        return s;
    }

    static string to_string(vector<string> list) {
        string s;
        for (auto a : list) {
            s += a + " ";
        }
        s += "\n";
        return s;
    }
};


#endif //AUTOTESTER_SETPROCESSOR_H
