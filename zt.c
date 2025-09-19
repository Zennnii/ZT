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

    FILE *tmp = fopen("templates/gtk_boilerplate", "r");
    if (!tmp) {
        perror("Failed to open template file");
        return 1;
    }

    int is_gui = 1;  // default

    // Read the first line
    if (fgets(line_buf, sizeof(line_buf), in)) {
        trim_newline(line_buf);
        trim_whitespace(line_buf);

        if (strncmp(line_buf, ":NON_GUI:", 9) == 0) {
            is_gui = 0;
        }
    }

    if (!is_gui) {
        // Emit standard non-GUI boilerplate
        fprintf(out,
            "#include <stdio.h>\n"
            "#include <stdlib.h>\n\n"
            "int main() {\n"
        );

        // Now continue parsing remaining lines as non-GUI statements
    } else {
        // GUI program logic
        char window_title[128] = {0};
        int win_width = 400, win_height = 300;  // defaults

        // If first line is GUI window declaration
        if (strncmp(line_buf, "window ", 7) == 0) {
            snprintf(window_title, sizeof(window_title), "%.*s", (int)(sizeof(window_title)-1), line_buf + 7);
        }

        // Read second line -> size
        if (fgets(line_buf, sizeof(line_buf), in)) {
            trim_newline(line_buf);
            trim_whitespace(line_buf);
            if (strncmp(line_buf, "windowSize", 10) == 0) {
                int w, h;
                if (sscanf(line_buf, "windowSize (%d, %d)", &w, &h) == 2) {
                    win_width = w;
                    win_height = h;
                }
            }
        }

        // Emit GTK boilerplate
        fseek(tmp, 0, SEEK_END);
        long size = ftell(tmp);  // signed
        fseek(tmp, 0, SEEK_SET);

        char *buffer = malloc(size + 1);
        if (!buffer) { perror("malloc failed"); return 1; }

        size_t read_bytes = fread(buffer, 1, (size_t)size, tmp);  // cast size to size_t
        if (read_bytes != (size_t)size) {
            fprintf(stderr, "Failed to read the entire template file\n");
            fclose(tmp);
            free(buffer);
            return 1;
        }
        buffer[size] = '\0';
        fclose(tmp);

        fprintf(out, buffer, window_title, win_width, win_height);
        free(buffer);
    }


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


    if (is_gui == 0) {
        fprintf(out, "return 0;\n}\n");
    }
    else {
        // do nothing
    }
    fclose(in);
    fclose(out);

    char cmd[512];
    if (is_gui == 1) {
        snprintf(cmd, sizeof(cmd), "gcc \"%s\" -o \"%s\" $(pkg-config --cflags --libs gtk+-3.0)", c_file, output_file);
    }
    else {
        snprintf(cmd, sizeof(cmd), "gcc \"%s\" -o \"%s\"", c_file, output_file);
    }
    int ret = system(cmd);
    if (ret == 0) {
        printf("Compiled %s -> %s\n", input_file, output_file);
        if (!keep_c) {
            remove(c_file);
        }
        if (run) {
			printf("\n");
			char rcmd[512];
			snprintf(rcmd, sizeof(rcmd), "./%s", output_file);
			int ret2 = system(rcmd);
			if (ret2 != 0) {
				fprintf(stderr, "Failed to run %s\n", output_file);
			}
		}
    } else {
        fprintf(stderr, "GCC compilation failed!\n");
        return 1;
    }

    return 0;
}