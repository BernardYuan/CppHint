execute_process(
    COMMAND llvm-config --cxxflags
      OUTPUT_VARIABLE LLVM_DEFINITIONS
  )

execute_process(
    COMMAND llvm-config --includedir
      OUTPUT_VARIABLE LLVM_INCLUDE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
  )

execute_process(
    COMMAND llvm-config --libdir
      OUTPUT_VARIABLE LLVM_LIB_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
  )

execute_process(
    COMMAND llvm-config --version
      OUTPUT_VARIABLE LLVM_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
  )

execute_process(
    COMMAND llvm-config --system-libs
      OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
  )

execute_process(
    COMMAND llvm-config --libs
      OUTPUT_VARIABLE LLVM_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
  )

set(LLVM_LIBRARIES ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})
set(LLVM_INCLUDE_DIRS ${LLVM_INCLUDE_DIR})
set(LLVM_DEFINITIONS ${LLVM_DEFINITIONS} "-fno-rtti")

include(FindPackageHandleStandardArgs)
link_directories(${LLVM_LIB_DIR})
find_package_handle_standard_args(LLVM DEFAULT_MSG LLVM_LIBRARIES LLVM_INCLUDE_DIRS LLVM_LIB_DIR)
