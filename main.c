#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    FLAG_NONE = 0,
    FLAG_HELP = 1 << 0,           /* `-h` or `--help` flag  */
    FLAG_OCTETS_PER_LINE = 1 << 1 /* `-c` flag  */
} Flags;

#define MAX_FLAG_LENGTH 6
#define DEFAULT_OCTETS_PER_LINE 16

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
    int oct_per_line = DEFAULT_OCTETS_PER_LINE;

    /* parse arguments */
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "-h", MAX_FLAG_LENGTH) == 0 ||
            strncmp(argv[i], "--help", MAX_FLAG_LENGTH) == 0) {
            flags |= FLAG_HELP;
            continue;
        }

        if (strncmp(argv[i], "-c", MAX_FLAG_LENGTH) == 0) {
            if (i + 1 < argc) {
                oct_per_line = atoi(argv[++i]);

                if (oct_per_line <= 0) {
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

    return EXIT_SUCCESS;
}
