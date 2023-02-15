#include "QueryClause.h"
#include <string>
using namespace std;
QueryClause::relationshipType QueryClause::getRelationshipType(string relationship) {
    if (relationship.compare("Follows") == 0) {
        return relationshipType::Follows;
    } else if (relationship.compare("Follows*") == 0) {
        return relationshipType::FollowsT;
    } else if (relationship.compare("Parent") == 0) {
        return relationshipType::Parent;
    } else if (relationship.compare("Parent*") == 0) {
        return relationshipType::ParentT;
    } else if (relationship.compare("Uses") == 0) {
        return relationshipType::Uses;
    } else if (relationship.compare("Modifies") == 0) {
        return relationshipType::Modifies;
    } else if (relationship.compare("Next") == 0) {
        return relationshipType::Next;
    } else if (relationship.compare("Next*") == 0) {
        return relationshipType::NextT;
    } else if (relationship.compare("Affects") == 0) {
        return relationshipType::Affects;
    } else if (relationship.compare("Affects*") == 0) {
        return relationshipType::AffectsT;
    } else if (relationship.compare("Calls") == 0) {
        return relationshipType::Calls;
    } else if (relationship.compare("Calls*") == 0) {
        return relationshipType::CallsT;
    } else if (relationship.compare("NextBip") == 0) {
        return relationshipType::NextBip;
    } else if (relationship.compare("NextBip*") == 0) {
        return relationshipType::NextBipT;
    } else if (relationship.compare("AffectsBip") == 0) {
        return relationshipType::AffectsBip;
    } else if (relationship.compare("AffectsBip*") == 0) {
        return relationshipType::AffectsBipT;
    } else {
        return relationshipType::WRONG_TYPE;
    }
}

string QueryClause::getPatternFlagAsString(patternFlag flag) {
    switch (flag) {
        case patternFlag::isAll:
            return "isAll";
        case patternFlag::isContains:
            return "isContains";
        case patternFlag::isExactMatch:
            return "isExactMatch";
        case patternFlag::isDeclaration:
            return "isDeclaration";
        default:
            return "NIL";
    }
}

QueryClause::patternFlag QueryClause::getStringAsPatternFlag(string flag) {
    if (flag.compare("_") == 0) {
        return QueryClause::patternFlag::isAll;
    } else if (flag.find("\"") == 0) {
        return QueryClause::patternFlag::isExactMatch;
    } else if (flag.find("_\"") == 0) {
        return QueryClause::patternFlag::isContains;
    } else {
        return QueryClause::patternFlag::isDeclaration;
    }
}
string QueryClause::getRelationshipTypeAsString(QueryClause::relationshipType r) {
    if (r==relationshipType::Follows) {
        return "Follows";
    } else if (r==relationshipType::FollowsT) {
        return "FollowsT";
    }
//    else if (relationship.compare("Parent") == 0) {
//        return relationshipType::Parent;
//    } else if (relationship.compare("Parent*") == 0) {
//        return relationshipType::ParentT;
//    } else if (relationship.compare("Uses") == 0) {
//        return relationshipType::Uses;
//    } else if (relationship.compare("Modifies") == 0) {
//        return relationshipType::Modifies;
//    } else if (relationship.compare("Next") == 0) {
//        return relationshipType::Next;
//    } else if (relationship.compare("Next*") == 0) {
//        return relationshipType::NextT;
//    } else if (relationship.compare("Affects") == 0) {
//        return relationshipType::Affects;
//    } else if (relationship.compare("Affects*") == 0) {
//        return relationshipType::AffectsT;
//    } else if (relationship.compare("Calls") == 0) {
//        return relationshipType::Calls;
//    } else if (relationship.compare("Calls*") == 0) {
//        return relationshipType::CallsT;
//    }
    else {
        return "ERROR";
    }
}

QueryClause::attribute QueryClause::getAttributeType(string attr) {
    if (attr.compare("stmt#") == 0) {
        return attribute::stmtNum;
    } else if (attr.compare("value") == 0) {
        return attribute::value;
    } else if (attr.compare("varName") == 0) {
        return attribute::varName;
    } else if (attr.compare("procName") == 0) {
        return attribute::procName;
    } else {
        return attribute::WRONG_ATTR;
    }
}

bool QueryClause::isSameClause(QueryClause qc1, QueryClause qc2) {
    if (qc1.type == qc2.type) {
        if (qc1.type == clauseType::with) {
            with w1 = qc1.withStruct;
            with w2 = qc2.withStruct;
            return (w1.lhs == w2.lhs) && (w1.flagLeft == w2.flagLeft) && (w1.rhs == w2.rhs) && (w1.flagRight == w2.flagRight);
        } else if (qc1.type == clauseType::pattern) {
            pattern p1 = qc1.patternStruct;
            pattern p2 = qc2.patternStruct;
            return (p1.synonym == p2.synonym) && (p1.lhs == p2.lhs) && (p1.leftFlag == p2.leftFlag) && (p1.rhs == p2.rhs) && (p1.rightFlag == p2.rightFlag);
        } else if (qc1.type == clauseType::suchThat && qc1.suchThatStruct.relation == qc2.suchThatStruct.relation) {
            suchThat st1 = qc1.suchThatStruct;
            suchThat st2 = qc2.suchThatStruct;
            if (st1.flag1 == st2.flag1 && st1.flag2 == st2.flag2) {
                bool match = true;
                if (st1.flag1 != suchThatFlag::isAll) {
                    match &= st1.arg1 == st2.arg1;
                }
                if (st1.flag2 != suchThatFlag::isAll) {
                    match &= st1.arg2 == st2.arg2;
                }
                return match;
            }
            return false;
        }
    }
    return false;
}


