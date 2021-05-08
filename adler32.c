#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define BUFLEN 4096

int main(int argc, char** argv) {
    int ret = EXIT_SUCCESS; // Return value
    char* path; // Current file (path)
    size_t bytes_read;
    int i;
    size_t j;
    uint32_t s1;
    uint32_t s2;
    char skip = 0; // Skip output for file
    uint8_t buf[BUFLEN];

    for (i = 1; i < argc; i++) {
        path = argv[i];
        FILE* f = fopen(path, "rb");
        if (f == NULL) {
            printf("Error while opening %s\n", path);
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
                printf("Error while reading %s\n", path);
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

        printf("%08x  %s\n", (s2 << 16) | s1, path);
    }

    return ret;
}

