#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli/cli.h"

int main(int argc, char *argv[]) {
  Cli *cli = cliCreate(argc, argv);

  cliExec(cli);

  cliCleanup(&cli);

  return 0;
}
