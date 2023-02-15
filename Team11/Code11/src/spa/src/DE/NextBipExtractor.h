#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include "StatementTypes.h"

class NextBipExtractor {
private:
    bool extractNextBipUtil();
    bool extractNextBipProcedureUtil(std::string procedure, std::unordered_map<std::string, std::vector<int>>& procedureProcessed, std::vector<int>& callStatementsStack);
    bool processNextBipLastStatements(std::vector<int>& callStatementsStack, std::vector<int>& lastStatements);
    bool processNextBipLastStatementsUtil(std::vector<int>& lastStatements, int callNext);
    bool processCallToProcessedProcedure(std::unordered_map<std::string, std::vector<int>>& procedureProcessed, std::pair<int, std::string>& stmtProcedurePair, std::vector<int>& callStatementsStack);
    bool extractNextBipProcedureStatementsUtil(std::pair<std::string, int> procedureStmtPair, std::unordered_map<std::string, std::vector<int>>& procedureProcessed, std::vector<int>& callStatementsStack);
public:
    static bool extractNextBip();
};