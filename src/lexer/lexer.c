#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

struct SLexer {
  const char *input;
  size_t inputLength;
  size_t position;
  size_t readPosition;
  char ch;
};

static void _lexerReadChar(Lexer *lexer);
static char _lexerPeek(Lexer *lexer);
static void _lexerSkipWhitespace(Lexer *lexer);
static const char *_lexerReadInst(Lexer *lexer, size_t *len);
static const char *_lexerReadInt(Lexer *lexer, size_t *len);
static const char *_lexerReadHex(Lexer *lexer, size_t *len);
static const char *_lexerReadBin(Lexer *lexer, size_t *len);

static uint8_t _isLetter(char ch);
static uint8_t _isNumber(char ch);
static uint8_t _isBin(char ch);
static uint8_t _isHex(char ch);

static TokenType _getTokenTypeFromLiteral(const char *literal, size_t len);

/******************************************************************************
                              PUBLIC FUNCTIONS                                *
*******************************************************************************/

Lexer *lexerCreate(const char *input) {
  size_t len = sizeof(Lexer);
  Lexer *lexer = malloc(len);
  memset(lexer, 0, len);

  lexer->input = input;
  lexer->inputLength = strlen(input);
  lexer->position = 0;
  lexer->readPosition = 0;

  _lexerReadChar(lexer);

  return lexer;
}

Token *lexerNext(Lexer *lexer) {
  Token *tok = NULL;

  _lexerSkipWhitespace(lexer);

  if (lexer->ch == '\0') {
    tok = tokenCreate(TokenTypeEof, NULL);
  } else if (lexer->ch == '0' && _lexerPeek(lexer) == 'x') {
    size_t len = 0;
    char *literal = NULL;
    const char *val = _lexerReadHex(lexer, &len);

    literal = strndup(val, len);
    tok = tokenCreate(TokenTypeHex, literal);

    return tok;
  } else if (lexer->ch == '0' && _lexerPeek(lexer) == 'b') {
    size_t len = 0;
    char *literal = NULL;
    const char *val = _lexerReadBin(lexer, &len);

    literal = strndup(val, len);
    tok = tokenCreate(TokenTypeBin, literal);

    return tok;
  } else if (_isNumber(lexer->ch)) {
    size_t len = 0;
    char *literal = NULL;
    const char *val = _lexerReadInt(lexer, &len);

    literal = strndup(val, len);
    tok = tokenCreate(TokenTypeInt, literal);

    return tok;
  } else if (_isLetter(lexer->ch)) {
    size_t len = 0;
    char *literal = NULL;
    const char *val = _lexerReadInst(lexer, &len);

    literal = strndup(val, len);
    TokenType type = _getTokenTypeFromLiteral(literal, len);
    tok = tokenCreate(type, literal);

    return tok;
  }

  if (!tok) {
    tok = tokenCreate(TokenTypeIllegal, NULL);
  }

  _lexerReadChar(lexer);

  return tok;
}

void lexerCleanup(Lexer **lexer) {
  if (!lexer) {
    return;
  }

  if (*lexer) {
    free(*lexer);
  }

  *lexer = NULL;
}

Token *tokenCreate(TokenType type, char *literal) {
  size_t len = sizeof(Token);
  Token *token = malloc(len);
  memset(token, 0, len);

  token->literal = literal;
  token->type = type;

  return token;
}

void tokenCleanup(Token **token) {
  if (*token && (*token)->literal) {
    free((*token)->literal);
  }

  if (*token) {
    free(*token);
  }

  *token = NULL;
}

/******************************************************************************
                              PRIVATE FUNCTIONS                               *
*******************************************************************************/

static void _lexerReadChar(Lexer *lexer) {
  if (lexer->readPosition >= lexer->inputLength) {
    lexer->ch = '\0';
  } else {
    lexer->ch = lexer->input[lexer->readPosition];
  }

  lexer->position = lexer->readPosition;
  lexer->readPosition++;
}

static char _lexerPeek(Lexer *lexer) {
  if (lexer->readPosition >= lexer->inputLength) {
    return '\0';
  } else {
    return lexer->input[lexer->readPosition];
  }
}

