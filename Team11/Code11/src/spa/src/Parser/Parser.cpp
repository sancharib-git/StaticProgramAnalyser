#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include "Parser/Parser.h"
#include "ParserException.h"
#include "ParserUtility.h"
#include "ProcedureParser.h"
using namespace std;

int Parser::lineNum = 0;
std::string Parser::currentProcName;

int Parser::Parse (std::string fileStr)
{
    while (!empty(fileStr)) {
        if(fileStr.find_first_not_of(" \t\n\v\f\r") != std::string::npos) //non-space string
        {
            int finalPosOfProc = ParserUtility::extractBraces(fileStr);
            std::string procedure = fileStr.substr(0, finalPosOfProc+1);
            ProcedureParser::parseProcedure(fileStr);
            fileStr = fileStr.substr(finalPosOfProc+1);
        } else  //filestr is all spaces
        {
            break;
        }
    }
	return 0;
}

/**
 * Reads file into string
 * */
std::string Parser::fileToString(const string &filename)
{
    std::ifstream inFile; //read the file as a string
    try
    {
        inFile.open(filename, std::fstream::in);
    }
    catch (exception& e)
    {
        throw ParserException("Cannot open file");
    }
    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
    std::string fileStr = strStream.str();
    return fileStr;
}
