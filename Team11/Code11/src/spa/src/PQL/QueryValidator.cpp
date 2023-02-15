#include "QueryValidator.h"
#include "Query.h"
#include "Parser/Tokeniser.h"
#include "QPPUtil.h"
#include <utility>
#include <regex>
#include <stack>

// identity is synonym
const std::regex SYNONYM("[A-Za-z][a-zA-Z_0-9]*");
const std::regex INTEGER("^0$|^[1-9][0-9]*$");
const string WILDCARD = "_";
const regex IDENT_WITH_QUOTES("^\"\\s*[A-Za-z][a-zA-Z_0-9]*\\s*\"$");
const std::regex STMT_REF("[A-Za-z][a-zA-Z_0-9]*|^0$|^[1-9][0-9]*$|^_$");
const std::regex ENT_REF("[A-Za-z][a-zA-Z_0-9]*|^_$|^\"\\s*[A-Za-z][a-zA-Z_0-9]*\\s*\"$");
const std::regex EXP_SPEC_NAME("^\"\\s*[A-Za-z][a-zA-Z_0-9]*\\s*\"$|^_\"\\s*[A-Za-z][a-zA-Z_0-9]*\\s*\"_$");
const std::regex EXP_SPEC_INT("^\"\\s*[0]\\s*\"$|^_\"\\s*[0]\\s*\"_$|^\"\\s*[1-9][0-9]*\\s*\"$|^_\"\\s*[1-9][0-9]*\\s*\"_$");
const std::regex FACTOR("[A-Za-z][a-zA-Z_0-9]*|^0$|^[1-9][0-9]*$");
const std::regex ASSIGN("^\"\\s*.*\\s*\"$|^_\"\\s*.*\\s*\"_$");

Query &qe = Query::getInstance();

bool QueryValidator::isValidSynRegex(string syn) {
    return regex_match(syn, SYNONYM);
}

bool QueryValidator::isDeclarationValid(Query::entityType entityType, string synonym) {
    if (!regex_match(synonym, SYNONYM)) {
        qe.setSyntaxError();
        return false;
    }
    if (entityType == Query::entityType::wrong_type) {
        qe.setSemanticError();
        return false;
    }
    if (qe.isVarDeclared(synonym)) {
        qe.setSemanticError();
        return false;
    }
    return true;
}

bool QueryValidator::isSelectExist(string query) {
    query.erase(0, query.find_first_not_of(" "));
    if (query.find("Select ") == 0) {
        return true;
    }
    qe.setSyntaxError();
    return false;
}

Query::selectFlag QueryValidator::isSelectVarValid(string selectVar, string attrRef) {
    if (!qe.isVarDeclared(selectVar)) {
        return Query::selectFlag::isInvalid;
    }
    Query::entityType entityType = qe.getEntityTypeBySynName(selectVar);
    QueryClause::attribute attr = QueryClause::getAttributeType(attrRef);

    if (attr == QueryClause::attribute::procName) {
        if (entityType == Query::entityType::procedure || entityType == Query::entityType::call) {
            return Query::selectFlag::isProcName;
        }
    } else if (attr == QueryClause::attribute::varName) {
        if (entityType == Query::entityType::var || entityType == Query::entityType::read ||
            entityType == Query::entityType::print) {
            return Query::selectFlag::isVarName;
        }
    } else if (attr == QueryClause::attribute::value) {
        if (entityType == Query::entityType::constant) {
            return Query::selectFlag::isSynonymName;
        }
    } else {
        // stmtNum
        if (entityType != Query::entityType::var && entityType != Query::entityType::constant &&
            entityType != Query::entityType::procedure) {
            return Query::selectFlag::isSynonymName;
        }
    }
    return Query::selectFlag::isInvalid;
}

bool QueryValidator::isSelectVarDeclared(string selectVar) {
    if (qe.isVarDeclared(selectVar)) {
        return true;
    }
    return false;
}

bool QueryValidator::isValidTuple(string var) {
    if (var.find("<") != 0 || var.find(">") == string::npos) {
        qe.setSyntaxError();
        return false;
    }

    return true;
}

