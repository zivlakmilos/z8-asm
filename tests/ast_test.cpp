#include "ast/ast.h"
#include <gtest/gtest.h>

TEST(TestAst, TestAst_Complete) {
  Token *tests[9];
  int testLength = 9;
  int i;

  Ast *ast = astCreate();

  tests[0] = tokenCreate(TokenTypeLdA, strdup("lda"));
  tests[1] = tokenCreate(TokenTypeHex, strdup("0x01"));
  tests[2] = tokenCreate(TokenTypeLdB, strdup("ldb"));
  tests[3] = tokenCreate(TokenTypeBin, strdup("0b00000010"));
  tests[4] = tokenCreate(TokenTypeAdd, strdup("add"));
  tests[5] = tokenCreate(TokenTypeNop, strdup("nop"));
  tests[6] = tokenCreate(TokenTypeStA, strdup("sta"));
  tests[7] = tokenCreate(TokenTypeInt, strdup("3"));
  tests[8] = tokenCreate(TokenTypeHlt, strdup("hlt"));

  for (i = 0; i < testLength; i++) {
    Token *token = tests[i];
    Token *arg = NULL;
    bool haveArg = token->type == TokenTypeLdA || token->type == TokenTypeLdB ||
                   token->type == TokenTypeStA;
    if (haveArg) {
      i++;
      arg = tests[i];
    }
    astAddStatement(ast, token, arg);
  }

  i = 0;
  StatementIter *iter = astCreateStatementIter(ast);
  while (statementIterNext(iter)) {
    Token *instruction = statementIterGetInstruction(iter);
    Token *arg = statementIterGetArg(iter);

    EXPECT_STREQ(instruction->literal, tests[i]->literal);
    ASSERT_EQ(instruction->type, tests[i]->type);
    i++;

    if (arg) {
      EXPECT_STREQ(arg->literal, tests[i]->literal);
      ASSERT_EQ(arg->type, tests[i]->type);
      i++;
    }
  }
  statementIterCleanup(&iter);

  astCleanup(&ast);
}
