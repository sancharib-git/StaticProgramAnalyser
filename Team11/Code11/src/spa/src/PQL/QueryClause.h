#ifndef AUTOTESTER_QUERYCLAUSE_H
#define AUTOTESTER_QUERYCLAUSE_H
#include <string>
#include "SetProcessor.h"
#include "QueryElement.h"
#include "PKB/PKB.h"

using namespace std;
class QueryClause {
public:
    //comparator
    struct compareQueryClause {
        bool operator() (QueryClause clause1, QueryClause clause2) {
            return clause1.getReversePriority(false, false) < clause2.getReversePriority(false, false);
        };
    };

    struct compareQueryClauseManyVars {
        bool operator() (QueryClause clause1, QueryClause clause2) {
            return clause1.getReversePriority(true, false) < clause2.getReversePriority(true, false);
        };
    };

    struct compareQueryClauseManyAffectsStmt {
        bool operator() (QueryClause clause1, QueryClause clause2) {
            return clause1.getReversePriority(false, true) < clause2.getReversePriority(false, true);
        };
    };

    struct compareQueryClauseManyVarsAndAffects {
        bool operator() (QueryClause clause1, QueryClause clause2) {
            return clause1.getReversePriority(true, true) < clause2.getReversePriority(true, true);
        };
    };

    // design abstractions
    enum class clauseType {pattern, suchThat, with, none};

    enum class relationshipType { Follows, FollowsT, Parent, ParentT, Uses, UsesProc, Modifies, ModifiesProc, Next, NextT, Affects, AffectsT, Calls, CallsT, NextBip, NextBipT, AffectsBip, AffectsBipT, WRONG_TYPE };
    enum class patternFlag { isContains, isAll, isExactMatch, isDeclaration, isNone }; //isNone means not wild
    enum class suchThatFlag { isNumber, isString, isAll, isDeclaration, isNone }; //isNone means not wild
    enum class withFlag { isProcName, isVarName, isValue, isStmtNum, isString, isNumber, isDeclaration, isNone };
    enum class attribute { procName, varName, value, stmtNum, NO_ATTR, WRONG_ATTR };

    static relationshipType getRelationshipType(string relationship);
    static attribute getAttributeType(string attr);
    static string getPatternFlagAsString(patternFlag flag);
    static patternFlag getStringAsPatternFlag(string flag);
    static string getRelationshipTypeAsString(relationshipType r);
    static bool isSameClause(QueryClause qc1, QueryClause qc2);

    int getReversePriority(bool moreVarThanStmt, bool manyAffectsStmt);
    int getReversePriorityWith();
    int getReversePrioritySuchThat(bool moreVarThanStmt, bool manyAffectsStmt);
    int getReversePriorityPattern();
    clauseType type;

    static struct pattern {
        string synonym;
        string lhs;
        string rhs;
        patternFlag leftFlag;
        patternFlag rightFlag;
        static pattern EMPTY() {
            return pattern ("", "", "", patternFlag::isNone, patternFlag::isNone);
        }

        pattern(string syn, string l, string r, patternFlag lf, patternFlag rf) {
            synonym = syn;
            lhs = l;
            rhs = r;
            leftFlag = lf;
            rightFlag = rf;
        }

        string to_string() {
            return synonym + ", "
                   + lhs + ", "
                   + getPatternFlagAsString(leftFlag) + ", "
                   + rhs + ", "
                   + getPatternFlagAsString(rightFlag);
        }

        int countDeclarations() {
            int count = 1;
            if (leftFlag == patternFlag::isDeclaration) {
                count++;
            }
            return count;
        }

    };
    static struct suchThat {
        relationshipType relation;
        string arg1;
        string arg2;
        suchThatFlag flag1;
        suchThatFlag flag2;
        static suchThat EMPTY() {
            return suchThat(relationshipType::WRONG_TYPE, "", suchThatFlag::isNone, "", suchThatFlag::isNone);
        }

