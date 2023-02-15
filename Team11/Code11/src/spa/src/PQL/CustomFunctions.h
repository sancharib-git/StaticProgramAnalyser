#ifndef AUTOTESTER_CUSTOMFUNCTIONS_H
#define AUTOTESTER_CUSTOMFUNCTIONS_H
#include "PKB/PKB.h"
#include "QueryElement.h"

class CustomFunctions {
public:
    struct isFollows {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getFollowsNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
    struct isFollowsT {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getFollowsStarNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
    struct isParent {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getParentNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
    struct isParentT {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getParentStarNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
    struct isUsesProc {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getUsesPPV(arg1.STRING, arg2.STRING).empty();
        }
    };
    struct isUses {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getUsesSSV(arg1.INTEGER, arg2.STRING).empty();
        }
    };
    struct isModifiesProc {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getModifiesPPV(arg1.STRING, arg2.STRING).empty();
        }
    };
    struct isModifies {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getModifiesSSV(arg1.INTEGER, arg2.STRING).empty();
        }
    };
    struct isCalls {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getCallsPPP(arg1.STRING, arg2.STRING).empty();
        }
    };
    struct isCallsT {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getCallsStarNN(arg1.STRING, arg2.STRING).empty();
        }
    };
    struct isNext {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getNextNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
    struct isNextT {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getNextStarNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
    struct isAffects {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getAffectsNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
    struct isAffectsT {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getAffectsStarNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
    struct isNextBip {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getNextBipNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
    struct isNextBipT {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getNextBipStarNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
    struct isAffectsBip {
        bool operator() (QueryElement arg1, QueryElement arg2) {
            return !PKB::getInstance().getAffectsBipNN(arg1.INTEGER, arg2.INTEGER).empty();
        }
    };
};


#endif //AUTOTESTER_CUSTOMFUNCTIONS_H
