include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(z8asm_test lexer_test.cpp ast_test.cpp parser_test.cpp)

target_link_libraries(z8asm_test
 PRIVATE
  GTest::GTest
  z8asm_lib)

add_test(NAME z8asm_test COMMAND z8asm_test)
