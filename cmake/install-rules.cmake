install(
    TARGETS lispy_interpreter
    RUNTIME COMPONENT lispy_interpreter_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
