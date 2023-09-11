#include "compiler/compiler.h"
#include "parser/parser.h"
#include <gtest/gtest.h>

TEST(TestCompiler, TestCompiler_Complete) {
  const char *input = "lda 0x01\n"
                      "ldb 0b00000010\n"
                      "add\n"
                      "nop\n"
                      "sta 3\n"
                      "hlt\n";
  Token *tests[9];
  int testLength = 9;
  int i;

  tests[0] = tokenCreate(TokenTypeLdA, strdup("lda"));
  tests[1] = tokenCreate(TokenTypeHex, strdup("0x01"));
  tests[2] = tokenCreate(TokenTypeLdB, strdup("ldb"));
  tests[3] = tokenCreate(TokenTypeBin, strdup("0b00000010"));
  tests[4] = tokenCreate(TokenTypeAdd, strdup("add"));
  tests[5] = tokenCreate(TokenTypeNop, strdup("nop"));
  tests[6] = tokenCreate(TokenTypeStA, strdup("sta"));
  tests[7] = tokenCreate(TokenTypeInt, strdup("3"));
  tests[8] = tokenCreate(TokenTypeHlt, strdup("hlt"));

  uint16_t expected[] = {
      0b0000010000000001, // TokenTypeLdA
      0b0000010100000010, // TokenTypeLdB
      0b0010110000000000, // TokenTypeAdd
      0b0000000000000000, // TokenTypeNop
      0b0000100000000011, // TokenTypeStA
      0b1111110000000000, // TokenTypeHlt
  };

  Lexer *lexer = lexerCreate(input);
  Parser *parser = parserCreate(lexer);
  Compiler *compiler = compilerCreate();

  Ast *ast = parseProgram(parser);
  size_t len = 0;
  uint16_t *buffer = compileProgram(compiler, ast, &len);

  for (i = 0; i < len; i++) {
    ASSERT_EQ(buffer[i], expected[i]);
  }

  for (i = 0; i < testLength; i++) {
    tokenCleanup(&tests[i]);
  }

  astCleanup(&ast);
  parserCleanup(&parser);
  compilerCleanup(&compiler);
}