bool QueryValidator::isValidPattern(string syn, vector<string> argument) {

    Query::entityType entityType = qe.getEntityTypeBySynName(syn);
    if (entityType == Query::entityType::wrong_type) {
        qe.setSyntaxError();
    }
    if (entityType != Query::entityType::assign && entityType != Query::entityType::whiles &&
        entityType != Query::entityType::ifl) {
        qe.setSemanticError();
        return false;
    }
    if (!qe.isVarDeclared(syn)) {
        return false;
    }
    if (entityType == Query::entityType::assign) {
        return isValidPatternAssign(argument);
    } else if (entityType == Query::entityType::whiles) {
        return isValidPatternWhile(argument);
    } else {
        return isValidPatternIf(argument);
    }
}

bool isValidOpOrFactor(int opPos, string &assignment, QueryValidator::assignType prevType) {
    if (opPos == 0) {
        if (prevType == QueryValidator::assignType::isOperator || prevType == QueryValidator::assignType::isOpenBracket) {
            return false;
        }
        assignment.erase(0, opPos + 1);
    } else {
        string sub = assignment.substr(0, opPos);
        if (!regex_match(sub, FACTOR)) {
            return false;
        }
        if (prevType == QueryValidator::assignType::isCloseBracket) {
            return false;
        }
        assignment.erase(0, opPos + 1);
    }
    return true;
}

bool isValidFactor(int bracePos, string &assignment, QueryValidator::assignType prevType) {
    string sub = assignment.substr(0, bracePos);
    if (!regex_match(sub, FACTOR)) {
        return false;
    }
    assignment.erase(0, bracePos);
    if (prevType == QueryValidator::assignType::isCloseBracket) {
        return false;
    }
    return true;
}
/**
 * Check with assignment statement without whitespaces is valid
 * @param assignment
 * @return
 */
bool QueryValidator::isValidAssignment(string assignment) {
    if (assignment.compare("_") == 0) {
        return true;
    }
    stack<int> parentheses;
    int bracePos = assignment.find_first_of("()");
    int opPos = assignment.find_first_of("+-*/%");
    QueryValidator::assignType prevType = assignType::isNone;
    while (!assignment.empty()) {
        if (bracePos == string::npos && opPos == string::npos) {
            if (regex_match(assignment, FACTOR)) {
                return true;
            }
            return false;
        }
        if ((bracePos < opPos && bracePos != string::npos) || opPos == string::npos) {
            // (x) or (x+5) or )*(x-y) or 5)
            if (bracePos != 0) {
                if (!isValidFactor(bracePos, assignment, prevType)) {
                    return false;
                }
                prevType = assignType::isFactor;
            } else {
                string brace = assignment.substr(0, bracePos + 1);
                if (brace.find("(") == 0) {
                    parentheses.push(1);
                    if (prevType == assignType::isCloseBracket || prevType == assignType::isFactor) {
                        return false;
                    }
                    prevType = assignType::isOpenBracket;
                } else {
                    if (parentheses.empty()) {
                        return false;
                    }
                    parentheses.pop();
                    if (prevType == assignType::isOpenBracket || prevType == assignType::isOperator) {
                        return false;
                    }
                    prevType = assignType::isCloseBracket;
                }
                assignment.erase(0, bracePos + 1);
            }
        }
        if (opPos < bracePos && opPos != string::npos || bracePos == string::npos) {
            // +5) or +5 or x+5*(x-y) or 5+y)
            if (!isValidOpOrFactor(opPos, assignment, prevType)) {
                return false;
            }
            prevType = QueryValidator::assignType::isOperator;
        }
        bracePos = assignment.find_first_of("()");
        opPos = assignment.find_first_of("+-*/%");
    }
    if (!parentheses.empty()) {
        return false;
    }
    return true;
}

bool QueryValidator::isSufficientAndSyntacticalArguments(string patternSyn, vector<string> arguments) {
    int size = arguments.size();
    Query::entityType entityType = qe.getEntityTypeBySynName(patternSyn);
    if (entityType == Query::entityType::assign || entityType == Query::entityType::whiles) {
        if (size != 2) {
            return false;
        }
        string rightHandSide = arguments.at(1);
        QPPUtil::stripArgument(rightHandSide);
        QPPUtil::stripAllWhiteSpaces(rightHandSide);
        if (!isValidAssignment(rightHandSide)) {
            return false;
        }
    } else if (entityType == Query::entityType::ifl) {
        if (size != 3) {
            return false;
        }
    }
    return true;
}

