include(GoogleTest)

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(unit_tests ${TEST_SOURCES})

target_link_libraries(unit_tests 
    PRIVATE 
    GTest::gtest_main
    GTest::gmock
    common_logic_test
)

target_compile_definitions(unit_tests PRIVATE UNIT_TESTING)
target_compile_options(unit_tests PRIVATE -Werror -Wall -Wextra)

include(GoogleTest)
gtest_discover_tests(unit_tests)

add_custom_target(run_tests
    COMMAND $<TARGET_FILE:unit_tests> --gtest_color=yes
    DEPENDS unit_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building and running unit tests..."
    USES_TERMINAL
)

# code coverage
# Check if lcov is installed on the system
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)

# note: some improvements could be made here to reduce bogus warnings.
if(LCOV_PATH AND GENHTML_PATH)
    add_custom_target(coverage
        # 1. Zero out existing counters
        COMMAND ${LCOV_PATH} --directory . --zerocounters
        COMMAND ${CMAKE_COMMAND} -E echo "Cleaning old coverage data..."
        COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -delete

        # 2. Run the tests (must happen to generate data) 
        COMMAND ./unit_tests
        
        # 3. Capture coverage data
        COMMAND ${LCOV_PATH} --directory ${CMAKE_BINARY_DIR} --capture --output-file coverage.info --ignore-errors mismatch --rc branch_coverage=1

        # 4. Filter out system headers and GTest code (important!)
        COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' 'build/*' --output-file coverage.info.cleaned --ignore-errors mismatch,empty,unused
        
        # 5. Generate HTML
        COMMAND ${GENHTML_PATH} coverage.info.cleaned --output-directory coverage_report

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
        DEPENDS unit_tests
        COMMENT "Generating HTML code coverage report..."
        USES_TERMINAL
    )
endif()
