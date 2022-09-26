cmake_minimum_required (VERSION 3.22 FATAL_ERROR)
project (fp VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET fp OR TARGET fp::fp)
  return()
endif()

# Configuration variables.
#
# Build the tests and set directories if this is the master project
#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(FP_TEST ON CACHE BOOL "Generate the tests.")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_DOCS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
  
  # Have cmake output the compilation commands and create a link to it
  # from the project root directory so the language server can find
  # it.
  #
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()

# Options for generating tests and documentation.
#
option(FP_TEST "Generate the tests." OFF)
option(FP_DOCS "Generate the docs." OFF)

# Output configuration
#
message("-- fp: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- fp: Included from: ${CMAKE_SOURCE_DIR}")
message("-- fp: FP_TEST ${FP_TEST}")
message("-- fp: FP_DOCS ${FP_DOCS}")
message("-- fp: CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")

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
include(GNUInstallDirs)
include(cmake/cpp_core_find_package.cmake)
cpp_core_find_package(mp)

# Build the library
#
add_library(fp INTERFACE)
add_library(fp::fp ALIAS fp)

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(fp INTERFACE FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

target_link_libraries(fp INTERFACE mp::mp)

# Optionally configure the tests
#
if(FP_TEST)
  enable_testing()
  add_subdirectory(test)
endif()

# Optionally configure the documentation
#
# if(FP_DOCS)
#   add_subdirectory(docs)
# endif()

# Installation
#
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/install_config.cmake)
install_config(fp)
