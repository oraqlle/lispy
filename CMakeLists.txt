cmake_minimum_required(VERSION 3.15)

include(cmake/prelude.cmake)

project(
    lispy
    VERSION 0.3.1
    DESCRIPTION "Simple Lisp-like language"
    LANGUAGES C CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Dependencies ----

find_package(Threads)
find_package(replxx CONFIG REQUIRED)

# ---- Declare library ----

add_library(
    lispy_interpreter_lib OBJECT
    src/lib/builtin.c
    src/lib/io.c
    src/lib/lenv.c
    src/lib/lval.c
    src/lib/parser.c
    src/lib/utilities.c
)

target_include_directories(
    lispy_interpreter_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(lispy_interpreter_lib PRIVATE c_std_11)

# ---- Declare executable ----

find_package(replxx CONFIG REQUIRED)

add_executable(lispy_interpreter src/bin/lispy.c)
add_executable(lispy_interpreter::exe ALIAS lispy_interpreter)

set_property(TARGET lispy_interpreter PROPERTY OUTPUT_NAME lispy_interpreter)

target_compile_features(lispy_interpreter PRIVATE c_std_11)

target_link_libraries(lispy_interpreter PRIVATE lispy_interpreter_lib)
target_link_libraries(lispy_interpreter PRIVATE replxx::replxx)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT lispy_interpreter_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of the Lispy interpreter"
  )
endif()

include(cmake/dev-mode.cmake)
