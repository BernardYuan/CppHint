add_executable(FindVariables
  FindVariables.cpp
)

target_link_libraries(FindVariables ${LibClangTooling_LIBRARIES})

set_target_properties(FindVariables
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      PREFIX ""
)

install(TARGETS FindVariables
  RUNTIME DESTINATION bin
)

