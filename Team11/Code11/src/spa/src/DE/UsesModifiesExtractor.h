#pragma once
#include <string>
#include <utility>
#include <unordered_map>
#include <unordered_set>

class UsesModifiesExtractor {
private:
    bool addUsesModifies();
    bool addUsesModifiesUtilProcedure(std::string procedure, std::unordered_set<std::string>& proceduresCalled);
    bool processCallUsesModifies(int statementNumber, std::unordered_set<std::string>& proceduresCalled);
    bool addUsesUtilCalls(int callStatementNumber, std::string procedureCalled);
    bool addModifiesUtilCalls(int callStatementNumber, std::string procedureCalled);
    bool addUsesVarProc(std::unordered_set<std::string>& proceduresCalled, int statementNumber);
    bool addModifiesVarProc(std::unordered_set<std::string>& proceduresCalled, int statementNumber);
    bool addUsesCallsContainer(int callStatementNumber);
    bool addModifiesCallsContainer(int callStatementNumber);
    bool checkSemanticErrorForProcedures(std::unordered_set<std::string>& proceduresCalled, std::string procedure);

public:
    UsesModifiesExtractor() {};
    bool extractUsesModifies();
};