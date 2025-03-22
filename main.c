#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/ccd.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s [options] [filename]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    parse_args(argc, argv);

    if (user_flags & FLAG_HELP) {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }

    char *file_path = argv[argc - 1];
    FILE *file = fopen(file_path, "rb");

    if (!file) {
        fprintf(stderr, "ERROR: failed to open file %s: %s\n", file_path,
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    print_dump(file);

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
