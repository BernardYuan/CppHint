cmake_minimum_required(VERSION 2.8)
project(CppHinter)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/cmake/modules"
  )


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

set(PACKAGE_NAME CppHinter)
set(PACKAGE_VERSION 0.1)
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

############## LLVM CONFIGURATION #################

# LLVM_DIR must be set to the prefix of /share/llvm/cmake via commandline


find_package(LibClang REQUIRED)
find_package(LibClangTooling REQUIRED)

include_directories(${LibClang_INCLUDE_DIRS})
include_directories(${LibClangTooling_INCLUDE_DIRS})

add_definitions(${LibClangTOoling_DEFINITIONS})

add_subdirectory(tools)