bool QueryValidator::isValidPatternAssign(vector<string> arguments) {
    if (arguments.size() != 2) {
        return false;
    }
    string leftHandSide = arguments.at(0);
    string rightHandSide = arguments.at(1);
    if (!regex_match(leftHandSide, ENT_REF)) {
        qe.setSyntaxError();
        return false;
    }
    if (!regex_match(rightHandSide, ASSIGN) && rightHandSide.compare(WILDCARD) != 0) {
        qe.setSyntaxError();
        return false;
    }
    if (!areArgumentsValid(leftHandSide, rightHandSide)) {
        qe.setSemanticError();
        return false;
    }
    // if lhs syn is not variable, returns false
    if (regex_match(leftHandSide, SYNONYM)) {
        Query::entityType leftEntity = qe.getEntityTypeBySynName(leftHandSide);
        if (leftEntity != Query::entityType::var) {
            qe.setSemanticError();
            return false;
        }
    }

    return true;
}

bool QueryValidator::isValidPatternIf(vector<string> arguments) {
    if (arguments.size() != 3) {
        return false;
    }
    string first = arguments.at(0);
    string second = arguments.at(1);
    string third = arguments.at(2);
    if (!regex_match(first, ENT_REF)) {
        qe.setSyntaxError();
        return false;
    }
    if (second.compare(WILDCARD) != 0 || third.compare(WILDCARD) != 0) {
        qe.setSyntaxError();
        return false;
    }
    // just to check if first is declared
    if (!areArgumentsValid(first, second)) {
        qe.setSemanticError();
        return false;
    }
    // if lhs syn is not variable, returns false
    if (regex_match(first, SYNONYM)) {
        Query::entityType leftEntity = qe.getEntityTypeBySynName(first);
        if (leftEntity != Query::entityType::var) {
            qe.setSemanticError();
            return false;
        }
    }
    return true;
}

bool QueryValidator::isValidPatternWhile(vector<string> arguments) {
    if (arguments.size() != 2) {
        return false;
    }
    string leftHandSide = arguments.at(0);
    string rightHandSide = arguments.at(1);
    if (!regex_match(leftHandSide, ENT_REF)) {
        qe.setSyntaxError();
        return false;
    }
    if (rightHandSide.compare(WILDCARD) != 0) {
        qe.setSyntaxError();
        return false;
    }
    if (!areArgumentsValid(leftHandSide, rightHandSide)) {
        qe.setSemanticError();
        return false;
    }
    // if lhs syn is not variable, returns false
    if (regex_match(leftHandSide, SYNONYM)) {
        Query::entityType leftEntity = qe.getEntityTypeBySynName(leftHandSide);
        if (leftEntity != Query::entityType::var) {
            qe.setSemanticError();
            return false;
        }
    }
    return true;

}

bool QueryValidator::isValidFolParentNext(vector<string> argument) {
    string leftHandSide = argument.at(0);
    string rightHandSide = argument.at(1);

    if (!regex_match(leftHandSide, STMT_REF) || !regex_match(rightHandSide, STMT_REF)) {
        qe.setSyntaxError();
        return false;
    }
    if (!areArgumentsValid(leftHandSide, rightHandSide)) {
        qe.setSemanticError();
        return false;
    }

    // if lhs or rhs are var, const or procedure, returns false
    if (regex_match(leftHandSide, SYNONYM)) {
        Query::entityType leftEntity = qe.getEntityTypeBySynName(leftHandSide);
        if (leftEntity == Query::entityType::var || leftEntity == Query::entityType::constant || leftEntity == Query::entityType::procedure) {
            qe.setSemanticError();
            return false;
        }
    }
    if (regex_match(rightHandSide, SYNONYM)) {
        Query::entityType rightEntity = qe.getEntityTypeBySynName(rightHandSide);
        if (rightEntity == Query::entityType::var || rightEntity == Query::entityType::constant || rightEntity == Query::entityType::procedure) {
            qe.setSemanticError();
            return false;
        }
    }
    return true;
}

