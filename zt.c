#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "zt.h"
#include "utils/utils.h"
#include "parser/parser.h"
#include "codegen/codegen.h"

char line_buf[256];
FILE *out;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s input.zt -o output_executable\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    char *output_file = NULL;
    int keep_c = 0;
    int run = 0;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i+1 < argc) {
            output_file = argv[i+1];
            i++;
        } 
        else if (strcmp(argv[i], "--keep-c") == 0) {
            keep_c = 1;
        }
        else if (strcmp(argv[i], "--run") == 0) {
            run = 1;
        }
    }

    if (!output_file) {
        fprintf(stderr, "Error: no output file specified (use -o <file>)\n");
        return 1;
    }

    char c_file[256];
    snprintf(c_file, sizeof(c_file), "%s.c", output_file);

    FILE *in = fopen(input_file, "r");
    out = fopen(c_file, "w");
    if (!in || !out) {
        perror("File open failed");
        return 1;
    }

    fprintf(out, "#include <stdio.h>\n#include <stdlib.h>\nint main() {\n");


    while (fgets(line_buf, sizeof(line_buf), in)) {
        trim_newline(line_buf);
        trim_whitespace(line_buf);

        // Skip empty lines and comments
        if (strlen(line_buf) == 0 || line_buf[0] == '#') {
            continue;
        }

        // Parse the line to get statement type
        StatementType stmt_type = parse_line(line_buf);
        
        // Generate code based on statement type
        if (stmt_type != STMT_UNKNOWN) {
            generate_code(stmt_type, line_buf);
        }
    }



    fprintf(out, "return 0;\n}\n");
    fclose(in);
    fclose(out);

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "gcc \"%s\" -o \"%s\"", c_file, output_file);
    int ret = system(cmd);
    if (ret == 0) {
        printf("Compiled %s -> %s\n", input_file, output_file);
        if (!keep_c) {
            remove(c_file);
        }
        else if (run == 1) {
            
        }
    } else {
        fprintf(stderr, "GCC compilation failed!\n");
        return 1;
    }

    return 0;
}