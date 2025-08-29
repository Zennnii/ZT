#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void trim_newline(char *s);
void trim_whitespace(char *s);
void strip_outer_parentheses(char *cond);

#endif