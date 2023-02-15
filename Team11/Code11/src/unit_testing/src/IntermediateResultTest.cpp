#include "PQL/IntermediateResult.h"
#include "PQL/DoubleColumnResult.h"
#include "SetProcessor.h"
#include "PQL/CrossProductStruct.h"
#include "catch.hpp"
#include "PQL/QueryElement.h"
#include <unordered_set>

using namespace std;

typedef vector<pair<QueryElement, QueryElement>> qeList;
qeList s3v1 = {make_pair(QueryElement(5), QueryElement("y")),
               make_pair(QueryElement(5), QueryElement("z")),
               make_pair(QueryElement(7), QueryElement("x")),};
qeList s1s2 = {make_pair(QueryElement(6), QueryElement(20)),
               make_pair(QueryElement(3), QueryElement(4)),
               make_pair(QueryElement(10), QueryElement(15)),
               make_pair(QueryElement(8), QueryElement(11)),};
qeList s3s1 = {make_pair(QueryElement(5), QueryElement(6)),
               make_pair(QueryElement(5), QueryElement(8)),
               make_pair(QueryElement(2), QueryElement(7)),
               make_pair(QueryElement(2), QueryElement(3)),
               make_pair(QueryElement(7), QueryElement(6)),};
qeList s2v1 = {make_pair(QueryElement(11), QueryElement("y")),
               make_pair(QueryElement(20), QueryElement("x")),
               make_pair(QueryElement(4), QueryElement("x")),
               make_pair(QueryElement(20), QueryElement("y")),
               make_pair(QueryElement(11), QueryElement("z")),
               make_pair(QueryElement(10), QueryElement("z")),};

TEST_CASE("testIntermediateResultEraseWithCascade") {
    IntermediateResult* test = new IntermediateResult();
    DoubleColumnResult* set1 = new DoubleColumnResult("s3", "v1");
    DoubleColumnResult* set2 = new DoubleColumnResult("s1", "s2");
    DoubleColumnResult* set3 = new DoubleColumnResult("s3", "s1");
    DoubleColumnResult* set4 = new DoubleColumnResult("s3", "v1");
    DoubleColumnResult* set5 = new DoubleColumnResult("outsider1", "outsider2");
    *set1 = DoubleColumnResult("s3", "v1", s3v1);
    *set2 = DoubleColumnResult("s1", "s2", s1s2);
    *set3 = DoubleColumnResult("s3", "s1", s3s1);
    *set4 = DoubleColumnResult("s2", "v1", s2v1);
    *set5 = DoubleColumnResult("outsider1", "outsider2", {make_pair(QueryElement(1), QueryElement(2))});

    test->addOrReplaceSingleColumn("s3", {QueryElement(2), QueryElement(5), QueryElement(7), QueryElement(1000)});
    test->addOrReplaceSingleColumn("s2", {QueryElement(11), QueryElement(20), QueryElement(4), QueryElement(15), QueryElement(10)});
    test->addOrReplaceSingleColumn("s1", {QueryElement(6), QueryElement(3),QueryElement(8), QueryElement(10), QueryElement(7)});
    test->addOrReplaceSingleColumn("v1", {QueryElement("x"), QueryElement("y"), QueryElement("z")});
    test->addOrReplaceSingleColumn("outsider1", {QueryElement(1)});
    test->addOrReplaceSingleColumn("outsider2", {QueryElement(2)});
    test->addOrReplaceDoubleColumn(set1, false);
    test->addOrReplaceDoubleColumn(set2, false);
    test->addOrReplaceDoubleColumn(set3, false);
    test->addOrReplaceDoubleColumn(set4, false);
    test->addOrReplaceDoubleColumn(set5, false);
    test->reduceAllResults();
    REQUIRE(true);
}

TEST_CASE("testIntermediateResultReplaceDoubleColumn") {
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
    IntermediateResult test = IntermediateResult();
    DoubleColumnResult* ptr1 = new DoubleColumnResult("a", "b");
    *ptr1 = test1;
    DoubleColumnResult* ptr2 = new DoubleColumnResult("a", "b");
    *ptr2 = test2;
    test.addOrReplaceDoubleColumn(ptr1, false);
    test.addOrReplaceDoubleColumn(ptr2, false);
    REQUIRE(true);
}

TEST_CASE("testSetProcessorCrossProduct") {
    typedef vector<vector<QueryElement>> table_body;
    typedef CrossProductStruct<QueryElement> cps;
    cps s3v1 = cps(
            {"s3", "v1"},
            {{QueryElement(5), QueryElement("y")},
                 {QueryElement(5), QueryElement("z")},
                 {QueryElement(7), QueryElement("x")},}
    );
    cps s1s2 = cps(
            {"s1", "s2"},
            {{QueryElement(6), QueryElement(20)},
             {QueryElement(3), QueryElement(4)},
             {QueryElement(10), QueryElement(15)},
             {QueryElement(8), QueryElement(11)},}
            );
    cps s3s1 = cps(
            {"s3", "s1"},
            {{QueryElement(5), QueryElement(6)},
             {QueryElement(5), QueryElement(8)},
             {QueryElement(2), QueryElement(7)},
             {QueryElement(2), QueryElement(3)},
             {QueryElement(7), QueryElement(6)},}
            );
    cps s2v1 = cps(
            {"s2", "v1"},
            {{QueryElement(11), QueryElement("y")},
             {QueryElement(20), QueryElement("x")},
             {QueryElement(4), QueryElement("x")},
             {QueryElement(20), QueryElement("y")},
             {QueryElement(11), QueryElement("z")},
             {QueryElement(10), QueryElement("z")},}
            );
    cps first = SetProcessor::crossProduct<QueryElement>(s3v1, s1s2);
    cps second = SetProcessor::crossProduct<QueryElement>(first, s2v1);
    cps third = SetProcessor::crossProduct<QueryElement>(second, s3s1);
    cps fourth = SetProcessor::crossProduct<QueryElement>({s3v1, s1s2, s3s1, s2v1});
    REQUIRE(true);
}

