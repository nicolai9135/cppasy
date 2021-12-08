add_executable(cli 
    cli.cpp
    argument_parser.cpp
    ../logic/polytope.cpp
    ../logic/orthotope.cpp
    ../logic/parameter_synthesis.cpp
    ../parse/smtlib_parse.cpp
    ../parse/parse_boundaries.cpp
    ../evaluation/evaluation.cpp
)

target_link_libraries(cli ${Boost_LIBRARIES})
target_link_libraries(cli ${Z3_LIBRARIES})
target_link_libraries(cli ${wxWidgets_LIBRARIES})

set_property(TARGET cli PROPERTY CXX_STANDARD 20)

target_include_directories(cli PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include/cli"
    "${PROJECT_SOURCE_DIR}/include/logic"
    "${PROJECT_SOURCE_DIR}/include/parse"
    "${PROJECT_SOURCE_DIR}/include/evaluation"
)