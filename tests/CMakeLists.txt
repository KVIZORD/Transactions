cmake_minimum_required(VERSION 3.14)
project(TransactionsTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES ${TESTS_DIR}/*.cc)

enable_testing()
add_executable(${PROJECT_NAME} ${TEST_SOURCES} ${COMMON_MODEL_DIR}/student.cc)
target_link_libraries(${PROJECT_NAME} GTest::gtest_main ${TARGET_GRAPH_LIB})
target_include_directories(${PROJECT_NAME} PUBLIC ${RELATIVE_HEADER_DIR})
add_definitions(-DSAMPLES_DIR="${SAMPLES_DIR}")

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

# Report
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

add_executable(report ${TEST_SOURCES} ${COMMON_MODEL_DIR}/student.cc)
target_link_libraries(report GTest::gtest_main)
target_include_directories(report PUBLIC ${RELATIVE_HEADER_DIR})