cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

add_stream()
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

configure_tests("core" "${LIBRARIES}" ${TESTS})
