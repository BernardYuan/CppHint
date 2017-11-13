configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake" 
               "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(CppHinter
  main.cpp
)

llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES ${LLVM_TARGETS_TO_BUILD}
        asmparser core linker bitreader bitwriter irreader ipo scalaropts
        analysis target mc support
)

target_link_libraries(CppHinter CppHinter-inst ${REQ_LLVM_LIBRARIES})

# Platform dependencies.
if( WIN32 )
  find_library(SHLWAPI_LIBRARY shlwapi)
  target_link_libraries(callgraph-profiler
    ${SHLWAPI_LIBRARY}
  )
else()
  find_package(Threads REQUIRED)
  find_package(Curses REQUIRED)
  target_link_libraries(CppHinter
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
    ${CURSES_LIBRARIES}
  )
endif()

add_dependencies(CppHinter CppHinter-rt)
set_target_properties(CppHinter
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      PREFIX ""
)

install(TARGETS CppHinter
  RUNTIME DESTINATION bin
)

