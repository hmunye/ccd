#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    FLAG_NONE = 0,
    FLAG_HELP = 1 << 0,           /* `-h` or `--help` flag  */
    FLAG_OCTETS_PER_LINE = 1 << 1 /* `-c` flag  */
} Flags;

#define MAX_FLAG_LENGTH 6
#define DEFAULT_BYTES_PER_LINE 16
#define BYTE_SPACING 2 /* Insert space after every 2 bytes */

void print_help(const char *prog_name) {
    printf("Usage:\n");
    printf("       %s [options] [filename]\n", prog_name);
    printf("Options:\n");
    printf("       -h, --help      Show help message.\n");
    printf(
        "       -c cols         Format <cols> octets per line. Default 16.\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s [options] [filename]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Flags flags = FLAG_NONE;
    int bytes_per_line = DEFAULT_BYTES_PER_LINE;

    /* parse arguments */
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "-h", MAX_FLAG_LENGTH) == 0 ||
            strncmp(argv[i], "--help", MAX_FLAG_LENGTH) == 0) {
            flags |= FLAG_HELP;
            continue;
        }

        if (strncmp(argv[i], "-c", MAX_FLAG_LENGTH) == 0) {
            if (i + 1 < argc) {
                bytes_per_line = atoi(argv[++i]);

                if (bytes_per_line <= 0) {
                    fprintf(stderr,
                            "ERROR: invalid number for octets per line\n");
                    exit(EXIT_FAILURE);
                }

                flags |= FLAG_OCTETS_PER_LINE;
                continue;
            } else {
                fprintf(
                    stderr,
                    "ERROR: -c flag required to specify octets per line.\n");
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
    size_t bytes_read = 0;
    size_t byte_offset = 0;

    while ((bytes_read =
                fread(buf, sizeof(char), (size_t)bytes_per_line, file)) > 0) {
        printf("%08zx: ", byte_offset);

        for (size_t i = 0; i < bytes_read; i++) {
            if (i != 0 && i % BYTE_SPACING == 0) {
                printf(" ");
            }

            printf("%02x", (unsigned char)buf[i]);
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
