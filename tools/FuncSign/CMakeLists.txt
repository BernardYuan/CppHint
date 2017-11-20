#include_directories(${CMAKE_CURRENT_BINARY_DIR})

#find_package(LibClang REQUIRED)

add_executable(FuncSign
    function_signature.cc
)

llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES ${LLVM_TARGETS_TO_BUILD}
        asmparser core linker bitreader bitwriter irreader ipo scalaropts
        analysis target mc support
)

target_link_libraries(FuncSign ${REQ_LLVM_LIBRARIES} clang)

# Platform dependencies.
if( WIN32 )
  find_library(SHLWAPI_LIBRARY shlwapi)
  target_link_libraries(callgraph-profiler
    ${SHLWAPI_LIBRARY}
  )
else()
  find_package(Threads REQUIRED)
  find_package(Curses REQUIRED)
  target_link_libraries(FuncSign
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${CURSES_LIBRARIES}
  )
endif()

#add_dependencies(FuncSign)
set_target_properties(FuncSign
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      PREFIX ""
)

install(TARGETS FuncSign
  RUNTIME DESTINATION bin
)

