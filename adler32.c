/*******************************************************************************
 * adler32.c: adler32 version 1.0.1
 * Copyright 2021 Hannes Braun (hannesbraun@mail.de)
 *
 * Version 1.0.1: Add newline character to the usage string
 *                                                    by Hannes Braun 2021/05/09
 * Version 1.0.0: Basic Adler-32 implementation for one or more files
 *                                                    by Hannes Braun 2021/05/09
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 4096

struct options {
    unsigned char valid:1;
    unsigned char quiet:1;
};

struct options parse_options(int argc, char** argv) {
    char* arg;
    size_t len;
    struct options options = {1, 0};

    // Loop variables
    int i;
    size_t j;

    for (i = 1; i < argc; i++) {
        arg = argv[i];

        if (arg[0] != '-') {
            break; // First file argument found
        }

        len = strlen(arg);
        for (j = 1; j < len; j++) {
            if (arg[j] == 'q') {
                options.quiet = 1;
            } else {
                options.valid = 0;
                printf("%s: illegal option -- %c\n", argv[0], arg[j]);
            }
        }
    }

    if (!options.valid) {
        printf("usage: %s [-q] [files ...]\n", argv[0]);
    }

    return options;
}

int main(int argc, char** argv) {
    int ret = EXIT_SUCCESS; // Return value
    char* arg; // Current argument
    FILE* f;
    size_t bytes_read;
    int i;
    size_t j;
    uint8_t buf[BUFLEN];
    uint32_t s1;
    uint32_t s2;
    char skip = 0; // Skip output for file
    char reading_args = 1;

    struct options options = parse_options(argc, argv);
    if (!options.valid) {
        return EXIT_FAILURE;
    }

    for (i = 1; i < argc; i++) {
        arg = argv[i];
        if (reading_args && arg[0] == '-') {
            continue;
        } else {
            reading_args = 0;
        }

        f = fopen(arg, "rb");
        if (f == NULL) {
            printf("Error while opening %s\n", arg);
            continue;
        }

        s1 = 1;
        s2 = 0;

        while (1) {
            bytes_read = fread(&buf, 1, BUFLEN, f);
            if (ferror(f))
            {
                ret = EXIT_FAILURE;
                skip = 1;
                printf("Error while reading %s\n", arg);
                break;
            }

            for (j = 0; j < bytes_read; j++) {
                s1 = (s1 + buf[j]);
                s2 = (s1 + s2);
            }

            s1 %= 65521;
            s2 %= 65521;

            if (bytes_read != BUFLEN && feof(f)) break;
        }

        fclose(f);

        if (skip == 1) {
            skip = 0;
            continue;
        }

        if (options.quiet) {
            printf("%08x\n", (s2 << 16) | s1);
        } else {
            printf("%08x  %s\n", (s2 << 16) | s1, arg);
        }
    }

    return ret;
}
