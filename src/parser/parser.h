#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct SParser Parser;

Parser *parserCreate(Lexer *lexer);
Ast *parseProgram(Parser *parser);
const char *parserGetError(Parser *parser);
void parserCleanup(Parser **parser);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _PARSER_H_
