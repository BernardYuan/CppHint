add_executable(FuncSign
    function_signature.cc
)

target_link_libraries(FuncSign ${LibClang_LIBRARIES})

set_target_properties(FuncSign
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      PREFIX ""
)

install(TARGETS FuncSign
  RUNTIME DESTINATION bin
)

