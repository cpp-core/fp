cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

cpp_core_find_package(stream)
cpp_core_find_package(GTest
  URL https://github.com/cpp-core/googletest
  VERSION v1.12.0
  EXCLUDE_FROM_ALL)
find_package(Threads REQUIRED)

set(TESTS
  fp/either
  fp/fold
  fp/option
  fp/try
  fp/mr/syntax
  fp/mr/util
  )

set(LIBRARIES
  fp
  stream::stream
  GTest::gtest
  Threads::Threads)

# If the cc library is available, build the parallel
# map-reduce tests. Otherwise, skip the parallel tests.
#
find_package(cc QUIET)

if(${cc_FOUND} EQUAL 1)
  message("-- fp: cc is available, building parallel map-reduce tests")
  list(APPEND TESTS fp/mr/executor)
  list(APPEND LIBRARIES cc::cc)
else()
  message("-- fp: cc is not available, skipping parallel map-reduce tests")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configure_tests.cmake)
configure_tests("core" "${LIBRARIES}" ${TESTS})
