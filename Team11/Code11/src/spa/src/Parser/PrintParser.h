#pragma once
#include <string>
#include <vector>

#include "Tokeniser.h"

class PrintParser {
public:
	static std::string parsePrintStmt(std::string stringtoParse);
	static bool checkPrintStmt(vector<std::string> tokens);
};