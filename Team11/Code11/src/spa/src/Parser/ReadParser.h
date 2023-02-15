#pragma once
#include <string>
#include <vector>

#include "Tokeniser.h"

class ReadParser {
public:
	static std::string parseReadStmt(std::string stringtoParse);
	static bool checkReadStmt(vector<std::string> tokens);
};