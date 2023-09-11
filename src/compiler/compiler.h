#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <stdint.h>

#include "../ast/ast.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct SCompiler Compiler;

Compiler *compilerCreate(void);
uint16_t *compileProgram(Compiler *compiler, Ast *ast, size_t *len);
void compilerCleanup(Compiler **parser);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _COMPILER_H_
