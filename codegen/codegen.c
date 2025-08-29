#include "codegen.h"

extern FILE *out; // Reference to global output file

void generate_code(StatementType stmt_type, char *line) {
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
            fprintf(out, "%s;\n", line);
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
            
            if (strncmp(type, "int", 3) == 0) {
                fprintf(out, "scanf(\"%%d\", &%s);\n", name);
            }
            else if (strncmp(type, "float", 5) == 0) {
                fprintf(out, "scanf(\"%%f\", &%s);\n", name);
            }
            else if (strncmp(type, "double", 6) == 0) {
                fprintf(out, "scanf(\"%%lf\", &%s);\n", name);
            }
            else if (strncmp(type, "char", 4) == 0) {
                fprintf(out, "scanf(\"%%c\", &%s);\n", name);
            }
            else {
                fprintf(out, "scanf(\"%%d\", &%s);\n", name);
            }
            break;
        }

        case STMT_IF: {
            char cond[128];
            char *brace = strchr(line, '{');
            if (brace) {
                size_t len = brace - (line + 3);
                strncpy(cond, line + 3, len);
                cond[len] = '\0';
            } else {
                strcpy(cond, line + 3);
            }
            strip_outer_parentheses(cond);
            fprintf(out, "if (%s) {\n", cond);
            break;
        }

        case STMT_ELSE_IF: {
            char cond[128];
            char *brace = strchr(line, '{');
            if (brace) {
                size_t len = brace - (line + 8);
                strncpy(cond, line + 8, len);
                cond[len] = '\0';
            } else {
                strcpy(cond, line + 8);
            }
            strip_outer_parentheses(cond);
            fprintf(out, "else if (%s) {\n", cond);
            break;
        }

        case STMT_ELSE: {
            fprintf(out, "else {\n");
            break;
        }

        case STMT_WHILE: {
            char cond[128];
            char *brace = strchr(line, '{');
            if (brace) {
                size_t len = brace - (line + 6);
                strncpy(cond, line + 6, len);
                cond[len] = '\0';
            } else {
                strcpy(cond, line + 6);
            }
            strip_outer_parentheses(cond);
            fprintf(out, "while (%s) {\n", cond);
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
        
        case STMT_EMPTY: {
            // Do nothing for empty lines
            break;
        }
        
        default: {
            fprintf(stderr, "Invalid syntax: %s\n", line);
            break;
        }
    }
}