#include <ctype.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

uint8_t *hex2byte(char const *hex);

int main(int argc, char const *argv[]) {
    int fd;
    struct stat st;
    uint8_t *file, *o_byte, *n_byte;
    size_t file_size, hex_size, offset;

    if (argc != 4) {
        fprintf(stderr, "Error: except three args\n");
        exit(EXIT_FAILURE);
    }

    o_byte = hex2byte(argv[2]);
    n_byte = hex2byte(argv[3]);

    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "Error: failed to open target file\n");
    }

    fstat(fd, &st);
    file_size = st.st_size;
    hex_size = strlen(argv[2]) / 2;

    file = mmap(0, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE,
                fd, 0);

    for (offset = 0; offset <= file_size - hex_size; offset++) {
        if (memcmp(file + offset, o_byte, hex_size) == 0) {
            memset(file + offset, 0, hex_size);
            memcpy(file + offset, n_byte, hex_size);
        }
    }

    msync(file, file_size, MS_SYNC);
    munmap(file, file_size);

    close(fd);

    free(o_byte);
    free(n_byte);

    exit(EXIT_SUCCESS);
}

uint8_t *hex2byte(char const *hex) {
    uint8_t *byte;
    char high, low;
    byte = malloc(sizeof(uint8_t) * strlen(hex) / 2);

    for (int i = 0; i < strlen(hex) - 1; i += 2) {
        high = toupper(hex[i]) - '0';
        low = toupper(hex[i + 1]) - '0';
        byte[i / 2] =
            ((high > 9 ? high - 7 : high) << 4) + (low > 9 ? low - 7 : low);
    }

    return byte;
}
