#include "lexer/lexer.h"
#include <gtest/gtest.h>

TEST(TestLexer, TestLexer_Complete) {
  const char *input = "lda 0x01\n"
                      "ldb 0b00000010\n"
                      "add\n"
                      "nop\n"
                      "sta 3\n"
                      "hlt\n";
  Lexer *lexer = NULL;
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

  lexer = lexerCreate(input);

  for (i = 0; i < testLength; i++) {
    Token *token = lexerNext(lexer);
    EXPECT_STREQ(token->literal, tests[i]->literal);
    ASSERT_EQ(token->type, tests[i]->type);
    tokenCleanup(&token);

    tokenCleanup(&tests[i]);
  }

  lexerCleanup(&lexer);
}
