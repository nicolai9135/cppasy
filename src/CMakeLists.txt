add_executable(cppasy 
    main.cpp
    argument_parser.cpp
    polytope.cpp
    orthotope.cpp
    parameter_synthesis.cpp
)

target_link_libraries(cppasy ${Boost_LIBRARIES})
target_link_libraries(cppasy ${Z3_LIBRARIES})

target_include_directories(cppasy PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
)