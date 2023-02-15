#include "Parser/Tokeniser.h"
#include "QueryPreprocessor.h"
#include "Query.h"
#include "QueryValidator.h"
#include "QPPUtil.h"
#include <stdio.h>
#include <string>
#include <utility>
#include <stack>
#include <vector>

const string SELECT = "Select";
const string SUCH_THAT = "such that ";
const string PATTERN = "pattern ";
const string AND = "and ";
const string WITH = "with ";

queue<string>* QueryPreprocessor::declarations = new queue<string>();
string QueryPreprocessor::queryStmt = "";

Query &qe3 = Query::getInstance();

/////////////// SET UP PARSING ///////////////

bool QueryPreprocessor::splitQuery(std::string query) {
    std::string delimiter = ";";
    size_t pos = query.find(delimiter);
    while (pos != std::string::npos) {
        addDeclaration(query.substr(0, pos));
        query = query.substr(pos + 1);
        QPPUtil::trimAllWhitespaces(query);
        pos = query.find(delimiter);
    }
    QPPUtil::trimAllWhitespaces(query);
    if (query.empty()) {
        declarations = new queue<string>();
        qe3.setSyntaxError();
        return false;
    } else {
        setQuery(query);
    }
    return true;
}

bool QueryPreprocessor::addDeclaration(std::string declaration) {
    QPPUtil::trimAllWhitespaces(declaration);
    declarations->push(declaration);
    return true;
}

bool QueryPreprocessor::setQuery(std::string& q) {
    QPPUtil::trimAllWhitespaces(queryStmt);
    QueryPreprocessor::queryStmt = q;
    return true;
}

/////////////// PARSING CLAUSES METHODS ///////////////

bool QueryPreprocessor::parse(string query) {
    qe3.reset();
    declarations = new queue<string>();
    queryStmt = "";

    splitQuery(query);
    if (qe3.isSyntaxError()) {
        return false;
    }
    parseDeclaration();
    if (qe3.isSyntaxError()) {
        return false;
    }
    parseResultClause(queryStmt);
    if (qe3.isSyntaxError()) {
        return false;
    }
    if (queryStmt.empty()) {
        return true;
    }
    parseClauses(queryStmt);
    if (qe3.isSyntaxError()) {
        return false;
    }
    return true;
}

bool QueryPreprocessor::parseDeclaration() {
    std::string delimiter = " ";
    while (!declarations->empty()) {
        string declaration = declarations->front();
        QPPUtil::trimAllWhitespaces(declaration);
        declarations->pop();
        size_t pos = declaration.find(delimiter);
        if (pos == string::npos) {
            qe3.setSyntaxError();
            return false;
        }
        string entity = declaration.substr(0, pos);
        QPPUtil::trimAllWhitespaces(entity);
        Query::entityType entityType = qe3.getEntityType(entity);
        string synonyms = declaration.substr(pos + 1);
        QPPUtil::trimAllWhitespaces(synonyms);
        while (!synonyms.empty()) {
            size_t comma = synonyms.find(",");
            string syn = synonyms.substr(0, comma);
            QPPUtil::trimAllWhitespaces(syn);
            if (QueryValidator::isDeclarationValid(entityType, syn)) {
                qe3.addDeclarationSynonym(syn, entityType);
            } else {
                return false;
            }
            if (comma == string::npos) {
                synonyms = "";
            } else {
                synonyms = synonyms.substr(comma + 1);
                QPPUtil::trimAllWhitespaces(synonyms);
            }
        }
    }
    return true;
}

bool QueryPreprocessor::parseResultClause(std::string &query) {
    if (!QueryValidator::isSelectExist(query)) {
        return false;
    }
    parseSelectVar(query);
    if (qe3.isSyntaxError()) {
        return false;
    }
    return true;
}

void QueryPreprocessor::processSuchThat(string &query) {
    int min = query.find(")");
    if (min == -1) {
        qe3.setSyntaxError();
        return;
    } else {
        pair<string, string> processed = QPPUtil::extractQuery(min + 1, query);
        query = processed.second;
        parseSuchThat(processed.first);
    }
}

void QueryPreprocessor::processPattern(string &query) {
    int min = QPPUtil::findNextClausePos(vector{query.find("_)"), query.find("\")")});
    if (min == -1) {
        qe3.setSyntaxError();
        return;
    } else {
        pair<string, string> processed = QPPUtil::extractQuery(min + 2, query);
        query = processed.second;
        parsePattern(processed.first);
    }
}

