# =============================================================================
# ganon tests
# =============================================================================

add_executable( ganon-tests
    aux/Aux.hpp
    setup/Catch2.setup.cpp
    ganon-build/GanonBuild.test.cpp
    ganon-classify/GanonClassify.test.cpp
    utils/SafeQueue.test.cpp
    utils/LCA.test.cpp )

target_link_libraries( ganon-tests
    PRIVATE
    utils
    Catch2
    ganon-build-lib
    ganon-classify-lib )

target_include_directories( ganon-tests PRIVATE . )

target_compile_definitions( ganon-tests
    PRIVATE "GANON_TEST_FILES_PATH=${CMAKE_CURRENT_SOURCE_DIR}/")

add_test( NAME GanonTestSuite
    COMMAND $<TARGET_FILE:ganon-tests>
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data )