        suchThat(relationshipType relation, string arg1, suchThatFlag flag1, string arg2, suchThatFlag flag2) {
            this->relation = relation;
            this->arg1 = arg1;
            this->arg2 = arg2;
            this->flag1 = flag1;
            this->flag2 = flag2;
        }

        string getSuchThatFlagAsString(suchThatFlag flag) {
            switch (flag) {
                case suchThatFlag::isAll:
                    return "isAll";
                case suchThatFlag::isString:
                    return "isString";
                case suchThatFlag::isNumber:
                    return "isNumber";
                case suchThatFlag::isDeclaration:
                    return "isDeclaration";
                default:
                    return "NIL";
            }
        }

        string to_string() {
            return getRelationshipTypeAsString(relation) + ", "
                   + arg1 + ", "
                   + getSuchThatFlagAsString(flag1) + ", "
                   + arg2 + ", "
                   + getSuchThatFlagAsString(flag2);
        }

        int countDeclarations() {
            int count = 0;
            if (flag1 == suchThatFlag::isDeclaration) {
                count++;
            }
            if (flag2 == suchThatFlag::isDeclaration) {
                count++;
            }
            return count;
        }

        int countIsAll() {
            int count = 0;
            if (flag1 == suchThatFlag::isAll) {
                count++;
            }
            if (flag2 == suchThatFlag::isAll) {
                count++;
            }
            return count;
        }
    };
    static struct with {
        string lhs;
        withFlag flagLeft;
        string rhs;
        withFlag flagRight;
        static with EMPTY() {
            return with("", withFlag::isNone, "", withFlag::isNone);
        }

        with(string lhs, withFlag flagLeft, string rhs, withFlag flagRight) {
            this->lhs = lhs;
            this->flagLeft = flagLeft;
            this->rhs = rhs;
            this-> flagRight = flagRight;
        }

        string getWithFlagAsString(withFlag attr) {
            switch (attr) {
                case withFlag::isDeclaration:
                    return "declaration";
                case withFlag::isString:
                    return "string";
                case withFlag::isNumber:
                    return "number";
                case withFlag::isProcName:
                    return "procName";
                case withFlag::isStmtNum:
                    return "stmt#";
                case withFlag::isValue:
                    return "value";
                case withFlag::isVarName:
                    return "varName";
                default:
                    return "NIL";
            }
        }

        string to_string() {
            return lhs + "."
                   + getWithFlagAsString(flagLeft) + " = "
                   + rhs + "."
                   + getWithFlagAsString(flagRight);
        }

        int countDeclarations() {
            int count = 0;
            if (flagLeft == withFlag::isDeclaration) {
                count++;
            }
            if (flagRight == withFlag::isDeclaration) {
                count++;
            }
            return count;
        }
    };

    QueryClause(with with) {
        this->withStruct = with;
        this->type = clauseType::with;
    }

    QueryClause(pattern pattern) {
        this->patternStruct = pattern;
        this->type = clauseType::pattern;
    }

    QueryClause(suchThat suchThat) {
        this->suchThatStruct = suchThat;
        this->type = clauseType::suchThat;
    }

    clauseType getClauseType() {
        return type;
    }

    string getClauseTypeAsString(clauseType type) {
        switch (type) {
            case clauseType::pattern:
                return "pattern";
            case clauseType::with:
                return "with";
            case clauseType::suchThat:
                return "suchThat";
            default:
                return "ERROR";
        }
    }

    string to_string() {
        if (type == clauseType::pattern) {
            return patternStruct.to_string();
        } else if (type == clauseType::suchThat) {
            return suchThatStruct.to_string();
        } else if (type == clauseType::with) {
            return withStruct.to_string();
        } else {
            return "ERROR";
        }
    }

    pattern patternStruct = QueryClause::pattern::EMPTY();
    suchThat suchThatStruct = QueryClause::suchThat::EMPTY();
    with withStruct = QueryClause::with::EMPTY();
};
#endif
