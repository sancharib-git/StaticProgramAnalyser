#include "SuchThatEvaluator.h"
#include "CustomFunctions.h"
//helper
QueryClauseResult* convertToReturnItemShortCutSameArg(unordered_set<QueryElement> set, QueryClauseResult* res, std::function<bool (QueryElement,QueryElement)> func) {
    for (auto a : set) {
        if (func(a,a)) {
            res->add(a,a);
        }
    }
    res->setCanSkipIntersect();
    return res;
}
//helper
QueryClauseResult* convertToReturnItemShortCut(CrossProductStruct<QueryElement>* cps, QueryClauseResult* res, std::function<bool (QueryElement, QueryElement)> func) {
    for (auto a : cps->body) {
        if (func(*(a.begin()),*(a.begin()+1))) {
            res->add(*(a.begin()),*(a.begin()+1));
        }
    }
    res->setCanSkipIntersect();
    return res;
}

//helper
template <typename S, typename T>
QueryClauseResult* getDoubleSynonymResult(QueryClause::suchThat st, QueryClauseResult* res, vector<pair<S, T>>* PKBlist, bool pairExists) {
    IntermediateResult* ir = IntermediateResult::getPtr();
    for (auto a : *PKBlist) {
        if (pairExists && ir->existsInDoubleColumn(st.arg1, st.arg2, QueryElement(a.first), QueryElement(a.second)) ||
            !pairExists && ir->existsInSingleColumn(st.arg1, QueryElement(a.first)) && ir->existsInSingleColumn(st.arg2, QueryElement(a.second))) {
            res->add(QueryElement(a.first), QueryElement(a.second));
        }
    }
    return res;
}
//helper
template <typename S, typename T>
QueryClauseResult* getRepeatedSynonymResult(QueryClause::suchThat st, QueryClauseResult* res, vector<pair<S, T>>* PKBlist) {
    IntermediateResult* ir = IntermediateResult::getPtr();
    for (auto a : *PKBlist) {
        if (QueryElement(a.first) == QueryElement(a.second)
            && ir->existsInSingleColumn(st.arg1, QueryElement(a.first))) {
            res->add(QueryElement(a.first));
        }
    }
    return res;
}
//helper
template <typename S, typename T>
QueryClauseResult* getSingleSynonymResult(QueryClause::suchThat st, QueryClauseResult* res, vector<pair<S, T>>* PKBlist, bool isFirstDec) {
    IntermediateResult* ir = IntermediateResult::getPtr();
    bool isFirstString = st.flag1 == QueryClause::suchThatFlag::isString;
    bool isSecondString = st.flag2 == QueryClause::suchThatFlag::isString;
    bool isFirstNum = st.flag1 == QueryClause::suchThatFlag::isNumber;
    bool isSecondNum = st.flag2 == QueryClause::suchThatFlag::isNumber;
    bool isFirstAll = st.flag1 == QueryClause::suchThatFlag::isAll;
    bool isSecondAll = st.flag2 == QueryClause::suchThatFlag::isAll;
    for (auto a : *PKBlist) {
        if (isFirstDec) {
            bool isSecondMatch = isSecondString ? QueryElement(a.second) == QueryElement(st.arg2)
                                                : isSecondNum && QueryElement(a.second) == QueryElement(stoi(st.arg2));
            if (ir->existsInSingleColumn(st.arg1, QueryElement(a.first)) && (isSecondAll || isSecondMatch)) {
                res->add(QueryElement(a.first));
            }
        } else {
            bool isFirstMatch = isFirstString ? QueryElement(a.first) == QueryElement(st.arg1)
                                              : isFirstNum && QueryElement(a.first) == QueryElement(stoi(st.arg1));
            if ((isFirstAll || isFirstMatch) && ir->existsInSingleColumn(st.arg2, QueryElement(a.second))) {
                res->add(QueryElement(a.second));
            }
        }
    }
    return res;
}
//helper
template <typename S, typename T>
QueryClauseResult* convertToReturnItem(QueryClause::suchThat st, vector<pair<S, T>>* PKBlist, bool canBeNonExact, std::function<bool (QueryElement,QueryElement)> func) {
    IntermediateResult* ir = IntermediateResult::getPtr();
    vector<string> header = {};
    bool sameArgs = st.flag1 == st.flag2 && st.arg1 == st.arg2;
    bool isFirstDec = st.flag1 == QueryClause::suchThatFlag::isDeclaration;
    bool isSecondDec = st.flag2 == QueryClause::suchThatFlag::isDeclaration;
    bool isFirstString = st.flag1 == QueryClause::suchThatFlag::isString;
    bool isSecondString = st.flag2 == QueryClause::suchThatFlag::isString;
    bool isFirstNum = st.flag1 == QueryClause::suchThatFlag::isNumber;
    bool isSecondNum = st.flag2 == QueryClause::suchThatFlag::isNumber;
    bool isFirstAll = st.flag1 == QueryClause::suchThatFlag::isAll;
    bool isSecondAll = st.flag2 == QueryClause::suchThatFlag::isAll;
    if (isFirstDec) {
        header.push_back(st.arg1);
    }
    if (isSecondDec && !sameArgs) {
        header.push_back(st.arg2);
    }
    QueryClauseResult* res = new QueryClauseResult(header);
    res->setCanSkipIntersect();
    if (!isFirstDec && !isSecondDec || canBeNonExact) {
        return new QueryClauseResult(!PKBlist->empty());
    }
    bool pairExists = isFirstDec && isSecondDec && ir->hasDoubleColumn(st.arg1, st.arg2);
    // TODO: remove later
    int PKBListSize = PKBlist->size();
    bool cond1 = (pairExists && !sameArgs
                  && (ir->doubleColumnSize(st.arg1, st.arg2) + SuchThatEvaluator::THRESHOLD_DIFFERENCE < PKBListSize
                      || 2 * ir->doubleColumnSize(st.arg1, st.arg2) < PKBListSize));
    bool cond2 = (pairExists && sameArgs && ir->hasSingleColumn(st.arg1) && ir->singleColumnSize(st.arg1) + SuchThatEvaluator::THRESHOLD_DIFFERENCE_2 < PKBListSize);
    if (cond1) {
        return convertToReturnItemShortCut(ir->getDoubleColumnCPSPtr(st.arg1, st.arg2), res, func);
    } else if (cond2) {
        return convertToReturnItemShortCutSameArg(ir->getSingleColumn(st.arg1), res, func);
    }
    // TODO: remove^
    if (isFirstDec && isSecondDec && !sameArgs) {
        return getDoubleSynonymResult<S,T>(st, res, PKBlist, pairExists);
    } else if (isFirstDec && isSecondDec && sameArgs) {
        return getRepeatedSynonymResult<S,T>(st, res, PKBlist);
    } else if (isFirstDec) {
       return getSingleSynonymResult<S,T>(st, res, PKBlist, true);
    } else if (isSecondDec) {
        return getSingleSynonymResult<S,T>(st, res, PKBlist, false);
    }
    return res;
}

