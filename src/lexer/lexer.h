#ifndef _LEXER_H_
#define _LEXER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  TokenTypeIllegal,
  TokenTypeEof,
  TokenTypeInt,
  TokenTypeHex,
  TokenTypeBin,
  TokenTypeNop,
  TokenTypeLdA,
  TokenTypeLdB,
  TokenTypeStA,
  TokenTypeStB,
  TokenTypeSetA,
  TokenTypeSetB,
  TokenTypeAnd,
  TokenTypeOr,
  TokenTypeXor,
  TokenTypeNand,
  TokenTypeNor,
  TokenTypeAdd,
  TokenTypeSub,
  TokenTypeJmp,
  TokenTypeJmpZ,
  TokenTypeJmpC,
  TokenTypeHlt,
} TokenType;

typedef struct SToken {
  TokenType type;
  char *literal;
} Token;

typedef struct SLexer Lexer;
Lexer *lexerCreate(const char *input);

Token *lexerNext(Lexer *lexer);
void lexerCleanup(Lexer **lexer);

Token *tokenCreate(TokenType type, char *literal);
void tokenCleanup(Token **token);

#ifdef __cplusplus
}
#endif

#endif // _LEXER_H_
