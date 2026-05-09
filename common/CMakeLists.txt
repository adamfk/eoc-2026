# Collect all C sources and Headers.
# CONFIGURE_DEPENDS: CMake will add logic to the main build system check target to rerun the flagged GLOB commands at build time.
file(GLOB COMMON_SOURCES CONFIGURE_DEPENDS *.c *.h **/*.c)

add_library(common_logic STATIC ${COMMON_SOURCES})
target_include_directories(common_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ---------- build just for testing ----------
# There are better (more advanced) ways of doing this
add_library(common_logic_test STATIC ${COMMON_SOURCES})
target_compile_definitions(common_logic_test PUBLIC UNIT_TESTING)
target_include_directories(common_logic_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(common_logic_test PRIVATE -Werror -Wall -Wextra -fexceptions)
# Note: -fexceptions allows gcc to safely handle exceptions thrown through C code

# -- Code Coverage ---
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Add coverage flags to the common logic so we can see what's being hit
target_compile_options(common_logic_test PUBLIC 
        --coverage 
        -fprofile-arcs 
        -ftest-coverage 
        -fno-inline           # <--- Add these to make line mapping
        -fno-inline-small-functions # <--- much more precise
        -fno-default-inline   # <--- for C++
        -O0                   # Ensure optimization is OFF
    )
    target_link_options(common_logic_test PUBLIC --coverage)
endif()