bool validateAffectsNN(string arg1, string arg2) {
    StatementTypes arg1Type = PKB::getInstance().getStatementType(stoi(arg1));
    StatementTypes arg2Type = PKB::getInstance().getStatementType(stoi(arg2));
    if (arg1Type != StatementTypes::ASSIGNMENT && arg1Type != StatementTypes::NO_TYPE) {
        return false;
    }
    if (arg2Type != StatementTypes::ASSIGNMENT && arg2Type != StatementTypes::NO_TYPE) {
        return false;
    }
    return true;
}

bool validateAffectsNT(string arg1, string arg2) {
    StatementTypes arg1Type = PKB::getInstance().getStatementType(stoi(arg1));
    StatementTypes arg2Type = Query::getInstance().getStatementTypeOfDec(arg2);
    if (arg1Type != StatementTypes::ASSIGNMENT && arg1Type != StatementTypes::NO_TYPE) {
        return QueryClauseResult::getInvalidAsPtr();
    }
    if (arg2Type != StatementTypes::ASSIGNMENT && arg2Type != StatementTypes::NO_TYPE) {
        return QueryClauseResult::getInvalidAsPtr();
    }
    return true;
}

bool validateAffectsTN(string arg1, string arg2) {
    StatementTypes arg1Type = Query::getInstance().getStatementTypeOfDec(arg1);
    StatementTypes arg2Type = PKB::getInstance().getStatementType(stoi(arg2));
    if (arg1Type != StatementTypes::ASSIGNMENT && arg1Type != StatementTypes::NO_TYPE) {
        return QueryClauseResult::getInvalidAsPtr();
    }
    if (arg2Type != StatementTypes::ASSIGNMENT && arg2Type != StatementTypes::NO_TYPE) {
        return QueryClauseResult::getInvalidAsPtr();
    }
    return true;
}

bool validateAffectsTT(string arg1, string arg2) {
    StatementTypes arg1Type = Query::getInstance().getStatementTypeOfDec(arg1);
    StatementTypes arg2Type = Query::getInstance().getStatementTypeOfDec(arg2);
    if (arg1Type != StatementTypes::ASSIGNMENT && arg1Type != StatementTypes::NO_TYPE) {
        return QueryClauseResult::getInvalidAsPtr();
    }
    if (arg2Type != StatementTypes::ASSIGNMENT && arg2Type != StatementTypes::NO_TYPE) {
        return QueryClauseResult::getInvalidAsPtr();
    }
    return true;
}

