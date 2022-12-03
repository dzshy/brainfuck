#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "rbtree.h"

#define LOGGER(s, ...) do {\
    fprintf(stderr, s"\n", ##__VA_ARGS__); \
} while(0)

uint8_t *pbuf;
uint8_t *nbuf;
long nsz, psz;
static const int INIT_SZ = 128;

typedef struct {
    RbNode tree;
    long key;
    long val;
} JmpTblEntry;

RbTree jmptbl = {0};

struct listnode {
    struct listnode *next;
    long val;
};
typedef struct {
    struct listnode *head;
} Stack;

void stack_push(Stack *s, long val) {
    struct listnode *n = malloc(sizeof(struct listnode));
    n->next = s->head;
    n->val = val;
    s->head = n;
}

long* stack_top(Stack *s) {
    if (s->head == NULL) return NULL;
    return &(s->head->val);
}

void stack_pop(Stack *s) {
    if (s->head == NULL) return;
    struct listnode *next = s->head->next;
    free(s->head);
    s->head = next;
}

int longcmp(void *x, void *y) {
    long *a = x, *b = y;
    if (*a < *b) return 1;
    if (*a > *b) return -1;
    return 0;
}

void buildjmptable(char* buf, long len) {
    Stack s = {0};
    jmptbl.cmp = longcmp;
    for (long i = 0; i < len; i++) {
        if (buf[i] == '[') {
            stack_push(&s, i);
        } else if (buf[i] == ']') {
            long j = *stack_top(&s);
            stack_pop(&s);
            JmpTblEntry *e1 = malloc(sizeof(JmpTblEntry));
            JmpTblEntry *e2 = malloc(sizeof(JmpTblEntry));
            e1->key = i;
            e1->val = j;
            e2->key = j;
            e2->val = i;
            rbtree_insert(&jmptbl, e1);
            rbtree_insert(&jmptbl, e2);
        }
    }
}

static long tbllookup(long pos) {
    JmpTblEntry *iter = rbtree_find(&jmptbl, &pos);
    return iter->val;
}

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
            break;
        case ',':
            setdata(data_p, getchar());
            break;
        case '[':
            if (getdata(data_p) == 0) {
                prog_p = tbllookup(prog_p);
            }
            break;
        case ']':
            if (getdata(data_p) != 0) {
                prog_p = tbllookup(prog_p);
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
    buildjmptable(prog, filesz);
    interp(prog, filesz);
    return EXIT_SUCCESS;
}