void QueryPreprocessor::processWith(string &query) {
    pair<string, string> extracted = parseWithContent(query);

    if (extracted.first.empty()) {
        qe3.setSyntaxError();
        return;
    }
    parseWith(extracted.first, extracted.second);
}

void QueryPreprocessor::processAnd(string &query, clauseType prevClause) {
    query.erase(0, AND.size());
    QPPUtil::trimAllWhitespaces(query);

    parseAnd(query, prevClause);
}

bool QueryPreprocessor::parseClauses(string &query) {
    size_t suchThatPos = query.find(SUCH_THAT);
    size_t patternPos = query.find(PATTERN);
    size_t withPos = query.find(WITH);
    size_t andPos = query.find(AND);

    clauseType prevClause = clauseType::noType;
    do {
        if (suchThatPos == 0) {
            query.erase(0, SUCH_THAT.size());
            QPPUtil::trimAllWhitespaces(query);
            processSuchThat(query);
            prevClause = clauseType::suchThat;
        } else if (patternPos == 0) {
            query.erase(0, PATTERN.size());
            QPPUtil::trimAllWhitespaces(query);
            processPattern(query);
            prevClause = clauseType::pattern;
        } else if (withPos == 0) {
            query.erase(0, WITH.size());
            QPPUtil::trimAllWhitespaces(query);
            processWith(query);
            prevClause = clauseType::with;
        } else if (andPos == 0) {
            processAnd(query, prevClause);
        } else {
            qe3.setSyntaxError();
            return false;
        }
        if (qe3.isSyntaxError()) {
            return false;
        }

        QPPUtil::trimAllWhitespaces(query);
        suchThatPos = query.find(SUCH_THAT);
        patternPos = query.find(PATTERN);
        withPos = query.find(WITH);
        andPos = query.find(AND);
    } while (!query.empty());
    return true;
}

bool QueryPreprocessor::parseVarSelect(std::string name) {
    Query::selectFlag selectFLag = Query::selectFlag::isSynonymName;
    if (isAttrExist(name)) {
        size_t dot = name.find(".");
        // is "v     .     varName" valid? --> assume so first
        string syn = name.substr(0, dot);
        string attrRef = name.substr(dot + 1);
        QPPUtil::trimAllWhitespaces(syn);
        QPPUtil::trimAllWhitespaces(attrRef);

        if (!QueryValidator::isValidSynRegex(syn) || QueryClause::getAttributeType(attrRef) == QueryClause::attribute::WRONG_ATTR) {
            qe3.setSyntaxError();
            return false;
        }
        selectFLag = QueryValidator::isSelectVarValid(syn, attrRef);
        if (selectFLag == Query::selectFlag::isInvalid) {
            qe3.isSemanticError();
            return false;
        }
        qe3.addSelectSynonym(syn, selectFLag);
    } else {
        if (!QueryValidator::isSelectVarDeclared(name)) {
            qe3.setSemanticError();
            return false;
        } else {
            qe3.addSelectSynonym(name, selectFLag);
        }
    }
    return true;
}

bool QueryPreprocessor::parseTuple(std::string &query) {
    if (!QueryValidator::isValidTuple(query)) {
        return false;
    }

    string var = query.substr(1, query.find(">") - 1);
    QPPUtil::trimAllWhitespaces(var);

    string delimiter = ",";
    size_t pos = var.find(delimiter);

    while (!var.empty()) {
        string name = var.substr(0, pos);
        QPPUtil::trimAllWhitespaces(name);
        if (pos == var.size() - 1) {
            qe3.setSyntaxError();
            return false;
        }

        if (!QueryPreprocessor::parseVarSelect(name)) {
            qe3.setSemanticError();
            return false;
        }
        if (pos == string::npos) {
            var = "";
        } else {
            var = var.substr(pos + 1);
            pos = var.find(delimiter);
        }
    }
    return true;
}

bool QueryPreprocessor::parseNonTuple(std::string &query) {
    std::string delimiter = " ";
    size_t pos = query.find(delimiter);

    string var = query.substr(0, pos);
    QPPUtil::trimAllWhitespaces(var);
    Query::selectFlag selectFLag = Query::selectFlag::isSynonymName;
    if (var.compare("BOOLEAN") == 0) {
        if (!QueryValidator::isSelectVarDeclared(var)) {
            selectFLag = Query::selectFlag::isBoolean;
        }
        qe3.addSelectSynonym(var, selectFLag);
    } else {
        QueryPreprocessor::parseVarSelect(var);
        if (qe3.isSyntaxError()) {
            return false;
        }
    }
    if (pos == string::npos) {
        query = "";
    } else {
        query = query.erase(0, pos + 1);
    }
    QPPUtil::trimAllWhitespaces(query);
    return true;
}

