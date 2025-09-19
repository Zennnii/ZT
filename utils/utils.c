#include "utils.h"

void trim_newline(char *s) {
    char *p = strchr(s, '\n');
    if (p) *p = '\0';
}

// Utility: trim leading/trailing whitespace and CR/LF
void trim_whitespace(char *s) {
    size_t len = strlen(s);
    // Trim trailing \r and \n
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) {
        s[len-1] = '\0';
        len--;
    }
    // Trim trailing spaces/tabs
    while (len > 0 && isspace((unsigned char)s[len-1])) {
        s[len-1] = '\0';
        len--;
    }
    // Trim leading spaces/tabs
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start)+1);
}

// Utility: strip outer parentheses if present
void strip_outer_parentheses(char *s) {
    trim_whitespace(s);
    size_t len = strlen(s);
    if (len >= 2 && s[0] == '(' && s[len-1] == ')') {
        memmove(s, s+1, len-2);
        s[len-2] = '\0';
    }
}