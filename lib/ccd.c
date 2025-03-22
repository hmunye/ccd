#include <stdlib.h>
#include <string.h>

#include "ccd.h"

ccd_flags_t user_flags = FLAG_NONE;
size_t bytes_per_line = DEFAULT_BYTES_PER_LINE;
size_t byte_grouping = DEFAULT_BYTE_GROUPING;

void parse_args(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "-h", MAX_FLAG_LENGTH) == 0 ||
            strncmp(argv[i], "--help", MAX_FLAG_LENGTH) == 0) {
            user_flags |= FLAG_HELP;

            continue;
        }

        if (strncmp(argv[i], "-c", MAX_FLAG_LENGTH) == 0) {
            if (i + 1 < argc) {
                int arg = atoi(argv[++i]);

                if (arg <= 0) {
                    fprintf(stderr,
                            "ERROR: -c: invalid number for octets per line\n");
                    exit(EXIT_FAILURE);
                }

                bytes_per_line = (size_t)arg;
                user_flags |= FLAG_BYTES_PER_LINE;

                continue;
            } else {
                fprintf(stderr,
                        "ERROR: -c: required to specify octets per line\n");
                exit(EXIT_FAILURE);
            }
        }

        if (strncmp(argv[i], "-g", MAX_FLAG_LENGTH) == 0) {
            if (i + 1 < argc) {
                int arg = atoi(argv[++i]);

                if (bytes_per_line <= 0) {
                    fprintf(stderr,
                            "ERROR: -g: invalid number for octets per group\n");
                    exit(EXIT_FAILURE);
                }

                byte_grouping = (size_t)arg;
                user_flags |= FLAG_BYTE_GROUPING;

                continue;
            } else {
                fprintf(stderr,
                        "ERROR: -g: required to specify octets per group\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void print_help(const char *prog_name) {
    printf("Usage:\n");
    printf("       %s [options] [filename]\n", prog_name);
    printf("Options:\n");
    printf("       -h, --help      Show help message.\n");
    printf(
        "       -c cols         Format <cols> octets per line. Default "
        "16.\n");
    printf(
        "       -g bytes        Number of octets per group in normal "
        "output. "
        "Default 2.\n");
}

void print_dump(FILE *file) {
    char buf[bytes_per_line];
    size_t byte_offset = 0;
    size_t bytes_read;

    while ((bytes_read = fread(buf, sizeof(char), bytes_per_line, file)) > 0) {
        printf("%08zx: ", byte_offset);

        for (size_t i = 0; i < bytes_read; i++) {
            if (i != 0 && i % byte_grouping == 0) {
                printf(" ");
            }

            printf("%02x", (unsigned char)buf[i]);
        }

        if (bytes_read < bytes_per_line) {
            // padding = (remaining bytes) * 2 for hex width, + spaces between
            // byte groups
            size_t padding = (bytes_per_line - bytes_read) * 2 +
                             ((bytes_per_line - bytes_read) / byte_grouping);
            printf("%*s", (int)padding, "");
        }

        printf("  ");

        for (size_t i = 0; i < bytes_read; i++) {
            /* range [33, 126] base_10 in ASCII table */
            if (buf[i] >= '!' && buf[i] <= '~') {
                printf("%c", buf[i]);
            } else if (buf[i] == ' ') {
                printf(" ");
            } else {
                printf(".");
            }
        }

        printf("\n");

        byte_offset += bytes_read;
    }
}
