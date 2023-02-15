//
// Created by Yen Peng Yip on 21/10/21.
//

#ifndef AUTOTESTER_DEFINITIONS_H
#define AUTOTESTER_DEFINITIONS_H
#include <unordered_set>

typedef std::unordered_set<int> UNORDERED_LIST_OF_INT;
typedef std::unordered_set<std::string> UNORDERED_LIST_OF_STR;

typedef std::vector<int> LIST_OF_INT;
typedef std::vector<std::string> LIST_OF_STR;

typedef std::vector<std::pair<int, int> > LIST_OF_INT_PAIRS;
typedef std::vector<std::pair<int, std::string> > LIST_OF_INTSTR_PAIRS;
typedef std::vector<std::pair<std::string, std::string> > LIST_OF_STR_PAIRS;

#endif //AUTOTESTER_DEFINITIONS_H