bool QueryPreprocessor::parseSelectVar(std::string& query) {
    query = query.substr(6);
    QPPUtil::trimAllWhitespaces(query);
    if (query.find("<") == 0) {
        parseTuple(query);
        if (qe3.isSyntaxError()) {
            return false;
        }
        // ending
        query = query.erase(0, query.find(">") + 1);
        QPPUtil::trimAllWhitespaces(query);
    } else {
        parseNonTuple(query);
        if (qe3.isSyntaxError()) {
            return false;
        }
    }
    return true;
}

bool QueryPreprocessor::parseSuchThat(std::string &query) {
    size_t pos = QPPUtil::getSmallestPos(query.find(" "), query.find("("));
    if (pos == -1) {
        qe3.setSyntaxError();
    }
    string relationship = query.substr(0, pos);
    QueryClause::relationshipType relationshipType = QueryClause::getRelationshipType(QPPUtil::trimAllWhitespaces(relationship));
    query.erase(0, pos);
    QPPUtil::trimAllWhitespaces(query);
    if (!QueryValidator::isBracketValid(query)) {
        qe3.setSyntaxError();
        return false;
    }
    string bracket = query.substr(1, query.find(")") - 1);
    if (!QueryValidator::isContentExist(bracket)) {
        qe3.setSyntaxError();
        return false;
    }
    vector<string> arguments = parseBracket(bracket);
    query.erase(0, query.find(")") + 1);
    QPPUtil::trimAllWhitespaces(query);
    if (!QueryValidator::isValidSuchThat(relationshipType, arguments)) {
        return false;
    }
    string lhs = arguments.at(0);
    string rhs = arguments.at(1);
    QueryClause::suchThatFlag leftFlag = qe3.getSuchThatFlagFromString(lhs);
    QueryClause::suchThatFlag rightFlag = qe3.getSuchThatFlagFromString(rhs);
    QPPUtil::stripArgument(lhs);
    QPPUtil::stripArgument(rhs);

    qe3.addSuchThat(relationshipType, lhs, leftFlag, rhs, rightFlag);
    return true;
}

bool QueryPreprocessor::isAttrExist(string query) {
    if (query.find(".") != string::npos) {
        return true;
    }
    return false;
}

bool QueryPreprocessor::parsePattern(std::string &query) {
    size_t pos = QPPUtil::getSmallestPos(query.find(" "), query.find("("));
    if (pos == -1) {
        qe3.setSyntaxError();
        return false;
    }
    string patternSyn = query.substr(0, pos);
    query.erase(0, pos);
    QPPUtil::trimAllWhitespaces(query);

    if (!QueryValidator::isBracketValid(query)) {
        qe3.setSyntaxError();
        return false;
    }
    string content = query.substr(1, query.find_last_of(")") - 1);
    QPPUtil::trimAllWhitespaces(content);
    if (!QueryValidator::isContentExist(content)) {
        qe3.setSyntaxError();
        return false;
    }
    vector<string> arguments = parseBracket(content);
    if (!QueryValidator::isSufficientAndSyntacticalArguments(patternSyn, arguments)) {
        qe3.setSyntaxError();
        return false;
    }
    query.erase(0, query.find_last_of(")") + 1);
    if (!QueryValidator::isValidPattern(patternSyn, arguments)) {
        return false;
    }

    string rhs = arguments.at(1);
    if (rhs.compare("_") != 0) {
        string subexpr = rhs;
        arguments[1] = QPPUtil::stripExtraBracketsInSubString(subexpr);
    }

    QPPUtil::trimAllWhitespaces(query);
    string lhs = arguments.at(0);
    QueryClause::patternFlag leftFlag = QueryClause::getStringAsPatternFlag(lhs);
    QPPUtil::stripArgument(lhs);
    QueryClause::patternFlag rightFlag = QueryClause::getStringAsPatternFlag(rhs);

    qe3.addPattern(patternSyn, lhs, leftFlag, arguments.at(1), rightFlag);
    return true;
}

pair<string, string> QueryPreprocessor::parseWithContent(std::string &query) {
    size_t pos = query.find("=");
    if (pos == string::npos) {
        return make_pair("", "");
    }
    string first = query.substr(0, pos);
    query.erase(0, pos);
    QPPUtil::trimAllWhitespaces(query);
    query.erase(0, query.find("=") + 1);
    QPPUtil::trimAllWhitespaces(query);
    pos = query.find(" ");
    string second = "";
    if (pos == string::npos) {
        second = query;
        query = "";
    } else {
        second = query.substr(0, pos);
        QPPUtil::trimAllWhitespaces(second);
        query.erase(0, pos + 1);
        QPPUtil::trimAllWhitespaces(query);
    }
    return make_pair(first, second);
}

