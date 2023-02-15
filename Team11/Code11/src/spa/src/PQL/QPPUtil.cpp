//
// Created by Xuan Zhi on 6/11/21.
//

#include "QPPUtil.h"
#include "TreeProcessor.h"
#include <stdio.h>
#include <string>
#include <utility>
#include <stack>
#include <vector>

using namespace QPPUtil;

int QPPUtil::findSmallestPositive(std::vector<size_t> vect) {
    int min = INT_MAX;
    for (int i : vect) {
        if (i >= 0 && i < min) {
            min = i;
        }
    }
    return min;
}

int QPPUtil::findNextClausePos(std::vector<size_t> vector) {
    int min = findSmallestPositive(vector);
    if (min == INT_MAX) {
        return -1;
    }
    return min;
}

size_t QPPUtil::getSmallestPos(size_t spacePos, size_t bracePos) {
    if (bracePos == std::string::npos) {
        return -1;
    } else if (spacePos != std::string ::npos && spacePos < bracePos) {
        return spacePos;
    } else if (bracePos < spacePos || (bracePos >=0 && spacePos == std::string::npos)) {
        return bracePos;
    } else {
        return -1;
    }
}

std::pair<std::string, std::string> QPPUtil::extractQuery(int min, std::string query) {
    std::string extracted = query.substr(0, min);
    QPPUtil::trimAllWhitespaces(extracted);

    std::string remaining = query.erase(0, min);
    QPPUtil::trimAllWhitespaces(remaining);
    return make_pair(extracted, remaining);
}

std::string QPPUtil::stripExtraBracketsInSubString(std::string subexpr) {
    QPPUtil::stripArgument(subexpr);
    QPPUtil::stripAllWhiteSpaces(subexpr);
    TreeProcessor* treeProcessor = new TreeProcessor();
    BinaryTree* rhsAST  = treeProcessor->buildASTFromSubexpr(subexpr);
    std::string processedString = treeProcessor->treeInString(*rhsAST);
    return processedString;
}

/**
 * Trim whitespaces of a string from the front
 * @param str
 * @return string
 */
std::string& QPPUtil::leftTrim(std::string &str) {
    str.erase(0,str.find_first_not_of(" "));
    return str;
}

/**
 * Trim whitespaces of a string from the back
 * @param str
 * @return string
 */
std::string& QPPUtil::rightTrim(std::string &str) {
    str.erase(str.find_last_not_of(" ") + 1);
    return str;
}

/**
 * Trim whitespaces of a string from both front and back
 * @param str
 * @return string
 */
std::string& QPPUtil::trimAllWhitespaces(std::string &str) {
    leftTrim(str);
    rightTrim(str);
    return str;
}

std::string& QPPUtil::stripArgument(std::string &str) {
    if (str.find("_\"") == 0) {
        str = str.substr(2);
        str = str.substr(0, str.size() - 2);
    } else if (str.find("\"") == 0) {
        str = str.substr(1);
        str = str.substr(0, str.size() - 1);
    }
    trimAllWhitespaces(str);
    return str;
}

std::string& QPPUtil::stripAllWhiteSpaces(std::string& str) {
    std::string finalString = "";
    for (char c : str) {
        if (!isspace(c)) {
            finalString.push_back(c);
        }
    }
    str = finalString;
    return str;
}