TEST_CASE("testIntermediateResultCrossProductWithinCluster") {
    IntermediateResult* test = new IntermediateResult();
    DoubleColumnResult* set1 = new DoubleColumnResult("s3", "v1");
    DoubleColumnResult* set2 = new DoubleColumnResult("s1", "s2");
    DoubleColumnResult* set3 = new DoubleColumnResult("s3", "s1");
    DoubleColumnResult* set4 = new DoubleColumnResult("s3", "v1");
    DoubleColumnResult* set5 = new DoubleColumnResult("outsider1", "outsider2");
    *set1 = DoubleColumnResult("s3", "v1", s3v1);
    *set2 = DoubleColumnResult("s1", "s2", s1s2);
    *set3 = DoubleColumnResult("s3", "s1", s3s1);
    *set4 = DoubleColumnResult("s2", "v1", s2v1);
    *set5 = DoubleColumnResult("outsider1", "outsider2", {make_pair(QueryElement(1), QueryElement(2))});

    test->addOrReplaceSingleColumn("s3", {QueryElement(6), QueryElement(5), QueryElement(7), QueryElement(1000)});
    test->addOrReplaceSingleColumn("s2", {QueryElement(11), QueryElement(20), QueryElement(4), QueryElement(15), QueryElement(10)});
    test->addOrReplaceSingleColumn("s1", {QueryElement(6), QueryElement(3),QueryElement(8), QueryElement(10), QueryElement(7)});
    test->addOrReplaceSingleColumn("v1", {QueryElement("x"), QueryElement("y"), QueryElement("z")});
    test->addOrReplaceSingleColumn("outsider1", {QueryElement(1)});
    test->addOrReplaceSingleColumn("outsider2", {QueryElement(2)});
    test->addOrReplaceDoubleColumn(set1, false);
    test->addOrReplaceDoubleColumn(set2, false);
    test->addOrReplaceDoubleColumn(set3, false);
    test->addOrReplaceDoubleColumn(set4, false);
    test->addOrReplaceDoubleColumn(set5, false);
    test->reduceAllResults();
    REQUIRE(true);
}

TEST_CASE("testIntermediateResultGetResultTuple") {
    IntermediateResult* test = new IntermediateResult();
    DoubleColumnResult* set1 = new DoubleColumnResult("s3", "v1");
    DoubleColumnResult* set2 = new DoubleColumnResult("s1", "s2");
    DoubleColumnResult* set3 = new DoubleColumnResult("s3", "s1");
    DoubleColumnResult* set4 = new DoubleColumnResult("s3", "v1");
    DoubleColumnResult* set5 = new DoubleColumnResult("outsider1", "outsider2");
    *set1 = DoubleColumnResult("s3", "v1", s3v1);
    *set2 = DoubleColumnResult("s1", "s2", s1s2);
    *set3 = DoubleColumnResult("s3", "s1", s3s1);
    *set4 = DoubleColumnResult("s2", "v1", s2v1);
    *set5 = DoubleColumnResult("outsider1", "outsider2", {make_pair(QueryElement(1), QueryElement(2))});

    test->addOrReplaceSingleColumn("s3", {QueryElement(2), QueryElement(5), QueryElement(7), QueryElement(1000)});
    test->addOrReplaceSingleColumn("s2", {QueryElement(11), QueryElement(20), QueryElement(4), QueryElement(15), QueryElement(10)});
    test->addOrReplaceSingleColumn("s1", {QueryElement(6), QueryElement(3),QueryElement(8), QueryElement(10), QueryElement(7)});
    test->addOrReplaceSingleColumn("v1", {QueryElement("x"), QueryElement("y"), QueryElement("z")});
    test->addOrReplaceSingleColumn("outsider1", {QueryElement(1)});
    test->addOrReplaceSingleColumn("outsider2", {QueryElement(2)});
    test->addOrReplaceDoubleColumn(set1, false);
    test->addOrReplaceDoubleColumn(set2, false);
    test->addOrReplaceDoubleColumn(set3, false);
    test->addOrReplaceDoubleColumn(set4, false);
    test->addOrReplaceDoubleColumn(set5, false);
    test->reduceAllResults();
    REQUIRE(true);
}

TEST_CASE("testCompareRow") {
    vector<vector<QueryElement>> test = {
            {QueryElement(3), QueryElement(2), QueryElement(2)},
            {QueryElement(1), QueryElement(1), QueryElement(1)},
            {QueryElement(4), QueryElement(6), QueryElement(7)},
            {QueryElement(1), QueryElement(1), QueryElement(3)},
    };
    REQUIRE(true);
}



