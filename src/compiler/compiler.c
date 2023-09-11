#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"

struct SCompiler {
  uint16_t *buffer;
  size_t bufferAlocLen;
  size_t bufferLen;
};

static const uint16_t _instructionTable[] = {
    0b0000000000000000, // TokenTypeIllegal (ignore, not instruction)
    0b0000000000000000, // TokenTypeEof (ignore, not instruction)
    0b0000000000000000, // TokenTypeInt (ignore, not instruction)
    0b0000000000000000, // TokenTypeHex (ignore, not instruction)
    0b0000000000000000, // TokenTypeBin (ignore, not instruction)
    0b0000000000000000, // TokenTypeNop
    0b0000010000000000, // TokenTypeLdA
    0b0000010100000000, // TokenTypeLdB
    0b0000100000000000, // TokenTypeStA
    0b0000100100000000, // TokenTypeStB
    0b0000110000000000, // TokenTypeSetA
    0b0000110100000000, // TokenTypeSetB
    0b0001000100000000, // TokenTypeNot
    0b0001010000000000, // TokenTypeAnd
    0b0001100000000000, // TokenTypeOr
    0b0001110000000000, // TokenTypeXor
    0b0010000000000000, // TokenTypeNand
    0b0010010000000000, // TokenTypeNor
    0b0010110000000000, // TokenTypeAdd
    0b0011000000000000, // TokenTypeSub
    0b1000010000000000, // TokenTypeJmp
    0b1000100000000000, // TokenTypeJmpZ
    0b1000110000000000, // TokenTypeJmpC
    0b1111110000000000, // TokenTypeHlt
};

uint16_t _compileInstruction(Token *instruction, Token *arg);

/******************************************************************************
                              PUBLIC FUNCTIONS                                *
*******************************************************************************/

Compiler *compilerCreate(void) {
  size_t len = sizeof(Compiler);
  Compiler *compiler = malloc(len);
  memset(compiler, 0, len);

  return compiler;
}

uint16_t *compileProgram(Compiler *compiler, Ast *ast, size_t *len) {
  compiler->bufferAlocLen = astGetLength(ast) * sizeof(uint16_t);
  compiler->buffer = malloc(sizeof(compiler->bufferAlocLen));

  StatementIter *iter = astCreateStatementIter(ast);
  while (statementIterNext(iter) &&
         compiler->bufferLen < compiler->bufferAlocLen) {
    Token *instruction = statementIterGetInstruction(iter);
    Token *arg = statementIterGetArg(iter);

    uint16_t inst = _compileInstruction(instruction, arg);
    compiler->buffer[compiler->bufferLen] = inst;
    compiler->bufferLen++;
  }

  if (len) {
    *len = compiler->bufferLen;
  }
  return compiler->buffer;
}

void compilerCleanup(Compiler **parser) {
  if (!parser) {
    return;
  }

  free((*parser)->buffer);

  free(*parser);

  *parser = NULL;
}

/******************************************************************************
                              PRIVATE FUNCTIONS                               *
*******************************************************************************/

#include <stdio.h>
uint16_t _compileInstruction(Token *instruction, Token *arg) {
  uint16_t inst = _instructionTable[instruction->type];

  if (arg) {
    uint8_t val = 0;

    if (arg->type == TokenTypeHex) {
      val = strtol(arg->literal + 2, NULL, 16);
    } else if (arg->type == TokenTypeBin) {
      val = strtol(arg->literal + 2, NULL, 2);
    } else {
      val = strtol(arg->literal, NULL, 10);
    }

    inst |= val;
  }

  return inst;
}
