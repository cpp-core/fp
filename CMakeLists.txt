cmake_minimum_required (VERSION 3.24 FATAL_ERROR)
project (fp VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET fp OR TARGET fp::fp)
  return()
endif()

# If this is the current project.
#
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  # Laod the cmake recipes and utilities.
  #
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/load_cmake_helpers.cmake)

  # Options for generating tests and documentation
  #
  option(FP_TEST "Generate the tests." ON)
  option(FP_DOCS "Generate the docs." OFF)

  # compile_commands.json
  #
  symlink_compile_commands()

  # Convenience targets for git operations
  #
  make_submodule_pull_target()

else()
  option(FP_TEST "Generate the tests." OFF)
  option(FP_DOCS "Generate the docs." OFF)
endif()

# Put executables in the top-level binary directory
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Output configuration
#
message("-- fp: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- fp: Included from: ${CMAKE_SOURCE_DIR}")
message("-- fp: Install prefix: ${CMAKE_INSTALL_PREFIX}")
message("-- fp: test ${FP_TEST}")
message("-- fp: docs ${FP_DOCS}")

# Setup compilation before adding dependencies
#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)
add_compile_options(-stdlib=libc++)
add_compile_options(-O)
add_link_options(-stdlib=libc++)

# Add our dependencies
#
add_tuple()

# Build the library
#
add_library(fp INTERFACE)
add_library(fp::fp ALIAS fp)

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(fp INTERFACE FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

target_link_libraries(fp INTERFACE tuple::tuple)

foreach(prog fp0 fp1)
  add_executable(${prog} src/tools/${prog}.cpp)
  target_link_libraries(${prog} fp::fp)
endforeach()

# Optionally configure the tests
#
if(FP_TEST)
  add_gtest()
  add_subdirectory(test)
endif()

# Optionally configure the documentation
#
# if(FP_DOCS)
#   add_subdirectory(docs)
# endif()

# Installation
#
install_config(fp)