static void _lexerSkipWhitespace(Lexer *lexer) {
  while (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\n' ||
         lexer->ch == '\r') {
    _lexerReadChar(lexer);
  }
}

static const char *_lexerReadInst(Lexer *lexer, size_t *len) {
  char *result = NULL;
  size_t position = lexer->position;

  while (_isLetter(lexer->ch)) {
    _lexerReadChar(lexer);
  }

  if (len) {
    *len = lexer->position - position;
  }

  return lexer->input + position;
}

static const char *_lexerReadInt(Lexer *lexer, size_t *len) {
  char *result = NULL;
  size_t position = lexer->position;

  while (_isNumber(lexer->ch)) {
    _lexerReadChar(lexer);
  }

  if (len) {
    *len = lexer->position - position;
  }

  return lexer->input + position;
}

static const char *_lexerReadHex(Lexer *lexer, size_t *len) {
  char *result = NULL;
  size_t position = lexer->position;

  while (_isHex(lexer->ch) ||
         (lexer->position - position == 1 && lexer->ch == 'x')) {
    _lexerReadChar(lexer);
  }

  if (len) {
    *len = lexer->position - position;
  }

  return lexer->input + position;
}

static const char *_lexerReadBin(Lexer *lexer, size_t *len) {
  char *result = NULL;
  size_t position = lexer->position;

  while (_isBin(lexer->ch) ||
         (lexer->position - position == 1 && lexer->ch == 'b')) {
    _lexerReadChar(lexer);
  }

  if (len) {
    *len = lexer->position - position;
  }

  return lexer->input + position;
}

static uint8_t _isLetter(char ch) {
  return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_';
}

static uint8_t _isNumber(char ch) { return '0' <= ch && '9' >= ch; }

static uint8_t _isBin(char ch) { return ch == '0' || ch == '1'; }

static uint8_t _isHex(char ch) {
  return '0' <= ch && ch <= '9' || 'a' <= ch && ch <= 'f' ||
         'A' <= ch && ch <= 'F';
}

static TokenType _getTokenTypeFromLiteral(const char *literal, size_t len) {
  if (strncmp(literal, "nop", len) == 0) {
    return TokenTypeNop;
  } else if (strncmp(literal, "lda", len) == 0) {
    return TokenTypeLdA;
  } else if (strncmp(literal, "ldb", len) == 0) {
    return TokenTypeLdB;
  } else if (strncmp(literal, "sta", len) == 0) {
    return TokenTypeStA;
  } else if (strncmp(literal, "stb", len) == 0) {
    return TokenTypeStB;
  } else if (strncmp(literal, "seta", len) == 0) {
    return TokenTypeSetA;
  } else if (strncmp(literal, "setb", len) == 0) {
    return TokenTypeSetB;
  } else if (strncmp(literal, "not", len) == 0) {
    return TokenTypeNot;
  } else if (strncmp(literal, "and", len) == 0) {
    return TokenTypeAnd;
  } else if (strncmp(literal, "or", len) == 0) {
    return TokenTypeOr;
  } else if (strncmp(literal, "xor", len) == 0) {
    return TokenTypeXor;
  } else if (strncmp(literal, "nand", len) == 0) {
    return TokenTypeNand;
  } else if (strncmp(literal, "nor", len) == 0) {
    return TokenTypeNor;
  } else if (strncmp(literal, "add", len) == 0) {
    return TokenTypeAdd;
  } else if (strncmp(literal, "sub", len) == 0) {
    return TokenTypeSub;
  } else if (strncmp(literal, "jmp", len) == 0) {
    return TokenTypeJmp;
  } else if (strncmp(literal, "jmpz", len) == 0) {
    return TokenTypeJmpZ;
  } else if (strncmp(literal, "jmpc", len) == 0) {
    return TokenTypeJmpC;
  } else if (strncmp(literal, "hlt", len) == 0) {
    return TokenTypeHlt;
  }

  return TokenTypeIllegal;
}
