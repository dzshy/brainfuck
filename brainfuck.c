#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOGGER(s, ...) do {\
    fprintf(stderr, s"\n", ##__VA_ARGS__); \
} while(0)

uint8_t *databuf;
static const int INIT_SZ = 1024;

void checkright(long pointer, long cur_data_sz) {
    if (pointer == cur_data_sz - 1) {
        databuf = realloc(databuf, cur_data_sz * 2);
        memset(databuf + cur_data_sz, 0, cur_data_sz);
        cur_data_sz *= 2;
    }
}

long move2right(char *prog, long p, long fsz) {
    int count = 1;
    while (count > 0 && p < fsz) {
        p++;
        if (prog[p] == ']') count--;
        if (prog[p] == '[') count++;
    }
    return p;
}

long move2left(char *prog, long p) {
    int count = 1;
    while (count > 0 && p > 0) {
        p--;
        if (prog[p] == '[') count--;
        if (prog[p] == ']') count++;
    }
    return p;
}

void interp(char *prog, long prog_sz) {
    long prog_p = 0;
    long data_p = 0;
    long cur_data_sz = INIT_SZ;
    databuf = malloc(INIT_SZ);
    memset(databuf, 0, INIT_SZ);
    while (prog_p < prog_sz) {
        switch (prog[prog_p]) {
        case '>':
            checkright(data_p, cur_data_sz);
            data_p++;
            break;
        case '<':
            if (data_p == 0) {
                LOGGER("error, left out of index");
                exit(EXIT_FAILURE);
            }
            data_p--;
            break;
        case '+':
            databuf[data_p]++;
            break;
        case '-':
            databuf[data_p]--;
            break;
        case '.':
            putchar(databuf[data_p]);
            break;
        case ',':
            databuf[data_p] = getchar();
            break;
        case '[':
            if (databuf[data_p] == 0) {
                prog_p = move2right(prog, prog_p, prog_sz);
            }
            break;
        case ']':
            if (databuf[data_p] != 0) {
                prog_p = move2left(prog, prog_p);
            }
            break;
        default:
            break;
        }
        prog_p++;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        LOGGER("invalid args");
        return EXIT_FAILURE;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        return EXIT_FAILURE;
    }
    struct stat fst;
    int ret = fstat(fd, &fst);
    if (ret != 0) {
        LOGGER("error: failed to get file stat");
        return EXIT_FAILURE;
    }
    long filesz = fst.st_size;
    char *prog = (char*)mmap(NULL, filesz, PROT_READ, MAP_SHARED, fd, 0); 
    interp(prog, filesz);
    return EXIT_SUCCESS;
}


