#ifndef _AST_H_
#define _AST_H_

#include <stddef.h>
#include <stdint.h>

#include "../lexer/lexer.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct SAst Ast;

typedef struct SStatementIter StatementIter;

typedef void (*forEachCallback)(Token *instruction, Token *arg);

Ast *astCreate(void);
void astAddStatement(Ast *ast, Token *instruction, Token *arg);
StatementIter *astCreateStatementIter(Ast *ast);
void astCleanup(Ast **ast);

uint8_t statementIterNext(StatementIter *iter);
Token *statementIterGetInstruction(StatementIter *iter);
Token *statementIterGetArg(StatementIter *iter);
void statementIterCleanup(StatementIter **iter);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _AST_H_
