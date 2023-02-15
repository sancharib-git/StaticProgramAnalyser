#ifndef AUTOTESTER_ASSIGNMENTVALIDATOR_H
#define AUTOTESTER_ASSIGNMENTVALIDATOR_H


class AssignmentValidator {
public:
    AssignmentValidator() = default;
    bool isExpr(std::string expr);
    static bool isValidTokens(std::vector<std::string> tokens);
private:
    bool isTerm(std::string term);
    bool isFactor(std::string factor);
};


#endif //AUTOTESTER_ASSIGNMENTVALIDATOR_H
