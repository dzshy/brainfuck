/*
 * Copyright 2002 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>

struct rbnode {
  struct {
    struct rbnode *rbe_left;
    struct rbnode *rbe_right;
    struct rbnode *rbe_parent;
    int rbe_color;
  } entry;
  char content[];
};

struct rbtree {
  struct rbnode *rbh_root;
  int (*cmp)(void *k1, void *k2);
};

typedef struct rbnode RbNode;
typedef struct rbtree RbTree;

void rbtree_remove(struct rbtree *, void* iter);

// return a iterator
void* rbtree_insert(struct rbtree *, void *treenode);
void* rbtree_find(struct rbtree *, void *val);
void* rbtree_next(struct rbtree *, void* iter);
void* rbtree_min(struct rbtree *);
void* rbtree_max(struct rbtree *);

void rbtree_free(struct rbtree *, void (*free_cb)(void*));