bool QueryPreprocessor::parseWith(std::string first, std::string second) {
    pair<string, QueryClause::attribute> left = splitAttr(first);
    pair<string, QueryClause::attribute> right = splitAttr(second);

    if (!QueryValidator::isValidWith(left.first, left.second, right.first, right.second)) {
        return false;
    }

    QueryClause::withFlag leftFlag = QueryClause::withFlag::isDeclaration;
    QueryClause::withFlag rightFlag = QueryClause::withFlag::isDeclaration;
    string lhs = left.first;
    string rhs = right.first;
    if (left.second == QueryClause::attribute::NO_ATTR) {
        leftFlag = qe3.getWithFlagFromString(lhs);
        QPPUtil::stripArgument(lhs);
    } else {
        leftFlag = qe3.getWithFlagFromAttribute(left.second);
    }

    if (right.second == QueryClause::attribute::NO_ATTR) {
        rightFlag = qe3.getWithFlagFromString(rhs);
        QPPUtil::stripArgument(rhs);
    } else {
        rightFlag = qe3.getWithFlagFromAttribute(right.second);
    }
    qe3.addWith(lhs, leftFlag, rhs, rightFlag);
    return true;
}

bool QueryPreprocessor::parseAnd(std::string &query, clauseType prevClause) {

    size_t pos = QPPUtil::getSmallestPos(query.find(" "), query.find("("));
    if (pos == -1) {
        if (prevClause == clauseType::with) {
            processWith(query);
            return true;
        } else {
            qe3.setSyntaxError();
            return false;
        }
    }
    string firstWord = query.substr(0, pos);
    QPPUtil::trimAllWhitespaces(firstWord);

    string secondWord = query.substr(pos);
    QPPUtil::trimAllWhitespaces(secondWord);

    QueryClause::relationshipType rType = QueryClause::getRelationshipType(firstWord);
    if (rType != QueryClause::relationshipType::WRONG_TYPE) {
        if (prevClause == clauseType::suchThat) {
            parseSuchThat(query);
        } else {
            qe3.setSyntaxError();
            return false;
        }
    } else if (secondWord.find("(") == 0) {
        if (prevClause == clauseType::pattern) {
            processPattern(query);
        } else {
            qe3.setSyntaxError();
            return false;
        }
    } else {
        if (query.find("=") != string::npos) {
            if (prevClause == clauseType::with) {
                processWith(query);
            } else {
                qe3.setSyntaxError();
                return false;
            }
        } else {
            qe3.setSyntaxError();
            return false;
        }
    }
    return true;
}

std::vector<string> QueryPreprocessor::parseBracket(std::string query) {
    vector<string> result;
    QPPUtil::trimAllWhitespaces(query);
    string delimiter = ",";
    size_t pos = query.find(delimiter);
    // no comma at all
    if (pos == std::string::npos) {
        qe3.setSyntaxError();
    }
    while (pos != string::npos) {
        string arg = query.substr(0, pos);
        QPPUtil::trimAllWhitespaces(arg);
        result.push_back(arg);
        query = query.substr(pos + 1);
        pos = query.find(delimiter);
    }
    QPPUtil::trimAllWhitespaces(query);
    result.push_back(query);

    return result;
}

QueryPreprocessor::clauseType QueryPreprocessor::getClauseType(string clause) {
    if (clause.compare(SUCH_THAT) == 0) {
        return clauseType::suchThat;
    } else if (clause.compare(PATTERN) == 0) {
        return clauseType::pattern;
    } else if (clause.compare(WITH) == 0) {
        return clauseType::with;
    } else {
        return clauseType::noType;
    }
}

// validate before calling this step
pair<string, QueryClause::attribute> QueryPreprocessor::splitAttr(string attr) {
    size_t dot = attr.find(".");
    string first = attr.substr(0, dot);
    QPPUtil::trimAllWhitespaces(first);
    if (dot != string::npos) {
        string second = attr.substr(dot + 1);
        QPPUtil::trimAllWhitespaces(second);
        QueryClause::attribute attribute = QueryClause::getAttributeType(second);
        return make_pair(first, attribute);
    } else {
        return make_pair(first, QueryClause::attribute::NO_ATTR);
    }
}

bool QueryPreprocessor::resetQE() {
    QueryPreprocessor::declarations = new queue<string>();
    QueryPreprocessor::queryStmt = "";
    return qe3.reset();
}