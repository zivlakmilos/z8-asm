#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

typedef struct SStatement {
  Token *instruction;
  Token *arg;

  struct SStatement *next;
  struct SStatement *prev;
} Statement;

struct SAst {
  Statement *head;
  Statement *tail;
};

struct SStatementIter {
  Ast *ast;
  Statement *cur;
  uint8_t isEnd;
};

Statement *_statementCreate(Token *instruction, Token *arg);
void _statementCleanup(Statement **statement);

/******************************************************************************
                              PUBLIC FUNCTIONS                                *
*******************************************************************************/

Ast *astCreate(void) {
  size_t len = sizeof(Ast);
  Ast *ast = malloc(len);
  memset(ast, 0, len);

  return ast;
}

void astAddStatement(Ast *ast, Token *instruction, Token *arg) {
  size_t len = sizeof(Statement);
  Statement *statement = malloc(len);
  memset(statement, 0, len);

  statement->instruction = instruction;
  statement->arg = arg;

  statement->prev = NULL;
  statement->next = NULL;

  if (!ast->head) {
    ast->head = statement;
    ast->tail = statement;

    return;
  }

  ast->tail->next = statement;
  statement->prev = ast->tail;
  ast->tail = statement;
}

StatementIter *astCreateStatementIter(Ast *ast) {
  size_t len = sizeof(StatementIter);
  StatementIter *iter = malloc(len);
  memset(iter, 0, len);

  iter->ast = ast;
  iter->cur = NULL;
  iter->isEnd = 0;

  return iter;
}

void astCleanup(Ast **ast) {
  if (!ast) {
    return;
  }

  Statement *stmt = (*ast)->head;
  while (stmt) {
    Statement *cur = stmt;
    stmt = stmt->next;

    _statementCleanup(&cur);
  }

  free(*ast);
  *ast = NULL;
}

uint8_t statementIterNext(StatementIter *iter) {
  if (iter->isEnd) {
    return 0;
  }

  if (!iter->cur) {
    iter->cur = iter->ast->head;
  } else {
    iter->cur = iter->cur->next;
  }

  if (iter->cur) {
    return 1;
  }

  iter->isEnd = 1;

  return 0;
}

Token *statementIterGetInstruction(StatementIter *iter) {
  if (!iter->cur) {
    return NULL;
  }

  return iter->cur->instruction;
}

Token *statementIterGetArg(StatementIter *iter) {
  if (!iter->cur) {
    return NULL;
  }

  return iter->cur->arg;
}

void statementIterCleanup(StatementIter **iter) {
  if (!iter) {
    return;
  }

  free(*iter);
  *iter = NULL;
}

/******************************************************************************
                              PRIVATE FUNCTIONS                               *
*******************************************************************************/

Statement *_statementCreate(Token *instruction, Token *arg) {
  size_t len = sizeof(Statement);
  Statement *statement = malloc(len);
  memset(statement, 0, len);

  statement->instruction = instruction;
  statement->arg = arg;

  return statement;
}

void _statementCleanup(Statement **statement) {
  if (!statement) {
    return;
  }

  tokenCleanup(&(*statement)->instruction);
  tokenCleanup(&(*statement)->instruction);

  free(*statement);
  *statement = NULL;
}
