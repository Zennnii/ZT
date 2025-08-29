#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include "../zt.h"
#include "../utils/utils.h"

typedef enum {
    STMT_LET,
    STMT_ASSIGNMENT,
    STMT_ECHO,
    STMT_INPUT,
    STMT_IF,
    STMT_ELSE_IF,
    STMT_ELSE,
    STMT_WHILE,
    STMT_CLOSING_BRACE,
    STMT_EXIT,
    STMT_UNKNOWN,
    STMT_EMPTY
} StatementType;

extern StatementType stmt_type;

StatementType parse_line(char *line);

#endif