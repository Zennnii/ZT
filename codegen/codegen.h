#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include <stdlib.h>
#include "../parser/parser.h"
#include "../zt.h"
#include "../utils/utils.h"

void generate_code(StatementType stmt_type, char *line);

#endif