QueryClauseResult* SuchThatEvaluator::evaluateSuchThat(QueryClause::suchThat st) {
    bool cond1 = st.flag1 == QueryClause::suchThatFlag::isDeclaration;
    bool cond2 = st.flag2 == QueryClause::suchThatFlag::isDeclaration;
    bool cond3 = st.flag1 == QueryClause::suchThatFlag::isString;
    bool cond4 = st.flag2 == QueryClause::suchThatFlag::isString;
    bool cond5 = st.flag1 == QueryClause::suchThatFlag::isNumber;
    bool cond6 = st.flag2 == QueryClause::suchThatFlag::isNumber;
    bool cond7 = st.flag1 == QueryClause::suchThatFlag::isAll;
    bool cond8 = st.flag2 == QueryClause::suchThatFlag::isAll;
    bool cond9 = cond1 == cond2 && !cond7 && !cond8 && st.arg1 == st.arg2;
    bool A = st.relation == QueryClause::relationshipType::Uses;
    bool B = st.relation == QueryClause::relationshipType::Modifies;
    bool C = st.relation == QueryClause::relationshipType::Follows;
    bool D = st.relation == QueryClause::relationshipType::FollowsT;
    bool E = st.relation == QueryClause::relationshipType::Parent;
    bool F = st.relation == QueryClause::relationshipType::ParentT;
    bool G = st.relation == QueryClause::relationshipType::Calls;
    bool H = st.relation == QueryClause::relationshipType::CallsT;
    bool I = st.relation == QueryClause::relationshipType::Next;
    bool J = st.relation == QueryClause::relationshipType::NextT;
    bool K = st.relation == QueryClause::relationshipType::Affects;
    bool L = st.relation == QueryClause::relationshipType::AffectsT;
    bool M = st.relation == QueryClause::relationshipType::NextBip;
    bool N = st.relation == QueryClause::relationshipType::NextBipT;
    bool O = st.relation == QueryClause::relationshipType::AffectsBip;
    bool P = st.relation == QueryClause::relationshipType::AffectsBipT;
    // ST: StatementTypes
    bool validCombinations = (A || B) && (cond1 || cond3 || cond5) && (cond2 || cond4 || cond8)
                             /*Uses/Modifies(declaration/string/number, declaration/string/all), declaration: (ST/proc/stmt/prog, var)*/
                             || (C || D || E || F) && (cond1 || cond5 || cond7) && (cond2 || cond6 || cond8) && !cond9
                             /*Follows/FollowsT/Parent/ParentT(declaration/number/all, declaration/number/all), declaration: (ST/stmt/prog, ST/stmt/prog), arg1 != arg2*/
                             || (G || H) && (cond1 || cond3 || cond7) && (cond2 || cond4 || cond8) && !cond9
                             /*Calls/CallsT(declaration/string/all, declaration/string/all), declaration: (proc, proc), arg1 != arg2*/
                             || (I || J || M || N) && (cond1 || cond5 || cond7) && (cond2 || cond6 || cond8)
                             /*Next/NextT and their Bips (declaration/number/all, declaration/number/all), declaration: (ST/stmt/prog, ST/stmt/prog)*/
                             || (K || L || O || P) && (cond1 || cond5 || cond7) && (cond2 || cond6 || cond8);
    /*Affects/AffectsT and their Bips(declaration/number/all, declaration/number/all), declaration: (ST(assign)/stmt/prog, ST(assign)/stmt/prog)*/

    if (!validCombinations) {
        return QueryClauseResult::getInvalidAsPtr();
    }
    if ((A || B) && ((cond1 && Query::getInstance().getDeclarationType(st.arg1) == Query::entityType::procedure) || cond3)) {
        st.relation = A ? QueryClause::relationshipType::UsesProc : QueryClause::relationshipType::ModifiesProc;
    }
    if ((cond1 || cond7) && (cond2 || cond8)) {
        //(_/dec, _/dec)
        return evaluateSuchThatTT(st);
    } else if ((cond3 || cond5) && (cond2 || cond8)) {
        //(string/number, _/dec)
        return evaluateSuchThatNT(st);
    } else if ((cond1 || cond7) && (cond4 || cond6)) {
        //(_/dec, string/number)
        return evaluateSuchThatTN(st);
    } else if ((cond3 || cond5) && (cond4 || cond6)) {
        //(string/number, string/number)
        return evaluateSuchThatNN(st);
    }
    return QueryClauseResult::getInvalidAsPtr();
}

