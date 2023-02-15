#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "StatementTypes.h"
#include "BinaryTree.h"

class NextExtractor {
private:
    bool processNextIfProgLine(int progLine);
    bool processNextWhileProgLine(int progLine);
    bool processNextIfContainerStatements(int progLine);
    bool processNextIfLastContainerStatement(int progLine, int parent);
    bool processNextIfLastContainerStatementNestedIf(int progLine, int parent);
    int processNextDeeplyNestedIfLastStatement(int progLine, int parent);
    bool processNextWhileContainerStatements(int progLine);
    bool processNextOuterProgLines(int progLine);
public:
    bool extractNext();
};