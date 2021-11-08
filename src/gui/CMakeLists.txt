add_executable(gui 
    gui.cpp
    cppasy_app.cpp
    main_frame.cpp
    ../logic/polytope.cpp
    ../logic/orthotope.cpp
    ../logic/parameter_synthesis.cpp
)

target_link_libraries(gui ${Z3_LIBRARIES})
target_link_libraries(gui ${wxWidgets_LIBRARIES})

target_include_directories(gui PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include/logic"
    "${PROJECT_SOURCE_DIR}/include/gui"
)