find_package(LLVM REQUIRED)

execute_process(
    COMMAND llvm-config --cxxflags
    OUTPUT_VARIABLE LibClang_DEFINITIONS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

set(LibClang_DEFINITIONS ${LibClang_DEFINITIONS} "-fno-rtti")

execute_process(
    COMMAND llvm-config --includedir
    OUTPUT_VARIABLE LibClang_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

find_library(LibClang_LIBRARY NAMES clang PATHS ${LLVM_LIB_DIR})

set(LibClang_LIBRARIES ${LibClang_LIBRARY})
set(LibClang_INCLUDE_DIRS ${LibClang_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibClang DEFAULT_MSG LibClang_LIBRARY LibClang_INCLUDE_DIR)

mark_as_advanced(LibClang_INCLUDE_DIR LibClang_LIBRARY)
