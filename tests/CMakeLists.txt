project(
    lispy_tests
    LANGUAGES CXX
)

# ---- Dependencies ----

find_package(Threads)
find_package(replxx CONFIG REQUIRED)

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----
add_executable(lispy_tests src/lispy_test.cxx)

target_link_libraries(
    lispy_tests PRIVATE
    lispy_interpreter_lib
    Catch2::Catch2WithMain
)
target_compile_features(lispy_tests PRIVATE cxx_std_11)
target_link_libraries(lispy_tests PRIVATE replxx::replxx)

catch_discover_tests(lispy_tests)

# ---- End-of-file commands ----
add_folders(Test)
