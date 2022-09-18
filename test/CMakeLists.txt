cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

find_package(googletest REQUIRED)
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
  cxx_core_util::cxx_core_util
  googletest::cxx-googletest
  Threads::Threads)

# If the cxx_core_cc library is available, build the parallel
# map-reduce tests. Otherwise, skip the parallel tests.
#
find_package(cxx_core_cc QUIET)

if(${cxx_core_cc_FOUND} EQUAL 1)
  message("-- cxx-core-fp: cxx_core_cc is available, building parallel map-reduce tests")
  list(APPEND TESTS fp/mr/executor)
  list(APPEND LIBRARIES cxx_core_cc::cxx_core_cc)
else()
  message("-- cxx-core-fp: cxx_core_cc is not available, skipping parallel map-reduce tests")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configure_tests.cmake)
configure_tests("core" "${LIBRARIES}" ${TESTS})