bool QueryValidator::isValidModifiesUses(QueryClause::relationshipType relationshipType, vector<string> argument) {
    string leftHandSide = argument.at(0);
    string rightHandSide = argument.at(1);
    if (!regex_match(leftHandSide, STMT_REF) && !regex_match(leftHandSide, ENT_REF)) {
        qe.setSyntaxError();
        return false;
    }
    if (relationshipType == QueryClause::relationshipType::Modifies && leftHandSide.compare(WILDCARD) == 0) {
        qe.setSemanticError();
        return false;
    }
    if (relationshipType == QueryClause::relationshipType::Uses && leftHandSide.compare(WILDCARD) == 0) {
        qe.setSemanticError();
        return false;
    }
    if (!regex_match(rightHandSide, ENT_REF)) {
        qe.setSyntaxError();
        return false;
    }
    if (!areArgumentsValid(leftHandSide, rightHandSide)) {
        qe.setSemanticError();
        return false;
    }
    if (regex_match(leftHandSide, SYNONYM)) {
        Query::entityType leftEntity = qe.getEntityTypeBySynName(leftHandSide);
        if (leftEntity == Query::entityType::var || leftEntity == Query::entityType::constant) {
            qe.setSemanticError();
            return false;
        }
    }
    if (regex_match(rightHandSide,SYNONYM)) {
        Query::entityType rightEntity = qe.getEntityTypeBySynName(rightHandSide);
        if (rightEntity != Query::entityType::var) {
            qe.setSemanticError();
            return false;
        }
    }
    return true;
}

bool QueryValidator::isValidAffects(vector<string> argument) {
    string leftHandSide = argument.at(0);
    string rightHandSide = argument.at(1);

    if (!regex_match(leftHandSide, STMT_REF) || !regex_match(rightHandSide, STMT_REF)) {
        qe.setSyntaxError();
        return false;
    }
    if (!areArgumentsValid(leftHandSide, rightHandSide)) {
        qe.setSemanticError();
        return false;
    }

    // if lhs or rhs are var, const or procedure, returns false
    if (regex_match(leftHandSide, SYNONYM)) {
        Query::entityType leftEntity = qe.getEntityTypeBySynName(leftHandSide);
        if (leftEntity != Query::entityType::assign && leftEntity != Query::entityType::stmt && leftEntity != Query::entityType::prog_line) {
            qe.setSemanticError();
            return false;
        }
    }
    if (regex_match(rightHandSide, SYNONYM)) {
        Query::entityType rightEntity = qe.getEntityTypeBySynName(rightHandSide);
        if (rightEntity != Query::entityType::assign && rightEntity != Query::entityType::stmt && rightEntity != Query::entityType::prog_line) {
            qe.setSemanticError();
            return false;
        }
    }
    return true;
}

bool QueryValidator::isValidCalls(vector<string> argument) {
    string leftHandSide = argument.at(0);
    string rightHandSide = argument.at(1);

    if (!regex_match(leftHandSide, ENT_REF) || !regex_match(rightHandSide, ENT_REF)) {
        qe.setSyntaxError();
        return false;
    }
    if (!areArgumentsValid(leftHandSide, rightHandSide)) {
        qe.setSemanticError();
        return false;
    }
    if (regex_match(leftHandSide, SYNONYM)) {
        Query::entityType leftEntity = qe.getEntityTypeBySynName(leftHandSide);
        if (leftEntity != Query::entityType::procedure) {
            qe.setSemanticError();
            return false;
        }
    }
    if (regex_match(rightHandSide,SYNONYM)) {
        Query::entityType rightEntity = qe.getEntityTypeBySynName(rightHandSide);
        if (rightEntity != Query::entityType::procedure) {
            qe.setSemanticError();
            return false;
        }
    }
    return true;
}

