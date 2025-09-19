#include "codegen.h"
#include "../utils/utils.h"
#include <string.h>
#include <ctype.h>

extern FILE *out; // Reference to global output file


void generate_code(StatementType stmt_type, char *line) {
    trim_whitespace(line); // Remove CR/LF and spaces

    switch (stmt_type) {
        case STMT_LET: {
            char type[32], name[64], expr[128];
            int parts = sscanf(line, "let %31s %63s = %127[^\n]", type, name, expr);
            if (parts == 3) {
                fprintf(out, "%s %s = %s;\n", type, name, expr);
            } 
            else if (sscanf(line, "let %31s %63s", type, name) == 2) {
                fprintf(out, "%s %s;\n", type, name);
            }
            else {
                fprintf(stderr, "Syntax error in let: %s\n", line);
            }
            break;
        }

        case STMT_ASSIGNMENT: {
            fprintf(out, "%s;\n", line); // Add semicolon
            break;
        }

        case STMT_ECHO: {
            char expr[128];
            if (sscanf(line, "echo %127[^\n]", expr) == 1) {
                trim_whitespace(expr);
                if (expr[0] == '"' || expr[0] == '\'') {
                    fprintf(out, "printf(\"%%s\", %s);\n", expr);
                } else {
                    fprintf(out, "printf(\"%%d\", %s);\n", expr);
                }
            }
            break;
        }

        case STMT_INPUT: {
            char type[32], name[64];
            if (sscanf(line + 6, "%31s %63s", type, name) != 2) {
                fprintf(stderr, "Syntax error in input: %s\n", line);
                break;
            }

            fprintf(out, "fflush(stdout);\n"); // ensure prompt is visible

            if (strncmp(type, "int", 3) == 0) {
                fprintf(out, "scanf(\" %%d\", &%s);\n", name);
            } else if (strncmp(type, "float", 5) == 0) {
                fprintf(out, "scanf(\" %%f\", &%s);\n", name);
            } else if (strncmp(type, "double", 6) == 0) {
                fprintf(out, "scanf(\" %%lf\", &%s);\n", name);
            } else if (strncmp(type, "char", 4) == 0) {
                fprintf(out, "scanf(\" %%c\", &%s);\n", name); // leading space to skip newline
            } else {
                fprintf(out, "scanf(\" %%d\", &%s);\n", name); // default to int
            }
            break;
        }

        case STMT_IF:
        case STMT_ELSE_IF:
        case STMT_WHILE:
        case STMT_FOR: {
            char cond[128];
            char *brace = strchr(line, '{');
            int offset = 0;
            if (stmt_type == STMT_IF) offset = 3;
            else if (stmt_type == STMT_ELSE_IF) offset = 8;
            else if (stmt_type == STMT_WHILE) offset = 6;
            else if (stmt_type == STMT_FOR) offset = 6;

            if (brace) {
                size_t len = brace - (line + offset);
                strncpy(cond, line + offset, len);
                cond[len] = '\0';
            } else {
                strcpy(cond, line + offset);
            }
            strip_outer_parentheses(cond);

            if (stmt_type == STMT_IF) fprintf(out, "if (%s) {\n", cond);
            else if (stmt_type == STMT_ELSE_IF) fprintf(out, "else if (%s) {\n", cond);
            else if (stmt_type == STMT_WHILE) fprintf(out, "while (%s) {\n", cond);
            else if (stmt_type == STMT_FOR) fprintf(out, "for (%s) {\n", cond);
            break;
        }

        case STMT_ELSE: {
            fprintf(out, "else {\n");
            break;
        }

        case STMT_CLOSING_BRACE: {
            fprintf(out, "}\n");
            break;
        }

        case STMT_EXIT: {
            fprintf(out, "exit(1);\n");
            break;
        }

        case STMT_UNKNOWN: {
            fprintf(stderr, "Unknown statement: %s\n", line);
            break;
        }

        case STMT_EMPTY:
            break; // Do nothing

        default:
            fprintf(stderr, "Invalid syntax: %s\n", line);
            break;
    }
}
