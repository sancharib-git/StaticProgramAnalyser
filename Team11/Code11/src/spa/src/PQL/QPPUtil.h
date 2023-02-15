//
// Created by Xuan Zhi on 6/11/21.
//

#ifndef AUTOTESTER_QPPUTIL_H
#define AUTOTESTER_QPPUTIL_H
#include <stdio.h>
#include <string>
#include <utility>
#include <stack>
#include <vector>

namespace QPPUtil {

    int findSmallestPositive(std::vector<size_t> vect);
    int findNextClausePos(std::vector<size_t> vector);
    size_t getSmallestPos(size_t spacePos, size_t bracePos);
    std::pair<std::string, std::string> extractQuery(int min, std::string query);
    std::string stripExtraBracketsInSubString(std::string subexpr);

    std::string& leftTrim(std::string& str);
    std::string& rightTrim(std::string& str);
    std::string& trimAllWhitespaces(std::string& str);
    std::string& stripArgument(std::string& str);
    std::string& stripAllWhiteSpaces(std::string& str);
};


#endif //AUTOTESTER_QPPUTIL_H
