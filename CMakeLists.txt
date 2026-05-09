cmake_minimum_required(VERSION 3.14)
project(ModularEmbeddedProject LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# --- Fetch GoogleTest ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Add Subdirectories ---
add_subdirectory(common)
add_subdirectory(simulation)

# --- Add Test Support ---
enable_testing()
add_subdirectory(tests)


