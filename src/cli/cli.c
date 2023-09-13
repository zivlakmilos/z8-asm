#include <argp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"

#include "../ast/ast.h"
#include "../compiler/compiler.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"

struct SCli {
  char *inFilename;
  char *outFilename;

  uint8_t hasError;

  char *input;
  uint16_t *output;
  size_t outputLen;
};

void _cliReadFile(Cli *cli);
void _cliWriteFile(Cli *cli);

const char *_argpProgramVersion = "z8asm 1.0.0";
const char *_argpProgramBugAddress = "<zi@zivlak.rs>";
static char _argpDoc[] = "Assembly for Z8 processor";
static char _argpArgsDoc[] = "FILENAME";
static struct argp_option _argpOptions[] = {
    {"out", 'o', "FILENAME", 0, "Compare lines instead of characters."},
    {0},
};

static error_t _argpParseOpt(int key, char *arg, struct argp_state *state) {
  Cli *cli = (Cli *)state->input;

  switch (key) {
  case 'o':
    cli->outFilename = strdup(arg);
    break;

  case ARGP_KEY_ARG:
    if (state->arg_num >= 1) {
      argp_usage(state);
      cli->hasError = 1;
      return ARGP_ERR_UNKNOWN;
    }
    cli->inFilename = strdup(arg);
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1) {
      argp_usage(state);
      cli->hasError = 1;
      return ARGP_ERR_UNKNOWN;
    }
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = {_argpOptions, _argpParseOpt, _argpArgsDoc, _argpDoc};

/******************************************************************************
                              PUBLIC FUNCTIONS                                *
*******************************************************************************/

Cli *cliCreate(int argc, char **argv) {
  size_t len = sizeof(Cli);
  Cli *cli = malloc(len);
  memset(cli, 0, len);

  argp_parse(&argp, argc, argv, 0, 0, cli);

  if (!cli->outFilename) {
    cli->outFilename = strdup("out.hex");
  }

  return cli;
}

void cliExec(Cli *cli) {
  if (cli->hasError) {
    return;
  }

  _cliReadFile(cli);
  if (!cli->input) {
    return;
  }

  Lexer *lexer = lexerCreate(cli->input);
  Parser *parser = parserCreate(lexer);
  Compiler *compiler = compilerCreate();

  Ast *ast = parseProgram(parser);
  if (ast) {
    cli->output = compileProgram(compiler, ast, &cli->outputLen);
    _cliWriteFile(cli);
  } else {
    printf("%s\n", parserGetError(parser));
  }

  compilerCleanup(&compiler);
  parserCleanup(&parser);
  astCleanup(&ast);
}

void cliCleanup(Cli **cli) {
  if (!cli) {
    return;
  }

  if ((*cli)->outFilename) {
    free((*cli)->outFilename);
  }
  if ((*cli)->inFilename) {
    free((*cli)->inFilename);
  }

  free(*cli);
  *cli = NULL;
}

/******************************************************************************
                              PRIVATE FUNCTIONS                               *
*******************************************************************************/

void _cliReadFile(Cli *cli) {
  FILE *f = fopen(cli->inFilename, "r");

  if (!f) {
    return;
  }

  const size_t chunk = 50;
  size_t len = chunk;
  size_t bufLen = 0;
  char *buffer = malloc(len * sizeof(char));
  char line[chunk];

  while (fgets(line, chunk, f)) {
    bufLen += strlen(line);
    while (bufLen >= len) {
      len += chunk;
      buffer = realloc(buffer, len);
    }

    strcat(buffer, line);
  }

  cli->input = strdup(buffer);

  fclose(f);
}

void _cliWriteFile(Cli *cli) {
  FILE *f = fopen(cli->outFilename, "wb");

  if (!f) {
    return;
  }

  fwrite(cli->output, sizeof(uint16_t), cli->outputLen, f);

  fclose(f);
}
