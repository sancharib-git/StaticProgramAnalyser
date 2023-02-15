#ifndef AUTOTESTER_CONDEXPRPARSER_H
#define AUTOTESTER_CONDEXPRPARSER_H

#include "StatementTypes.h"

class CondExprParser {
public:
    CondExprParser() = default;
    void parseCondition(const std::string& condString, StatementTypes parent);
    bool checkConditionSyntax(const std::string& condString, StatementTypes parent);

private:
    bool isCondExpr(const std::string& condString, StatementTypes parent);
    bool isRelExpr(const std::string& relString, StatementTypes parent);
    bool isRelFactor(const std::string& relFactor, StatementTypes parent);
    bool isExpr(std::string expr, StatementTypes parent);
    bool isFactor(std::string factor, StatementTypes parent);
    bool isTerm(std::string term, StatementTypes parent);
};


#endif //AUTOTESTER_CONDEXPRPARSER_H
