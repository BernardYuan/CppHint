add_executable(HintRefactor
  HintRefactor.cpp
)

target_link_libraries(HintRefactor ${LibClangTooling_LIBRARIES})

set_target_properties(HintRefactor
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      PREFIX ""
)

install(TARGETS HintRefactor
  RUNTIME DESTINATION bin
)