bool QueryValidator::isValidSuchThat(QueryClause::relationshipType relationshipType, vector<string> argument) {
    if (relationshipType == QueryClause::relationshipType::WRONG_TYPE) {
        qe.setSyntaxError();
        return false;
    }
    if (argument.size() != 2) {
        qe.setSyntaxError();
        return false;
    }
    string leftHandSide = argument.at(0);
    string rightHandSide = argument.at(1);

    if (relationshipType == QueryClause::relationshipType::Follows ||
        relationshipType == QueryClause::relationshipType::FollowsT || relationshipType == QueryClause::relationshipType::Parent ||
        relationshipType == QueryClause::relationshipType::ParentT || relationshipType == QueryClause::relationshipType::Next ||
        relationshipType == QueryClause::relationshipType::NextT || relationshipType == QueryClause::relationshipType::NextBip ||
        relationshipType == QueryClause::relationshipType::NextBipT) {
        return isValidFolParentNext(argument);
    }

    if (relationshipType == QueryClause::relationshipType::Modifies || relationshipType == QueryClause::relationshipType::Uses) {
        return isValidModifiesUses(relationshipType, argument);
    }

    if (relationshipType == QueryClause::relationshipType::Affects || relationshipType == QueryClause::relationshipType::AffectsT ||
        relationshipType == QueryClause::relationshipType::AffectsBip || relationshipType == QueryClause::relationshipType::AffectsBipT) {
        return isValidAffects(argument);
    }
    if (relationshipType == QueryClause::relationshipType::Calls || relationshipType == QueryClause::relationshipType::CallsT) {
        return isValidCalls(argument);
    }
    return true;
}

bool checkWithArgsWithNoAttr(string str) {
    if (regex_match(str, SYNONYM)) {
        if (!qe.isVarDeclared(str)) {
            qe.setSemanticError();
            return false;
        }
        if (qe.getEntityTypeBySynName(str) != Query::entityType::prog_line) {
            qe.setSemanticError();
            return false;
        }
    } else if (!regex_match(str, INTEGER) && !regex_match(str, IDENT_WITH_QUOTES)) {
        qe.setSyntaxError();
        return false;
    }
    return true;
}

bool isAttrAndValueTypeConsistent(QueryClause::attribute attribute, Query::entityType entityType) {
    if (attribute == QueryClause::attribute::value) {
        if (entityType != Query::entityType::constant) {
            return false;
        }
    } else if (attribute == QueryClause::attribute::varName) {
        if (entityType != Query::entityType::var && entityType != Query::entityType::read
            && entityType != Query::entityType::print) {
            return false;
        }
    } else if (attribute == QueryClause::attribute::procName) {
        if (entityType != Query::entityType::procedure && entityType != Query::entityType::call) {
            return false;
        }
    } else if (attribute == QueryClause::attribute::stmtNum) {
        if (entityType == Query::entityType::var || entityType == Query::entityType::constant ||
            entityType == Query::entityType::procedure || entityType == Query::entityType::prog_line) {
            return false;
        }
    }
    return true;
}

bool isValidLeftWithSyn(string rhs, QueryClause::attribute attr2) {
    if (regex_match(rhs, IDENT_WITH_QUOTES)) {
        qe.setSemanticError();
        return false;
    }
    if (attr2 != QueryClause::attribute::NO_ATTR) {
        if (attr2 != QueryClause::attribute::value && attr2 != QueryClause::attribute::stmtNum) {
            qe.setSemanticError();
            return false;
        }
    } else {
        if (!regex_match(rhs, INTEGER)) {
            if (qe.getEntityTypeBySynName(rhs) != Query::entityType::prog_line) {
                qe.setSemanticError();
                return false;
            }
        }
    }
    return true;
}

