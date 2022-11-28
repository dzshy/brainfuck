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

uint8_t *pbuf;
uint8_t *nbuf;
long nsz, psz;
static const int INIT_SZ = 128;

void ensurespc_impl(uint8_t** buf, long *cursz, long ptr) {
    if (ptr > *cursz) {
        *buf = realloc(*buf, ptr * 2);
        memset((*buf) + (*cursz), 0, 2 * ptr - (*cursz));
        *cursz = ptr * 2;
    }
}

void ensurespc(long ptr) {
    if (ptr >= 0) {
        ensurespc_impl(&pbuf, &psz, ptr);        
    } else {
        ensurespc_impl(&nbuf, &nsz, -ptr);
    }
}

uint8_t getdata(long ptr) {
    ensurespc(ptr);
    if (ptr >= 0) return pbuf[ptr];
    else return nbuf[-ptr];
}

void setdata(long ptr, uint8_t val) {
    ensurespc(ptr);
    if (ptr >= 0) pbuf[ptr] = val;
    else nbuf[-ptr] = val;
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
    nbuf = malloc(INIT_SZ);
    pbuf = malloc(INIT_SZ);
    nsz = INIT_SZ;
    psz = INIT_SZ;
    memset(nbuf, 0, INIT_SZ);
    memset(pbuf, 0, INIT_SZ);

    while (prog_p < prog_sz) {
        switch (prog[prog_p]) {
        case '>':
            data_p++;
            break;
        case '<':
            data_p--;
            break;
        case '+':
            setdata(data_p, getdata(data_p) + 1);
            break;
        case '-':
            setdata(data_p, getdata(data_p) - 1);
            break;
        case '.':
            putchar(getdata(data_p));
            fflush(stdout);
            break;
        case ',':
            setdata(data_p, getchar());
            break;
        case '[':
            if (getdata(data_p) == 0) {
                prog_p = move2right(prog, prog_p, prog_sz);
            }
            break;
        case ']':
            if (getdata(data_p) != 0) {
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

