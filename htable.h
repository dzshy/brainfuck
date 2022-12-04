#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    void *buf;
    int size;
    int cap;
    int taken;
    void* begin;
    int elemsz;
    uint32_t (*hash)(void*);
    bool (*eq)(void*, void*);
} HTable;

void htable_init(HTable *ht, int elemsz, int cap, uint32_t (*hash)(void*),
                 bool (*eq)(void*, void*));
bool htable_insert(HTable *ht, void* elem);
void htable_del(HTable *ht, void* iter);

// return a iterator
void* htable_find(HTable *ht, void* elem);
void* htable_begin(HTable *ht);
void* htable_next(HTable *ht, void *iter);

#endif

