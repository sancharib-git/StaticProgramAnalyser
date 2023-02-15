#pragma once
#include <string>
#include <vector>

#include "Tokeniser.h"

class CallParser {
public:
	static std::string parseCallStmt(std::string stringtoParse);
	static bool checkCallStmt(vector<std::string> tokens);
};