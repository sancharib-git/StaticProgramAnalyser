#ifndef AUTOTESTER_PROCEDUREPARSER_H
#define AUTOTESTER_PROCEDUREPARSER_H
#include <vector>
#include <string>

class ProcedureParser {

public:
    ProcedureParser() = default;
    static void parseProcedure(const std::string& procString);
private:
    static std::string getProcedureStomach(const std::string& procString);
};


#endif //AUTOTESTER_PROCEDUREPARSER_H
