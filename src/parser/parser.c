#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#define _ERROR_BUFFER_LEN 100

struct SParser {
  Lexer *lexer;

  Token *currentToken;
  Token *nextToken;

  char error[_ERROR_BUFFER_LEN];
  uint8_t hasError;
};

void _parserNextToken(Parser *parser);

uint8_t _hasArgument(Token *token);
uint8_t _isArgument(Token *token);
uint8_t _isInstruction(Token *token);

/******************************************************************************
                              PUBLIC FUNCTIONS                                *
*******************************************************************************/

Parser *parserCreate(Lexer *lexer) {
  size_t len = sizeof(Parser);
  Parser *parser = malloc(len);
  memset(parser, 0, len);

  parser->lexer = lexer;

  _parserNextToken(parser);
  _parserNextToken(parser);

  return parser;
}

Ast *parseProgram(Parser *parser) {
  Ast *ast = astCreate();

  while (!parser->hasError && parser->currentToken) {
    if (parser->currentToken->type == TokenTypeEof) {
      break;
    }

    if (!_isInstruction(parser->currentToken)) {
      parser->hasError = 1;
      sprintf(parser->error, "Expected instruciton");
    }

    if (_hasArgument(parser->currentToken)) {
      if (parser->nextToken && _isArgument(parser->nextToken)) {
        astAddStatement(ast, parser->currentToken, parser->nextToken);
        _parserNextToken(parser);
      } else {
        parser->hasError = 1;
        sprintf(parser->error, "Expected argument for instruction %s",
                parser->currentToken->literal);
      }
    } else {
      astAddStatement(ast, parser->currentToken, NULL);
    }

    _parserNextToken(parser);
  }

  if (parser->hasError) {
    astCleanup(&ast);
    return NULL;
  }

  return ast;
}

const char *getError(Parser *parser) {
  if (!parser->hasError) {
    return NULL;
  }

  return parser->error;
}

void parserCleanup(Parser **parser) {
  if (!parser) {
    return;
  }

  lexerCleanup(&(*parser)->lexer);

  free(*parser);
  *parser = NULL;
}

/******************************************************************************
                              PRIVATE FUNCTIONS                               *
*******************************************************************************/

void _parserNextToken(Parser *parser) {
  parser->currentToken = parser->nextToken;
  parser->nextToken = lexerNext(parser->lexer);
}

uint8_t _hasArgument(Token *token) {
  switch (token->type) {
  case TokenTypeLdA:
  case TokenTypeLdB:
  case TokenTypeStA:
  case TokenTypeStB:
  case TokenTypeSetA:
  case TokenTypeSetB:
  case TokenTypeJmp:
  case TokenTypeJmpZ:
  case TokenTypeJmpC:
    return 1;
  default:
    return 0;
  }
}

uint8_t _isArgument(Token *token) {
  switch (token->type) {
  case TokenTypeInt:
  case TokenTypeHex:
  case TokenTypeBin:
    return 1;
  default:
    return 0;
  }
}

uint8_t _isInstruction(Token *token) {
  switch (token->type) {
  case TokenTypeNop:
  case TokenTypeLdA:
  case TokenTypeLdB:
  case TokenTypeStA:
  case TokenTypeStB:
  case TokenTypeSetA:
  case TokenTypeSetB:
  case TokenTypeNot:
  case TokenTypeAnd:
  case TokenTypeOr:
  case TokenTypeXor:
  case TokenTypeNand:
  case TokenTypeNor:
  case TokenTypeAdd:
  case TokenTypeSub:
  case TokenTypeJmp:
  case TokenTypeJmpZ:
  case TokenTypeJmpC:
  case TokenTypeHlt:
    return 1;
  default:
    return 0;
  }
}