bool isValidLeftRightEntities(string lhs, pair<Query::entityType, QueryClause::attribute> left, string rhs, pair<Query::entityType, QueryClause::attribute> right) {
    QueryClause::attribute attr1 = left.second;
    Query::entityType rightEntity = right.first;
    QueryClause::attribute attr2 = right.second;

    if (attr1 == QueryClause::attribute::procName || attr1 == QueryClause::attribute::varName) {
        if (attr2 == QueryClause::attribute::NO_ATTR) {
            if (!regex_match(rhs, IDENT_WITH_QUOTES)) {
                qe.setSemanticError();
                return false;
            }
        } else {
            if (attr2 != QueryClause::attribute::procName && attr2 != QueryClause::attribute::varName) {
                qe.setSemanticError();
                return false;
            }
        }
    } else if (attr1 == QueryClause::attribute::value || attr1 == QueryClause::attribute::stmtNum) {
        if (attr2 == QueryClause::attribute::NO_ATTR) {
            if (rightEntity == Query::entityType::var || rightEntity == Query::entityType::procedure) {
                qe.setSemanticError();
                return false;
            }
        } else {
            if ((attr2 != QueryClause::attribute::value && attr2 != QueryClause::attribute::stmtNum)) {
                qe.setSemanticError();
                return false;
            }
        }
    } else if (regex_match(lhs, IDENT_WITH_QUOTES)) {
        if (!regex_match(rhs, IDENT_WITH_QUOTES) && attr2 != QueryClause::attribute::varName && attr2 != QueryClause::attribute::procName) {
            qe.setSemanticError();
            return false;
        }
    } else if (regex_match(lhs, INTEGER)) {
        if (!regex_match(rhs, INTEGER) && attr2 != QueryClause::attribute::stmtNum && attr2 != QueryClause::attribute::value && rightEntity != Query::entityType::prog_line) {
            qe.setSemanticError();
            return false;
        }
    } else if (regex_match(lhs, SYNONYM)) {
        return isValidLeftWithSyn(rhs, attr2);
    }
    return true;
}

bool QueryValidator::isValidWith(string lhs, QueryClause::attribute attr1, string rhs,
                                 QueryClause::attribute attr2) {
    if (attr1 == QueryClause::attribute::WRONG_ATTR || attr2 == QueryClause::attribute::WRONG_ATTR) {
        qe.setSyntaxError();
        return false;
    }
    Query::entityType leftEntity = Query::entityType::no_type;
    Query::entityType rightEntity = Query::entityType::no_type;
    if (attr1 == QueryClause::attribute::NO_ATTR) {
        if (!checkWithArgsWithNoAttr(lhs)) {
            return false;
        }
        leftEntity = qe.getEntityTypeBySynName(lhs);
    } else {
        if (!regex_match(lhs, SYNONYM)) {
            qe.setSyntaxError();
            return false;
        }
        if (!qe.isVarDeclared(lhs)) {
            qe.setSemanticError();
            return false;
        }
        leftEntity = qe.getEntityTypeBySynName(lhs);
        if (!isAttrAndValueTypeConsistent(attr1, leftEntity)) {
            qe.setSemanticError();
            return false;
        }
    }
    if (attr2 == QueryClause::attribute::NO_ATTR) {
        if (!checkWithArgsWithNoAttr(rhs)) {
            return false;
        }
        rightEntity = qe.getEntityTypeBySynName(rhs);
    } else {
        if (!regex_match(rhs, SYNONYM)) {
            qe.setSyntaxError();
            return false;
        }
        if (!qe.isVarDeclared(rhs)) {
            qe.setSemanticError();
            return false;
        }
        rightEntity = qe.getEntityTypeBySynName(rhs);
        if (!isAttrAndValueTypeConsistent(attr2, rightEntity)) {
            qe.setSemanticError();
            return false;
        }
    }
    return isValidLeftRightEntities(lhs, make_pair(leftEntity, attr1), rhs, make_pair(rightEntity, attr2));
}

bool QueryValidator::areArgumentsValid(string leftHand, string rightHand) {
    if (regex_match(leftHand, SYNONYM)) {
        if (!qe.isVarDeclared(leftHand)) {
            qe.setSemanticError();
            return false;
        }
    }
    // right hand: synonym/ _/ integer or "sth" or expression spec
    if (regex_match(rightHand, SYNONYM)) {
        if (!qe.isVarDeclared(rightHand)) {
            qe.setSemanticError();
            return false;
        }
    }

    return true;
}

bool QueryValidator::isBracketValid(string bracket) {
    if (bracket.find("(") != 0 || bracket.find(")") == string::npos) {
        qe.setSyntaxError();
        return false;
    }
    return true;
}

bool QueryValidator::isContentExist(string content) {
    if (!content.find(",")) {
        return false;
    }
    size_t pos = content.find(",");
    string lhs = content.substr(0, pos);
    lhs.erase(lhs.find_last_not_of(" ") + 1);
    string rhs = content.substr(pos + 1);
    rhs.erase(0, rhs.find_first_not_of(" "));

    if (lhs.empty() || rhs.empty()) {
        qe.setSyntaxError();
        return false;
    }
    return true;
}

