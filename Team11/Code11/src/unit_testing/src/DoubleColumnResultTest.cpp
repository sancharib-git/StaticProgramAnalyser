#include "PQL/DoubleColumnResult.h"
#include "catch.hpp"
#include "PQL/QueryElement.h"
#include <unordered_set>

using namespace std;
TEST_CASE("testDoubleColumnResultToString") {
    DoubleColumnResult test = DoubleColumnResult("a", "b");
    test.add("a", "b", QueryElement(1), QueryElement("heh"));
    test.add("a", "b", QueryElement(1), QueryElement("heh2"));
    test.add("a", "b", QueryElement(1), QueryElement("heh3"));
    test.add("a", "b", QueryElement(1), QueryElement("heh4"));
    test.add("a", "b", QueryElement(1), QueryElement("heh5"));
    test.add("b", "a", QueryElement(80), QueryElement("heh"));
    test.erase("a", QueryElement(1));
    REQUIRE(true);
}

TEST_CASE("testDoubleColumnResultIntersect") {
    DoubleColumnResult test1 = DoubleColumnResult("a", "b", {make_pair(QueryElement(6), QueryElement(4)),
                                                            make_pair(QueryElement(3), QueryElement(4)),
                                                             make_pair(QueryElement(7), QueryElement(4)),
                                                             make_pair(QueryElement(20), QueryElement(15)),
                                                             make_pair(QueryElement("random string1"), QueryElement(4)),
                                                            make_pair(QueryElement(10), QueryElement(11)),});
    DoubleColumnResult test2 = DoubleColumnResult("a", "b", {make_pair(QueryElement(6), QueryElement(4)),
                                                             make_pair(QueryElement(3), QueryElement(4)),
                                                             make_pair(QueryElement(20), QueryElement(15)),
                                                             make_pair(QueryElement(10), QueryElement(15)),
                                                             make_pair(QueryElement(3600), QueryElement("random string2")),
                                                             make_pair(QueryElement(11), QueryElement(11)),});
    REQUIRE(true);
}


