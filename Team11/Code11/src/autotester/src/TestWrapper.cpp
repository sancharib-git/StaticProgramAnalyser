#pragma once
#include <iostream>
#include "TestWrapper.h"
#include "Parser/Parser.h"
#include "PKB/PKB.h"
#include "PQL/QueryPreprocessor.h"
#include "ParserException.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
	_parser = new Parser();
	_sharedPkb = new PKB();
    _de = new DesignExtractor();
    _qe = new QueryEvaluator();
    _resultProjector = new QueryResultProjector();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
    std::string fileString = _parser->fileToString(filename);
    try
    {
        _parser->Parse(fileString);
        _de->populateAssignmentSubExpressions();
        _de->populateDesignAbstractions();
    } catch (exception& e)
    {
        cout << "Exception: " << e.what() << endl;
        std::exit(1);
    }

}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){

    _qe->reset();
    QueryPreprocessor::parse(query);
    
    _qe->getInstance().evaluate();
    list<string>* queryResult = _resultProjector->getInstance().getProcessedResult();
    results = *queryResult;
    _resultProjector->getInstance().resetResultProjector();

    _sharedPkb->clearCache();
}