QueryClauseResult* SuchThatEvaluator::evaluateSuchThatNN(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int,int>>();
    vector<pair<int, string>>* list2 = new vector<pair<int,string>>();
    vector<pair<string, int>>* list3 = new vector<pair<string,int>>();
    vector<pair<string, string>>* list4 = new vector<pair<string,string>>();
    if (st.relation == QueryClause::relationshipType::Uses) {
        *list2 = PKB::getInstance().getUsesSSV(stoi(st.arg1), st.arg2);
        return convertToReturnItem<int, string>(st, list2, true, CustomFunctions::isUses());
    } else if (st.relation == QueryClause::relationshipType::UsesProc) {
        *list4 = PKB::getInstance().getUsesPPV(st.arg1,st.arg2);
        return convertToReturnItem<string, string>(st, list4, true, CustomFunctions::isUsesProc());
    } else if (st.relation == QueryClause::relationshipType::Modifies) {
        *list2 = PKB::getInstance().getModifiesSSV(stoi(st.arg1),st.arg2);
        return convertToReturnItem<int, string>(st, list2, true, CustomFunctions::isModifies());
    } else if (st.relation == QueryClause::relationshipType::ModifiesProc) {
        *list4 = PKB::getInstance().getModifiesPPV(st.arg1, st.arg2);
        return convertToReturnItem<string, string>(st, list4, true, CustomFunctions::isModifiesProc());
    } else if (st.relation == QueryClause::relationshipType::Follows) {
        *list1 = PKB::getInstance().getFollowsNN(stoi(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isFollows());
    } else if (st.relation == QueryClause::relationshipType::FollowsT) {
        *list1 = PKB::getInstance().getFollowsStarNN(stoi(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isFollowsT());
    } else if (st.relation == QueryClause::relationshipType::Parent) {
        *list1 = PKB::getInstance().getParentNN(stoi(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isParent());
    } else if (st.relation == QueryClause::relationshipType::ParentT) {
        *list1 = PKB::getInstance().getParentStarNN(stoi(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isParentT());
    } else if (st.relation == QueryClause::relationshipType::Calls) {
        *list4 = PKB::getInstance().getCallsPPP(st.arg1, st.arg2);
        return convertToReturnItem<string, string>(st, list4, true, CustomFunctions::isCalls());
    } else if (st.relation == QueryClause::relationshipType::CallsT) {
        *list4 = PKB::getInstance().getCallsStarNN(st.arg1, st.arg2);
        return convertToReturnItem<string, string>(st, list4, true, CustomFunctions::isCallsT());
    } else if (st.relation == QueryClause::relationshipType::Next) {
        *list1 = PKB::getInstance().getNextNN(stoi(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isNext());
    } else if (st.relation == QueryClause::relationshipType::NextT) {
        *list1 = PKB::getInstance().getNextStarNN(stoi(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isNextT());
    } else if (st.relation == QueryClause::relationshipType::Affects) {// TODO: check if both are assign types, to be implemented
        if(!validateAffectsNN(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        *list1 = PKB::getInstance().getAffectsNN(stoi(st.arg1), stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isAffects());
    } else if (st.relation == QueryClause::relationshipType::AffectsT) {
        // TODO: check if both are assign types, to be implemented
        if(!validateAffectsNN(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        *list1 = PKB::getInstance().getAffectsStarNN(stoi(st.arg1), stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isAffectsT());
    } else if (st.relation == QueryClause::relationshipType::NextBip || st.relation == QueryClause::relationshipType::NextBipT
               || st.relation == QueryClause::relationshipType::AffectsBip || st.relation == QueryClause::relationshipType::AffectsBipT) {
        return evaluateSuchThatNNExtension(st);
    } else {
        return QueryClauseResult::getInvalidAsPtr();
    }
}

QueryClauseResult* SuchThatEvaluator::evaluateSuchThatNT(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int,int>>();
    vector<pair<int, string>>* list2 = new vector<pair<int,string>>();
    vector<pair<string, int>>* list3 = new vector<pair<string,int>>();
    vector<pair<string, string>>* list4 = new vector<pair<string,string>>();
    bool canBeNonExact = optimiser.canBeNonExact(QueryClause(st));
    if (st.relation == QueryClause::relationshipType::Uses) {
        *list2 = PKB::getInstance().getUsesSST(stoi(st.arg1), Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, string>(st, list2, canBeNonExact, CustomFunctions::isUses());
    } else if (st.relation == QueryClause::relationshipType::UsesProc) {
        *list4 = PKB::getInstance().getUsesPPT(st.arg1, Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isUsesProc());
    } else if (st.relation == QueryClause::relationshipType::Modifies) {
        *list2 = PKB::getInstance().getModifiesSST(stoi(st.arg1),Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, string>(st, list2, canBeNonExact, CustomFunctions::isModifies());
    } else if (st.relation == QueryClause::relationshipType::ModifiesProc) {
        *list4 = PKB::getInstance().getModifiesPPT(st.arg1, Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isModifiesProc());
    } else if (st.relation == QueryClause::relationshipType::Follows) {
        *list1 = PKB::getInstance().getFollowsNT(stoi(st.arg1),Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isFollows());
    } else if (st.relation == QueryClause::relationshipType::FollowsT) {
        *list1 = PKB::getInstance().getFollowsStarNT(stoi(st.arg1),Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isFollowsT());
    } else if (st.relation == QueryClause::relationshipType::Parent) {
        *list1 = PKB::getInstance().getParentNT(stoi(st.arg1),Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isParent());
    } else if (st.relation == QueryClause::relationshipType::ParentT) {
        *list1 = PKB::getInstance().getParentStarNT(stoi(st.arg1),Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isParentT());
    } else if (st.relation == QueryClause::relationshipType::Calls) {
        *list4 = PKB::getInstance().getCallsPPT(st.arg1, Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isCalls());
    } else if (st.relation == QueryClause::relationshipType::CallsT) {
        *list4 = PKB::getInstance().getCallsStarNT(st.arg1, Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isCallsT());
    } else if (st.relation == QueryClause::relationshipType::Next) {
        *list1 = PKB::getInstance().getNextNT(stoi(st.arg1),Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isNext());
    } else if (st.relation == QueryClause::relationshipType::NextT || st.relation == QueryClause::relationshipType::Affects
               || st.relation == QueryClause::relationshipType::AffectsT) {
        return evaluateSuchThatNTOptimised(st);
    } else if (st.relation == QueryClause::relationshipType::NextBip || st.relation == QueryClause::relationshipType::NextBipT
               || st.relation == QueryClause::relationshipType::AffectsBip || st.relation == QueryClause::relationshipType::AffectsBipT) {
        return evaluateSuchThatNTExtension(st);
    } else {
        return QueryClauseResult::getInvalidAsPtr();
    }
}

QueryClauseResult* SuchThatEvaluator::evaluateSuchThatTN(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int,int>>();
    vector<pair<int, string>>* list2 = new vector<pair<int,string>>();
    vector<pair<string, int>>* list3 = new vector<pair<string,int>>();
    vector<pair<string, string>>* list4 = new vector<pair<string,string>>();
    bool canBeNonExact = optimiser.canBeNonExact(QueryClause(st));
    if (st.relation == QueryClause::relationshipType::Uses) {
        *list2 = PKB::getInstance().getUsesSTV(Query::getInstance().getStatementTypeOfDec(st.arg1), st.arg2);
        return convertToReturnItem<int, string>(st, list2, canBeNonExact, CustomFunctions::isUses());
    } else if (st.relation == QueryClause::relationshipType::UsesProc) {
        *list4 = PKB::getInstance().getUsesPTV(Query::getInstance().getStatementTypeOfDec(st.arg1),st.arg2);
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isUsesProc());
    } else if (st.relation == QueryClause::relationshipType::Modifies) {
        *list2 = PKB::getInstance().getModifiesSTV(Query::getInstance().getStatementTypeOfDec(st.arg1),st.arg2);
        return convertToReturnItem<int, string>(st, list2, canBeNonExact, CustomFunctions::isModifies());
    } else if (st.relation == QueryClause::relationshipType::ModifiesProc) {
        *list4 = PKB::getInstance().getModifiesPTV(Query::getInstance().getStatementTypeOfDec(st.arg1),st.arg2);
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isModifiesProc());
    } else if (st.relation == QueryClause::relationshipType::Follows) {
        *list1 = PKB::getInstance().getFollowsTN(Query::getInstance().getStatementTypeOfDec(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isFollows());
    } else if (st.relation == QueryClause::relationshipType::FollowsT) {
        *list1 = PKB::getInstance().getFollowsStarTN(Query::getInstance().getStatementTypeOfDec(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isFollowsT());
    } else if (st.relation == QueryClause::relationshipType::Parent) {
        *list1 = PKB::getInstance().getParentTN(Query::getInstance().getStatementTypeOfDec(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isParent());
    } else if (st.relation == QueryClause::relationshipType::ParentT) {
        *list1 = PKB::getInstance().getParentStarTN(Query::getInstance().getStatementTypeOfDec(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isParentT());
    } else if (st.relation == QueryClause::relationshipType::Calls) {
        *list4 = PKB::getInstance().getCallsPTP(Query::getInstance().getStatementTypeOfDec(st.arg1),st.arg2);
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isCalls());
    } else if (st.relation == QueryClause::relationshipType::CallsT) {
        *list4 = PKB::getInstance().getCallsStarTN(Query::getInstance().getStatementTypeOfDec(st.arg1),st.arg2);
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isCallsT());
    } else if (st.relation == QueryClause::relationshipType::Next) {
        *list1 = PKB::getInstance().getNextTN(Query::getInstance().getStatementTypeOfDec(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isNext());
    } else if (st.relation == QueryClause::relationshipType::NextT || st.relation == QueryClause::relationshipType::Affects
               || st.relation == QueryClause::relationshipType::AffectsT) {
        return evaluateSuchThatTNOptimised(st);
    } else if (st.relation == QueryClause::relationshipType::NextBip || st.relation == QueryClause::relationshipType::NextBipT
               || st.relation == QueryClause::relationshipType::AffectsBip || st.relation == QueryClause::relationshipType::AffectsBipT) {
        return evaluateSuchThatTNExtension(st);
    } else {
        return QueryClauseResult::getInvalidAsPtr();
    }
}

QueryClauseResult* SuchThatEvaluator::evaluateSuchThatTT(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int,int>>();
    vector<pair<int, string>>* list2 = new vector<pair<int,string>>();
    vector<pair<string, int>>* list3 = new vector<pair<string,int>>();
    vector<pair<string, string>>* list4 = new vector<pair<string,string>>();
    bool canBeNonExact = optimiser.canBeNonExact(QueryClause(st));
    if (st.relation == QueryClause::relationshipType::Uses) {
        *list2 = PKB::getInstance().getUsesSTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                               Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, string>(st, list2, canBeNonExact, CustomFunctions::isUses());
    } else if (st.relation == QueryClause::relationshipType::UsesProc) {
        *list4 = PKB::getInstance().getUsesPTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                               Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isUsesProc());
    } else if (st.relation == QueryClause::relationshipType::Modifies) {
        *list2 = PKB::getInstance().getModifiesSTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                                   Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, string>(st, list2, canBeNonExact, CustomFunctions::isModifies());
    } else if (st.relation == QueryClause::relationshipType::ModifiesProc) {
        *list4 = PKB::getInstance().getModifiesPTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                                   Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isModifiesProc());
    } else if (st.relation == QueryClause::relationshipType::Follows) {
        *list1 = PKB::getInstance().getFollowsTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                                 Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isFollows());
    } else if (st.relation == QueryClause::relationshipType::FollowsT) {
        *list1 = PKB::getInstance().getFollowsStarTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                                     Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isFollowsT());
    } else if (st.relation == QueryClause::relationshipType::Parent) {
        *list1 = PKB::getInstance().getParentTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                                Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isParent());
    } else if (st.relation == QueryClause::relationshipType::ParentT) {
        *list1 = PKB::getInstance().getParentStarTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                                    Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isParentT());
    } else if (st.relation == QueryClause::relationshipType::Calls) {
        *list4 = PKB::getInstance().getCallsPTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                                Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isCalls());
    } else if (st.relation == QueryClause::relationshipType::CallsT) {
        *list4 = PKB::getInstance().getCallsStarTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                                   Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<string, string>(st, list4, canBeNonExact, CustomFunctions::isCallsT());
    } else if (st.relation == QueryClause::relationshipType::Next) {
        *list1 = PKB::getInstance().getNextTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                              Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isNext());
    } else if (st.relation == QueryClause::relationshipType::NextT || st.relation == QueryClause::relationshipType::Affects
               || st.relation == QueryClause::relationshipType::AffectsT) {
        return evaluateSuchThatTTOptimised(st);
    } else if (st.relation == QueryClause::relationshipType::NextBip || st.relation == QueryClause::relationshipType::NextBipT
               || st.relation == QueryClause::relationshipType::AffectsBip || st.relation == QueryClause::relationshipType::AffectsBipT) {
        return evaluateSuchThatTTExtension(st);
    }  else {
        return QueryClauseResult::getInvalidAsPtr();
    }
}

QueryClauseResult* SuchThatEvaluator::evaluateSuchThatNTOptimised(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int, int>>();
    bool canBeNonExact = optimiser.canBeNonExact(QueryClause(st));
    if (st.relation == QueryClause::relationshipType::NextT) {
        if (canBeNonExact) {
            return new QueryClauseResult(PKB::getInstance().getNextStarNTBoolean(stoi(st.arg1), Query::getInstance().getStatementTypeOfDec(st.arg2)));
        }
        *list1 = PKB::getInstance().getNextStarNT(stoi(st.arg1),Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, false, CustomFunctions::isNextT());
    } else if (st.relation == QueryClause::relationshipType::Affects) {
        // TODO: check if both are assign types, to be implemented
        StatementTypes arg2Type = Query::getInstance().getStatementTypeOfDec(st.arg2);
        if(!validateAffectsNT(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        if (canBeNonExact) {
            return new QueryClauseResult(PKB::getInstance().getAffectsNTBoolean(stoi(st.arg1), arg2Type));
        }
        *list1 = PKB::getInstance().getAffectsNT(stoi(st.arg1), arg2Type);
        return convertToReturnItem<int, int>(st, list1, false, CustomFunctions::isAffects());
    } else if (st.relation == QueryClause::relationshipType::AffectsT) {
        // TODO: check if both are assign types, to be implemented
        StatementTypes arg2Type = Query::getInstance().getStatementTypeOfDec(st.arg2);
        if(!validateAffectsNT(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        if (canBeNonExact) {
            return new QueryClauseResult(PKB::getInstance().getAffectsStarNTBoolean(stoi(st.arg1), arg2Type));
        }
        *list1 = PKB::getInstance().getAffectsStarNT(stoi(st.arg1), arg2Type);
        return convertToReturnItem<int, int>(st, list1, false, CustomFunctions::isAffectsT());
    }
    return QueryClauseResult::getInvalidAsPtr();
}

QueryClauseResult* SuchThatEvaluator::evaluateSuchThatTNOptimised(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int, int>>();
    bool canBeNonExact = optimiser.canBeNonExact(QueryClause(st));
    if (st.relation == QueryClause::relationshipType::NextT) {
        if (canBeNonExact) {
            return new QueryClauseResult(PKB::getInstance().getNextStarTNBoolean(Query::getInstance().getStatementTypeOfDec(st.arg1), stoi(st.arg2)));
        }
        *list1 = PKB::getInstance().getNextStarTN(Query::getInstance().getStatementTypeOfDec(st.arg1), stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, false, CustomFunctions::isNextT());
    } else if (st.relation == QueryClause::relationshipType::Affects) {
        // TODO: check if both are assign types, to be implemented
        StatementTypes arg1Type = Query::getInstance().getStatementTypeOfDec(st.arg1);
        if(!validateAffectsTN(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        if (canBeNonExact) {
            return new QueryClauseResult(PKB::getInstance().getAffectsTNBoolean(arg1Type, stoi(st.arg2)));
        }
        *list1 = PKB::getInstance().getAffectsTN(arg1Type, stoi((st.arg2)));
        return convertToReturnItem<int, int>(st, list1, false, CustomFunctions::isAffects());
    } else if (st.relation == QueryClause::relationshipType::AffectsT) {
        // TODO: check if both are assign types, to be implemented
        StatementTypes arg1Type = Query::getInstance().getStatementTypeOfDec(st.arg1);
        if(!validateAffectsTN(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        if (canBeNonExact) {
            return new QueryClauseResult(PKB::getInstance().getAffectsStarTNBoolean(arg1Type, stoi(st.arg2)));
        }
        *list1 = PKB::getInstance().getAffectsStarTN(arg1Type, stoi((st.arg2)));
        return convertToReturnItem<int, int>(st, list1, false, CustomFunctions::isAffectsT());
    }
    return QueryClauseResult::getInvalidAsPtr();
}

QueryClauseResult* SuchThatEvaluator::evaluateSuchThatTTOptimised(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int, int>>();
    bool canBeNonExact = optimiser.canBeNonExact(QueryClause(st));
    if (st.relation == QueryClause::relationshipType::NextT) {
        if (canBeNonExact) {
            return new QueryClauseResult(PKB::getInstance().getNextStarTTBoolean(Query::getInstance().getStatementTypeOfDec(st.arg1), Query::getInstance().getStatementTypeOfDec(st.arg2)));
        }
        *list1 = PKB::getInstance().getNextStarTT(Query::getInstance().getStatementTypeOfDec(st.arg1),
                                                  Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, false, CustomFunctions::isNextT());
    } else if (st.relation == QueryClause::relationshipType::Affects) {
        // TODO: check if both are assign types, to be implemented
        StatementTypes arg1Type = Query::getInstance().getStatementTypeOfDec(st.arg1);
        StatementTypes arg2Type = Query::getInstance().getStatementTypeOfDec(st.arg2);
        if(!validateAffectsTT(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        if (canBeNonExact) {
            return new QueryClauseResult(PKB::getInstance().getAffectsTTBoolean(arg1Type, arg2Type));
        }
        *list1 = PKB::getInstance().getAffectsTT(Query::getInstance().getStatementTypeOfDec(st.arg1), Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, false, CustomFunctions::isAffects());
    } else if (st.relation == QueryClause::relationshipType::AffectsT) {
        // TODO: check if both are assign types, to be implemented
        StatementTypes arg1Type = Query::getInstance().getStatementTypeOfDec(st.arg1);
        StatementTypes arg2Type = Query::getInstance().getStatementTypeOfDec(st.arg2);
        if(!validateAffectsTT(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        if (canBeNonExact) {
            return new QueryClauseResult(PKB::getInstance().getAffectsStarTTBoolean(arg1Type, arg2Type));
        }
        *list1 = PKB::getInstance().getAffectsStarTT(arg1Type, arg2Type);
        return convertToReturnItem<int, int>(st, list1, false, CustomFunctions::isAffectsT());
    }
    return QueryClauseResult::getInvalidAsPtr();
}

QueryClauseResult *SuchThatEvaluator::evaluateSuchThatNNExtension(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int, int>>();
    if (st.relation == QueryClause::relationshipType::NextBip) {
        *list1 = PKB::getInstance().getNextBipNN(stoi(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isNextBip());
    } else if (st.relation == QueryClause::relationshipType::NextBipT) {
        *list1 = PKB::getInstance().getNextBipStarNN(stoi(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isNextBipT());
    } else if (st.relation == QueryClause::relationshipType::AffectsBip) {
        if(!validateAffectsNN(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        *list1 = PKB::getInstance().getAffectsBipNN(stoi(st.arg1),stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, true, CustomFunctions::isAffectsBip());
    }
    return QueryClauseResult::getInvalidAsPtr();
}

QueryClauseResult *SuchThatEvaluator::evaluateSuchThatNTExtension(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int, int>>();
    bool canBeNonExact = optimiser.canBeNonExact(QueryClause(st));
    if (st.relation == QueryClause::relationshipType::NextBip) {
        *list1 = PKB::getInstance().getNextBipNT(stoi(st.arg1),Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isNextBip());
    } else if (st.relation == QueryClause::relationshipType::NextBipT) {
        *list1 = PKB::getInstance().getNextBipStarNT(stoi(st.arg1),Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isNextBipT());
    } else if (st.relation == QueryClause::relationshipType::AffectsBip) {
        if(!validateAffectsNT(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        *list1 = PKB::getInstance().getAffectsBipNT(stoi(st.arg1),Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isAffectsBip());
    }
    return QueryClauseResult::getInvalidAsPtr();
}

QueryClauseResult *SuchThatEvaluator::evaluateSuchThatTNExtension(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int, int>>();
    bool canBeNonExact = optimiser.canBeNonExact(QueryClause(st));
    if (st.relation == QueryClause::relationshipType::NextBip) {
        *list1 = PKB::getInstance().getNextBipTN(Query::getInstance().getStatementTypeOfDec(st.arg1), stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isNextBip());
    } else if (st.relation == QueryClause::relationshipType::NextBipT) {
        *list1 = PKB::getInstance().getNextBipStarTN(Query::getInstance().getStatementTypeOfDec(st.arg1), stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isNextBipT());
    } else if (st.relation == QueryClause::relationshipType::AffectsBip) {
        if(!validateAffectsTN(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        *list1 = PKB::getInstance().getAffectsBipTN(Query::getInstance().getStatementTypeOfDec(st.arg1), stoi(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isAffectsBip());
    }
    return QueryClauseResult::getInvalidAsPtr();
}

QueryClauseResult *SuchThatEvaluator::evaluateSuchThatTTExtension(QueryClause::suchThat st) {
    vector<pair<int, int>>* list1 = new vector<pair<int, int>>();
    bool canBeNonExact = optimiser.canBeNonExact(QueryClause(st));
    if (st.relation == QueryClause::relationshipType::NextBip) {
        *list1 = PKB::getInstance().getNextBipTT(Query::getInstance().getStatementTypeOfDec(st.arg1), Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isNextBip());
    } else if (st.relation == QueryClause::relationshipType::NextBipT) {
        *list1 = PKB::getInstance().getNextBipStarTT(Query::getInstance().getStatementTypeOfDec(st.arg1), Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isNextBipT());
    } else if (st.relation == QueryClause::relationshipType::AffectsBip) {
        if(!validateAffectsTT(st.arg1, st.arg2)) {
            return QueryClauseResult::getInvalidAsPtr();
        }
        *list1 = PKB::getInstance().getAffectsBipTT(Query::getInstance().getStatementTypeOfDec(st.arg1), Query::getInstance().getStatementTypeOfDec(st.arg2));
        return convertToReturnItem<int, int>(st, list1, canBeNonExact, CustomFunctions::isAffectsBip());
    }
    return QueryClauseResult::getInvalidAsPtr();
}

SuchThatEvaluator::SuchThatEvaluator(QueryOptimiser op) {
    optimiser = op;
}

