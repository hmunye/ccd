#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    FLAG_NONE = 0,
    FLAG_HELP = 1 << 0,           /* `-h` or `--help` flag  */
    FLAG_BYTES_PER_LINE = 1 << 1, /* `-c` flag  */
    FLAG_BYTE_SPACING = 1 << 2    /* `-g` flag  */
} Flags;

#define DEFAULT_BYTES_PER_LINE 16 /* bytes to print per line */
#define DEFAULT_BYTE_SPACING 2    /* byte grouping */
#define MAX_FLAG_LENGTH 6

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

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s [options] [filename]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Flags flags = FLAG_NONE;
    size_t bytes_per_line = DEFAULT_BYTES_PER_LINE;
    size_t byte_spacing = DEFAULT_BYTE_SPACING;

    /* parse arguments */
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "-h", MAX_FLAG_LENGTH) == 0 ||
            strncmp(argv[i], "--help", MAX_FLAG_LENGTH) == 0) {
            flags |= FLAG_HELP;
            continue;
        }

        if (strncmp(argv[i], "-c", MAX_FLAG_LENGTH) == 0) {
            if (i + 1 < argc) {
                bytes_per_line = (size_t)atoi(argv[++i]);

                if (bytes_per_line <= 0) {
                    fprintf(
                        stderr,
                        "ERROR: flag -c: invalid number for octets per line\n");
                    exit(EXIT_FAILURE);
                }

                flags |= FLAG_BYTES_PER_LINE;
                continue;
            } else {
                fprintf(
                    stderr,
                    "ERROR: -c flag required to specify octets per line.\n");
                exit(EXIT_FAILURE);
            }
        }

        if (strncmp(argv[i], "-g", MAX_FLAG_LENGTH) == 0) {
            if (i + 1 < argc) {
                byte_spacing = (size_t)atoi(argv[++i]);

                if (bytes_per_line <= 0) {
                    fprintf(
                        stderr,
                        "ERROR: flag -g: invalid number for byte spacing\n");
                    exit(EXIT_FAILURE);
                }

                flags |= FLAG_BYTE_SPACING;
                continue;
            } else {
                fprintf(
                    stderr,
                    "ERROR: -g flag required to specify bytes per group.\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (flags & FLAG_HELP) {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }

    char *file_path = argv[argc - 1];
    FILE *file = fopen(file_path, "r");

    if (!file) {
        fprintf(stderr, "ERROR: failed to open file %s: %s\n", file_path,
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    char buf[bytes_per_line];
    size_t byte_offset = 0;
    size_t bytes_read;

    while ((bytes_read = fread(buf, sizeof(char), bytes_per_line, file)) > 0) {
        printf("%08zx: ", byte_offset);

        for (size_t i = 0; i < bytes_read; i++) {
            if (i != 0 && i % byte_spacing == 0) {
                printf(" ");
            }

            printf("%02x", (unsigned char)buf[i]);
        }

        if (bytes_read < bytes_per_line) {
            // padding: (remaining bytes) * 2 for hex width, + spaces between
            // byte groups
            size_t padding = (bytes_per_line - bytes_read) * 2 +
                             ((bytes_per_line - bytes_read) / byte_spacing);
            printf("%*s", (int)padding, "");
        }

        printf("  ");

        for (size_t i = 0; i < bytes_read; i++) {
            /* range [33, 126] from ASCII table */
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

    if (ferror(file)) {
        fprintf(stderr, "ERROR: failed to read file %s: %s\n", file_path,
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (fclose(file) == EOF) {
        fprintf(stderr, "ERROR: failed to close file %s: %s\n", file_path,
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