int QueryClause::getReversePriority(bool moreVarThanStmt, bool manyAffectsStmt) {
    //the lower the magnitude the higher the priority
    int priority = 0;
    if (type == clauseType::with) {
        return getReversePriorityWith();
    } else if (type == clauseType::pattern) {
       return getReversePriorityPattern();
    } else if (type == clauseType::suchThat) {
        return getReversePrioritySuchThat(moreVarThanStmt, manyAffectsStmt);
    }
    return priority;
}

int QueryClause::getReversePriorityWith() {
    int priority = 0;
    priority += withStruct.countDeclarations() * 10; //0, 10, 20
    priority = priority != 0 ? priority + 10 : priority; //0, 20, 30
    return priority;
}

int QueryClause::getReversePrioritySuchThat(bool moreVarThanStmt, bool manyAffectsStmt) {
    relationshipType r = suchThatStruct.relation;
    int priority = 0;
    int weight;
    int decCount = suchThatStruct.countDeclarations();
    int wildCardCount = suchThatStruct.countIsAll();
    weight = decCount == 0 ? decCount : decCount + 1; //syn, syn: 3, syn, all: 2, syn, num: 2, all/num, all/num: 0
    weight = decCount == 1 && wildCardCount == 0 ? weight / 2 : weight; //syn, syn: 3, syn, all: 2, syn, num: 1, all/num, all/num: 0
    if (r == QueryClause::relationshipType::Uses) {
        priority = !moreVarThanStmt ? 11 : 10;
    } else if (r == QueryClause::relationshipType::UsesProc) {
        priority = 7;
    } else if (r == QueryClause::relationshipType::Modifies) {
        priority = 4;
    } else if (r == QueryClause::relationshipType::ModifiesProc) {
        priority = 6;
    } else if (r == QueryClause::relationshipType::Follows) {
        priority = 8;
    } else if (r == QueryClause::relationshipType::FollowsT) {
        priority = !moreVarThanStmt ? 10 : 11;
    } else if (r == QueryClause::relationshipType::Parent) {
        priority = 3;
    } else if (r == QueryClause::relationshipType::ParentT) {
        priority = 5;
    } else if (r == QueryClause::relationshipType::Calls) {
        priority = 1;
    } else if (r == QueryClause::relationshipType::CallsT) {
        priority = 2;
    } else if (r == QueryClause::relationshipType::Next) {
        priority = 9;
    } else if (r == QueryClause::relationshipType::NextT) {
        priority = !manyAffectsStmt ? 14 : 13; // modify weight?
        weight = weight == 0 ? 5 : 30 / weight; //syn, syn: 10, syn, all: 15, syn, num: 30, all/num, all/num: 5
    } else if (r == QueryClause::relationshipType::Affects) {
        priority = 12;
        //since there can only be assign type, syn/all should be calculated before syn/num
        weight = weight == 0 ? 5 : 30 / weight; //syn, syn: 10, syn, all: 15, syn, num: 30, all/num, all/num: 5
    } else if (r == QueryClause::relationshipType::AffectsT) {
        priority = !manyAffectsStmt ? 13 : 14;
        //since there can only be assign type, syn/all should be calculated before syn/num
        weight = weight == 0 ? 5 : 30 / weight;
    }
    return weight * 100 + priority;
}

int QueryClause::getReversePriorityPattern() {
    int priority = 100;
    int reverseWeight;
    reverseWeight = patternStruct.leftFlag == patternFlag::isAll ? 1 : 0;
    reverseWeight *= 10;
    if (patternStruct.rightFlag == patternFlag::isAll) {
        reverseWeight += 1;
    } else if (patternStruct.rightFlag == patternFlag::isContains) {
        reverseWeight += 2;
    } else if (patternStruct.rightFlag == patternFlag::isExactMatch) {
        reverseWeight += 3;
    }
    return priority - reverseWeight;
}
/*order: clause, reversePriority
 * with num/string = num/string, (0)
 * suchThat(num/string/all, num/string/all), (1-10)
 * with num/string = syn, (20)
 * with num/string = num/string, (30)
 * pattern a(_, exact), (88)
 * pattern a(_, partial), (89)
 * pattern a(_, _), (90)
 * pattern a(v, exact), (98)
 * pattern a(v, partial), (99)
 * pattern a(v, _) (100)
 * suchThat(syn, num/string/all) (201-210)
 * suchThat(syn, syn) (301 - 310)
 * affects/affectsT/nextT(all/num, all/num) (>500)
 * affects/affectsT/nextT(syn, syn) (>1000)
 * affects/affectsT/nextT(all, syn) (>1500)
 * affects/affectsT/nextT(num, syn) (>3000)
*/
