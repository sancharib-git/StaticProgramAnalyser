#include "AddToPKB.h"
#include "BlockStmtParser.h"
#include "PKB/PKB.h"
using namespace std;

/**
 * Populates design abstractions for all child statements of this block
 */
void BlockStmtParser::populateInnerAbstractions(int innerConditionNum, int conditionStmtNum, StatementTypes parentType)
{
    PKB& sharedPkb = PKB::getInstance();
    vector<pair<int, string>> innerUses = sharedPkb.getUsesSST(innerConditionNum, NO_TYPE);
    for (int i = 0; i < innerUses.size(); i++)
    {
        pair<int, string> currentPair = innerUses[i];
        string variableUsed = currentPair.second;
        AddToPKB::addUsesSRelation(conditionStmtNum, parentType, variableUsed);
    }
    vector<pair<int, string>> innerModifies = sharedPkb.getModifiesSST(innerConditionNum, NO_TYPE);
    for (int i = 0; i < innerModifies.size(); i++)
    {
        pair<int, string> currentPair = innerModifies[i];
        string variableUsed = currentPair.second;
        AddToPKB::addModifiesSRelation(conditionStmtNum, parentType, variableUsed);
    }
}
