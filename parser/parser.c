#include "parser.h"

StatementType parse_line(char *line) {
    // Handle comments
    char *comment = strstr(line, "//");
    if (comment) *comment = '\0';
    trim_whitespace(line);
    if (*line == '\0') return STMT_EMPTY;

    // Let statements
    if (strncmp(line, "let ", 4) == 0) {
        return STMT_LET;
    }
    // Variable assignment
    else if (strchr(line, '=') != NULL && strncmp(line, "input", 5) != 0) {
        return STMT_ASSIGNMENT;
    }
    // Echo statement
    else if (strncmp(line, "echo ", 5) == 0) {
        return STMT_ECHO;
    }
    // Input statement
    else if (strncmp(line, "input ", 6) == 0) {
        return STMT_INPUT;
    }
    // If statement
    else if (strncmp(line, "if ", 3) == 0) {
        return STMT_IF;
    }
    // Else if statement
    else if (strncmp(line, "else if ", 8) == 0) {
        return STMT_ELSE_IF;
    }
    // Else statement
    else if (strncmp(line, "else", 4) == 0) {
        return STMT_ELSE;
    }
    // While statement
    else if (strncmp(line, "while ", 6) == 0) {
        return STMT_WHILE;
    }
    // Closing brace 
    else if (strchr(line, '}') != NULL) {
        return STMT_CLOSING_BRACE;
    }
    // Exit statement
    else if (strncmp(line, "exit", 4) == 0) {
        return STMT_EXIT;
    }
    // Unknown syntax
    else {
        return STMT_UNKNOWN;
    }
}