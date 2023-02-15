#include <vector>
#include <string>
#include "Parser/Parser.h"
#include "BlockStmtParser.h"
#include <string>
#include "StatementTypes.h"

using namespace std;

class WhileParser : public BlockStmtParser
{
public:
    WhileParser() = default;
    static int parseWhile(const string& whileBlock, int parent, StatementTypes parentType, StatementTypes blockType);
private:
    static void addWhileInfoToPKB(int condLineNum, int parent, StatementTypes parentType, StatementTypes blockType);

};
