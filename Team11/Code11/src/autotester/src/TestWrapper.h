
#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#pragma once
#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "TestWrapper.h"
#include "Parser/Parser.h"
#include "Parser/Tokeniser.h"
#include "Parser/AssignmentParser.h"
#include "PKB/PKB.h"
#include "DE/DesignExtractor.h"
#include "DE/Graph.h"
#include "Hasher.h"
#include "DE/LinkedList.h"
#include "Parser/Tokeniser.h"
#include "PKB/Table.h"
#include "PQL/QueryEvaluator.h"
#include "QueryResultProjector.h"

class TestWrapper : public AbstractWrapper {
private:
    Parser* _parser;
    PKB* _sharedPkb;
    DesignExtractor* _de;
    QueryEvaluator* _qe;
    QueryResultProjector* _resultProjector;

 public:
  // default constructor
  TestWrapper();
  
  // destructor
  ~TestWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif
