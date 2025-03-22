#ifndef CCD_H
#define CCD_H

#include <stddef.h>
#include <stdio.h>

#define DEFAULT_BYTES_PER_LINE 16 /* how many bytes to print per line */
#define DEFAULT_BYTE_GROUPING 2   /* how many bytes per group */
#define MAX_FLAG_LENGTH 6         /* max length of flag when parsing args */

typedef enum {
    FLAG_NONE = 0,
    FLAG_HELP = 1 << 0,           /* `-h` or `--help` flag  */
    FLAG_BYTES_PER_LINE = 1 << 1, /* `-c` flag  */
    FLAG_BYTE_GROUPING = 1 << 2   /* `-g` flag  */
} ccd_flags_t;

extern ccd_flags_t user_flags;
extern size_t bytes_per_line;
extern size_t byte_grouping;

/* parse command-line arguments, setting flags and configuration options */
void parse_args(int argc, char **argv);

/* print usage instructions for the program */
void print_help(const char *prog_name);

/* read a file and print its contents in a hex/ascii dump format */
void print_dump(FILE *file);

#endif  // CCD_H
