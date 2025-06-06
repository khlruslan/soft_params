include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(googletest)

# Collect  C++ source files recursively
file(GLOB_RECURSE CXX_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cxx")
add_executable(unit_tests ${CXX_FILES})
set_target_properties( unit_tests
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)
# Link GoogleTest libraries
target_link_libraries(unit_tests
    PRIVATE
    gtest_main
    FormatUtils
    SoftParams
    Tabulator
)
# Include directories (including where GoogleTest is built)
target_include_directories(unit_tests PRIVATE ${gtest_SOURCE_DIR}/include)
# Add include directories for tests to find headers
target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)



target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/lib)


# Enable testing and discover tests
# Discover and run tests
include(GoogleTest)
gtest_discover_tests(unit_tests)

