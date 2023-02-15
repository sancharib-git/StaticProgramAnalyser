#include "QueryElement.h"
QueryElement::QueryElement() {
    *this = QueryElement::INVALID();
}

bool QueryElement::operator==(const QueryElement& wrap) const
{
    if (TYPE == type::string) {
        return (wrap.TYPE == type::string) && (wrap.STRING == STRING);
    } else if (TYPE == type::integer) {
        return (wrap.TYPE == type::integer) && (wrap.INTEGER == INTEGER);
    } else {
        return (TYPE == wrap.TYPE) && (STRING == wrap.STRING) && (INTEGER == wrap.INTEGER);
    }
}

bool QueryElement::operator<(const QueryElement& wrap) const
{
    if (TYPE == type::string) {
        return (wrap.TYPE == type::string) && (wrap.STRING < STRING);
    } else if (TYPE == type::integer) {
        return (wrap.TYPE == type::integer) && (wrap.INTEGER < INTEGER);
    } else {
        return false;
    }
}

bool QueryElement::lessThan(QueryElement wrap) {
    if (TYPE == type::string) {
        return (wrap.TYPE == type::string) && (wrap.STRING < STRING);
    } else if (TYPE == type::integer) {
        return (wrap.TYPE == type::integer) && (wrap.INTEGER < INTEGER);
    } else {
        return false;
    }
}

QueryElement::QueryElement(std::string s) {
TYPE = type::string;
this->STRING = s;
}

QueryElement::QueryElement(int i) {
    TYPE = type::integer;
    this->INTEGER = i;
}

 QueryElement QueryElement::INVALID() {
    QueryElement e = QueryElement("");
    e.invalid = true;
    return e;
}

 QueryElement QueryElement::WILDCARD() {
    QueryElement e = QueryElement("");
    e.wildcard = true;
    return e;
}

bool QueryElement::isInvalid() {
    return (invalid);
}

bool QueryElement::isWildCard() {
    return wildcard;
}

std::string QueryElement::to_string() const {
    if (TYPE == QueryElement::type::integer) {
        return ::to_string(INTEGER);
    } else if (TYPE == QueryElement::type::string) {
        return STRING;
    } else {
        return "ERROR";
    }
}