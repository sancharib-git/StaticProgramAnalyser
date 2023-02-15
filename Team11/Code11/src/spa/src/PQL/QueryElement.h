#ifndef AUTOTESTER_QUERYELEMENT_H
#define AUTOTESTER_QUERYELEMENT_H

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;
class QueryElement {
public:
    QueryElement();

    enum type {string, integer};
    std::string STRING;
    int INTEGER;
    type TYPE;
    bool invalid = false;
    bool wildcard = false;

    bool operator==(const QueryElement& wrap) const;

    // TODO: test this!!
    bool operator<(const QueryElement& wrap) const;

    bool lessThan(QueryElement wrap);

    QueryElement(std::string s);

    QueryElement(int i);

    static QueryElement INVALID();

    static QueryElement WILDCARD();

    bool isInvalid();

    bool isWildCard();

    std::string to_string() const;

};

namespace std
{
    template <>
    struct hash<QueryElement>
    {
        size_t operator()( const QueryElement& wrap ) const
        {
            if (wrap.TYPE == QueryElement::type::integer) {
                return hash<int>()(wrap.INTEGER);
            } else {
                return hash<string>()(wrap.STRING);
            }
        }
    };
};
#endif //AUTOTESTER_QUERYELEMENT_H
