#include "utils.h"

void trim_newline(char *s) {
    char *p = strchr(s, '\n');
    if (p) *p = '\0';
}

void trim_whitespace(char *s) {
    char *p = s;
    while (*p && isspace(*p)) p++;
    // Shift string left to remove leading spaces
    memmove(s, p, strlen(p)+1);
}

// Function to strip outer parentheses if they wrap the whole expression
void strip_outer_parentheses(char *cond) {
    trim_whitespace(cond);
    size_t len = strlen(cond);
    if (len >= 2 && cond[0] == '(' && cond[len - 1] == ')') {
        // Remove the first and last character
        memmove(cond, cond + 1, len - 2);
        cond[len - 2] = '\0';
        trim_whitespace(cond); // Remove any extra spaces
    }
}