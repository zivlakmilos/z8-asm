#ifndef _CLI_H_
#define _CLI_H_

#include <stdint.h>

typedef struct SCli Cli;

Cli *cliCreate(int argc, char **argv);
void cliExec(Cli *cli);
void cliCleanup(Cli **cli);

#endif // _CLI_H_
