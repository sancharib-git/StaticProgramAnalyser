file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(autotester ${srcs} ../spa/src/Parser/Parser.cpp ../spa/src/Parser/Parser.h ../spa/src/Parser/WhileParser.cpp ../spa/src/Parser/WhileParser.h ../spa/src/Parser/Tokeniser.h ../spa/src/Parser/Tokeniser.cpp ../spa/src/PKB/PKB.h ../spa/src/PKB/PKB.cpp ../spa/src/DE/DesignExtractor.h ../spa/src/DE/DesignExtractor.cpp ../spa/src/DE/BinaryTree.cpp ../spa/src/DE/BinaryTree.h ../spa/src/DE/Graph.cpp ../spa/src/DE/Graph.h ../spa/src/DE/GraphNode.cpp ../spa/src/DE/GraphNode.h ../spa/src/DE/LinkedList.cpp ../spa/src/DE/LinkedList.h ../spa/src/Parser/ParserUtility.cpp ../spa/src/Parser/ParserUtility.h ../spa/src/Parser/BlockStmtParser.cpp ../spa/src/Parser/BlockStmtParser.h)
target_link_libraries(autotester spa ${AUTOTESTER_LIBRARIES})

if (NOT WIN32)
    target_link_libraries(autotester pthread)
endif()

