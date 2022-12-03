/*	$OpenBSD: tree.h,v 1.30 2020/10/10 18:03:41 otto Exp $	*/
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

#include "rbtree.h"

static struct rbnode *rbtree_RB_MINMAX(struct rbtree *, int);
void* rbtree_min(struct rbtree *head) {
    return rbtree_RB_MINMAX(head, -1);
}
void* rbtree_max(struct rbtree *head) {
    return rbtree_RB_MINMAX(head, 1);
}


static void rbtree_insert_COLOR(struct rbtree *, struct rbnode *);
static void rbtree_remove_COLOR(struct rbtree *, struct rbnode *, struct rbnode *);

static void rbtree_insert_COLOR(struct rbtree *head, struct rbnode *elm) {
  struct rbnode *parent, *gparent, *tmp;
  while ((parent = (elm)->entry.rbe_parent) && (parent)->entry.rbe_color == 1) {
    gparent = (parent)->entry.rbe_parent;
    if (parent == (gparent)->entry.rbe_left) {
      tmp = (gparent)->entry.rbe_right;
      if (tmp && (tmp)->entry.rbe_color == 1) {
        (tmp)->entry.rbe_color = 0;
        do {
          (parent)->entry.rbe_color = 0;
          (gparent)->entry.rbe_color = 1;
        } while (0);
        elm = gparent;
        continue;
      }
      if ((parent)->entry.rbe_right == elm) {
        do {
          (tmp) = (parent)->entry.rbe_right;
          if (((parent)->entry.rbe_right = (tmp)->entry.rbe_left)) {
            ((tmp)->entry.rbe_left)->entry.rbe_parent = (parent);
          };
          if (((tmp)->entry.rbe_parent = (parent)->entry.rbe_parent)) {
            if ((parent) == ((parent)->entry.rbe_parent)->entry.rbe_left)
              ((parent)->entry.rbe_parent)->entry.rbe_left = (tmp);
            else
              ((parent)->entry.rbe_parent)->entry.rbe_right = (tmp);
            ;
          } else
            (head)->rbh_root = (tmp);
          (tmp)->entry.rbe_left = (parent);
          (parent)->entry.rbe_parent = (tmp);
          ;
        } while (0);
        tmp = parent;
        parent = elm;
        elm = tmp;
      }
      do {
        (parent)->entry.rbe_color = 0;
        (gparent)->entry.rbe_color = 1;
      } while (0);
      do {
        (tmp) = (gparent)->entry.rbe_left;
        if (((gparent)->entry.rbe_left = (tmp)->entry.rbe_right)) {
          ((tmp)->entry.rbe_right)->entry.rbe_parent = (gparent);
        };
        if (((tmp)->entry.rbe_parent = (gparent)->entry.rbe_parent)) {
          if ((gparent) == ((gparent)->entry.rbe_parent)->entry.rbe_left)
            ((gparent)->entry.rbe_parent)->entry.rbe_left = (tmp);
          else
            ((gparent)->entry.rbe_parent)->entry.rbe_right = (tmp);
          ;
        } else
          (head)->rbh_root = (tmp);
        (tmp)->entry.rbe_right = (gparent);
        (gparent)->entry.rbe_parent = (tmp);
        ;
      } while (0);
    } else {
      tmp = (gparent)->entry.rbe_left;
      if (tmp && (tmp)->entry.rbe_color == 1) {
        (tmp)->entry.rbe_color = 0;
        do {
          (parent)->entry.rbe_color = 0;
          (gparent)->entry.rbe_color = 1;
        } while (0);
        elm = gparent;
        continue;
      }
      if ((parent)->entry.rbe_left == elm) {
        do {
          (tmp) = (parent)->entry.rbe_left;
          if (((parent)->entry.rbe_left = (tmp)->entry.rbe_right)) {
            ((tmp)->entry.rbe_right)->entry.rbe_parent = (parent);
          };
          if (((tmp)->entry.rbe_parent = (parent)->entry.rbe_parent)) {
            if ((parent) == ((parent)->entry.rbe_parent)->entry.rbe_left)
              ((parent)->entry.rbe_parent)->entry.rbe_left = (tmp);
            else
              ((parent)->entry.rbe_parent)->entry.rbe_right = (tmp);
            ;
          } else
            (head)->rbh_root = (tmp);
          (tmp)->entry.rbe_right = (parent);
          (parent)->entry.rbe_parent = (tmp);
          ;
        } while (0);
        tmp = parent;
        parent = elm;
        elm = tmp;
      }
      do {
        (parent)->entry.rbe_color = 0;
        (gparent)->entry.rbe_color = 1;
      } while (0);
      do {
        (tmp) = (gparent)->entry.rbe_right;
        if (((gparent)->entry.rbe_right = (tmp)->entry.rbe_left)) {
          ((tmp)->entry.rbe_left)->entry.rbe_parent = (gparent);
        };
        if (((tmp)->entry.rbe_parent = (gparent)->entry.rbe_parent)) {
          if ((gparent) == ((gparent)->entry.rbe_parent)->entry.rbe_left)
            ((gparent)->entry.rbe_parent)->entry.rbe_left = (tmp);
          else
            ((gparent)->entry.rbe_parent)->entry.rbe_right = (tmp);
          ;
        } else
          (head)->rbh_root = (tmp);
        (tmp)->entry.rbe_left = (gparent);
        (gparent)->entry.rbe_parent = (tmp);
        ;
      } while (0);
    }
  }
  (head->rbh_root)->entry.rbe_color = 0;
}

static void rbtree_remove_COLOR(struct rbtree *head, struct rbnode *parent,
                            struct rbnode *elm) {
  struct rbnode *tmp;
  while ((elm == NULL || (elm)->entry.rbe_color == 0) &&
         elm != (head)->rbh_root) {
    if ((parent)->entry.rbe_left == elm) {
      tmp = (parent)->entry.rbe_right;
      if ((tmp)->entry.rbe_color == 1) {
        do {
          (tmp)->entry.rbe_color = 0;
          (parent)->entry.rbe_color = 1;
        } while (0);
        do {
          (tmp) = (parent)->entry.rbe_right;
          if (((parent)->entry.rbe_right = (tmp)->entry.rbe_left)) {
            ((tmp)->entry.rbe_left)->entry.rbe_parent = (parent);
          };
          if (((tmp)->entry.rbe_parent = (parent)->entry.rbe_parent)) {
            if ((parent) == ((parent)->entry.rbe_parent)->entry.rbe_left)
              ((parent)->entry.rbe_parent)->entry.rbe_left = (tmp);
            else
              ((parent)->entry.rbe_parent)->entry.rbe_right = (tmp);
            ;
          } else
            (head)->rbh_root = (tmp);
          (tmp)->entry.rbe_left = (parent);
          (parent)->entry.rbe_parent = (tmp);
          ;
        } while (0);
        tmp = (parent)->entry.rbe_right;
      }
      if (((tmp)->entry.rbe_left == NULL ||
           ((tmp)->entry.rbe_left)->entry.rbe_color == 0) &&
          ((tmp)->entry.rbe_right == NULL ||
           ((tmp)->entry.rbe_right)->entry.rbe_color == 0)) {
        (tmp)->entry.rbe_color = 1;
        elm = parent;
        parent = (elm)->entry.rbe_parent;
      } else {
        if ((tmp)->entry.rbe_right == NULL ||
            ((tmp)->entry.rbe_right)->entry.rbe_color == 0) {
          struct rbnode *oleft;
          if ((oleft = (tmp)->entry.rbe_left))
            (oleft)->entry.rbe_color = 0;
          (tmp)->entry.rbe_color = 1;
          do {
            (oleft) = (tmp)->entry.rbe_left;
            if (((tmp)->entry.rbe_left = (oleft)->entry.rbe_right)) {
              ((oleft)->entry.rbe_right)->entry.rbe_parent = (tmp);
            };
            if (((oleft)->entry.rbe_parent = (tmp)->entry.rbe_parent)) {
              if ((tmp) == ((tmp)->entry.rbe_parent)->entry.rbe_left)
                ((tmp)->entry.rbe_parent)->entry.rbe_left = (oleft);
              else
                ((tmp)->entry.rbe_parent)->entry.rbe_right = (oleft);
              ;
            } else
              (head)->rbh_root = (oleft);
            (oleft)->entry.rbe_right = (tmp);
            (tmp)->entry.rbe_parent = (oleft);
            ;
          } while (0);
          tmp = (parent)->entry.rbe_right;
        }
        (tmp)->entry.rbe_color = (parent)->entry.rbe_color;
        (parent)->entry.rbe_color = 0;
        if ((tmp)->entry.rbe_right)
          ((tmp)->entry.rbe_right)->entry.rbe_color = 0;
        do {
          (tmp) = (parent)->entry.rbe_right;
          if (((parent)->entry.rbe_right = (tmp)->entry.rbe_left)) {
            ((tmp)->entry.rbe_left)->entry.rbe_parent = (parent);
          };
          if (((tmp)->entry.rbe_parent = (parent)->entry.rbe_parent)) {
            if ((parent) == ((parent)->entry.rbe_parent)->entry.rbe_left)
              ((parent)->entry.rbe_parent)->entry.rbe_left = (tmp);
            else
              ((parent)->entry.rbe_parent)->entry.rbe_right = (tmp);
            ;
          } else
            (head)->rbh_root = (tmp);
          (tmp)->entry.rbe_left = (parent);
          (parent)->entry.rbe_parent = (tmp);
          ;
        } while (0);
        elm = (head)->rbh_root;
        break;
      }
    } else {
      tmp = (parent)->entry.rbe_left;
      if ((tmp)->entry.rbe_color == 1) {
        do {
          (tmp)->entry.rbe_color = 0;
          (parent)->entry.rbe_color = 1;
        } while (0);
        do {
          (tmp) = (parent)->entry.rbe_left;
          if (((parent)->entry.rbe_left = (tmp)->entry.rbe_right)) {
            ((tmp)->entry.rbe_right)->entry.rbe_parent = (parent);
          };
          if (((tmp)->entry.rbe_parent = (parent)->entry.rbe_parent)) {
            if ((parent) == ((parent)->entry.rbe_parent)->entry.rbe_left)
              ((parent)->entry.rbe_parent)->entry.rbe_left = (tmp);
            else
              ((parent)->entry.rbe_parent)->entry.rbe_right = (tmp);
            ;
          } else
            (head)->rbh_root = (tmp);
          (tmp)->entry.rbe_right = (parent);
          (parent)->entry.rbe_parent = (tmp);
          ;
        } while (0);
        tmp = (parent)->entry.rbe_left;
      }
      if (((tmp)->entry.rbe_left == NULL ||
           ((tmp)->entry.rbe_left)->entry.rbe_color == 0) &&
          ((tmp)->entry.rbe_right == NULL ||
           ((tmp)->entry.rbe_right)->entry.rbe_color == 0)) {
        (tmp)->entry.rbe_color = 1;
        elm = parent;
        parent = (elm)->entry.rbe_parent;
      } else {
        if ((tmp)->entry.rbe_left == NULL ||
            ((tmp)->entry.rbe_left)->entry.rbe_color == 0) {
          struct rbnode *oright;
          if ((oright = (tmp)->entry.rbe_right))
            (oright)->entry.rbe_color = 0;
          (tmp)->entry.rbe_color = 1;
          do {
            (oright) = (tmp)->entry.rbe_right;
            if (((tmp)->entry.rbe_right = (oright)->entry.rbe_left)) {
              ((oright)->entry.rbe_left)->entry.rbe_parent = (tmp);
            };
            if (((oright)->entry.rbe_parent = (tmp)->entry.rbe_parent)) {
              if ((tmp) == ((tmp)->entry.rbe_parent)->entry.rbe_left)
                ((tmp)->entry.rbe_parent)->entry.rbe_left = (oright);
              else
                ((tmp)->entry.rbe_parent)->entry.rbe_right = (oright);
              ;
            } else
              (head)->rbh_root = (oright);
            (oright)->entry.rbe_left = (tmp);
            (tmp)->entry.rbe_parent = (oright);
            ;
          } while (0);
          tmp = (parent)->entry.rbe_left;
        }
        (tmp)->entry.rbe_color = (parent)->entry.rbe_color;
        (parent)->entry.rbe_color = 0;
        if ((tmp)->entry.rbe_left)
          ((tmp)->entry.rbe_left)->entry.rbe_color = 0;
        do {
          (tmp) = (parent)->entry.rbe_left;
          if (((parent)->entry.rbe_left = (tmp)->entry.rbe_right)) {
            ((tmp)->entry.rbe_right)->entry.rbe_parent = (parent);
          };
          if (((tmp)->entry.rbe_parent = (parent)->entry.rbe_parent)) {
            if ((parent) == ((parent)->entry.rbe_parent)->entry.rbe_left)
              ((parent)->entry.rbe_parent)->entry.rbe_left = (tmp);
            else
              ((parent)->entry.rbe_parent)->entry.rbe_right = (tmp);
            ;
          } else
            (head)->rbh_root = (tmp);
          (tmp)->entry.rbe_right = (parent);
          (parent)->entry.rbe_parent = (tmp);
          ;
        } while (0);
        elm = (head)->rbh_root;
        break;
      }
    }
  }
  if (elm)
    (elm)->entry.rbe_color = 0;
}

void rbtree_remove(struct rbtree *head, void* elmv) {
  struct rbnode *elm = elmv;
  struct rbnode *child, *parent;
  int color;
  if ((elm)->entry.rbe_left == NULL)
    child = (elm)->entry.rbe_right;
  else if ((elm)->entry.rbe_right == NULL)
    child = (elm)->entry.rbe_left;
  else {
    struct rbnode *old = elm, *left;
    elm = (elm)->entry.rbe_right;
    while ((left = (elm)->entry.rbe_left))
      elm = left;
    child = (elm)->entry.rbe_right;
    parent = (elm)->entry.rbe_parent;
    color = (elm)->entry.rbe_color;
    if (child)
      (child)->entry.rbe_parent = parent;
    if (parent) {
      if ((parent)->entry.rbe_left == elm)
        (parent)->entry.rbe_left = child;
      else
        (parent)->entry.rbe_right = child;
      ;
    } else
      (head)->rbh_root = child;
    if ((elm)->entry.rbe_parent == old)
      parent = elm;
    (elm)->entry = (old)->entry;
    if ((old)->entry.rbe_parent) {
      if (((old)->entry.rbe_parent)->entry.rbe_left == old)
        ((old)->entry.rbe_parent)->entry.rbe_left = elm;
      else
        ((old)->entry.rbe_parent)->entry.rbe_right = elm;
      ;
    } else
      (head)->rbh_root = elm;
    ((old)->entry.rbe_left)->entry.rbe_parent = elm;
    if ((old)->entry.rbe_right)
      ((old)->entry.rbe_right)->entry.rbe_parent = elm;
    if (parent) {
      left = parent;
      do {
        ;
      } while ((left = (left)->entry.rbe_parent));
    }
    goto color;
  }
  parent = (elm)->entry.rbe_parent;
  color = (elm)->entry.rbe_color;
  if (child)
    (child)->entry.rbe_parent = parent;
  if (parent) {
    if ((parent)->entry.rbe_left == elm)
      (parent)->entry.rbe_left = child;
    else
      (parent)->entry.rbe_right = child;
    ;
  } else
    (head)->rbh_root = child;
color:
  if (color == 0)
    rbtree_remove_COLOR(head, parent, child);
}

void* rbtree_insert(struct rbtree *head, void *elmv) {
  struct rbnode *elm = elmv;
  struct rbnode *tmp;
  struct rbnode *parent = NULL;
  int comp = 0;
  tmp = (head)->rbh_root;
  while (tmp) {
    parent = tmp;
    comp = (head->cmp)((void*)elm->content, (void*)parent->content);
    if (comp < 0)
      tmp = (tmp)->entry.rbe_left;
    else if (comp > 0)
      tmp = (tmp)->entry.rbe_right;
    else
      return (tmp);
  }
  do {
    (elm)->entry.rbe_parent = parent;
    (elm)->entry.rbe_left = (elm)->entry.rbe_right = NULL;
    (elm)->entry.rbe_color = 1;
  } while (0);
  if (parent != NULL) {
    if (comp < 0)
      (parent)->entry.rbe_left = elm;
    else
      (parent)->entry.rbe_right = elm;
    ;
  } else
    (head)->rbh_root = elm;
  rbtree_insert_COLOR(head, elm);
  return (NULL);
}

void* rbtree_find(struct rbtree *head, void *key) {
  struct rbnode *tmp = (head)->rbh_root;
  int comp;
  while (tmp) {
    comp = (head->cmp)(key, (void*)tmp->content);
    if (comp < 0)
      tmp = (tmp)->entry.rbe_left;
    else if (comp > 0)
      tmp = (tmp)->entry.rbe_right;
    else
      return (tmp);
  }
  return (NULL);
}

void* rbtree_next(struct rbtree *head, void *elmv) {
  struct rbnode *elm = elmv;
  if ((elm)->entry.rbe_right) {
    elm = (elm)->entry.rbe_right;
    while ((elm)->entry.rbe_left)
      elm = (elm)->entry.rbe_left;
  } else {
    if ((elm)->entry.rbe_parent &&
        (elm == ((elm)->entry.rbe_parent)->entry.rbe_left))
      elm = (elm)->entry.rbe_parent;
    else {
      while ((elm)->entry.rbe_parent &&
             (elm == ((elm)->entry.rbe_parent)->entry.rbe_right))
        elm = (elm)->entry.rbe_parent;
      elm = (elm)->entry.rbe_parent;
    }
  }
  return (elm);
}

static struct rbnode *rbtree_RB_MINMAX(struct rbtree *head, int val) {
  struct rbnode *tmp = (head)->rbh_root;
  struct rbnode *parent = NULL;
  while (tmp) {
    parent = tmp;
    if (val < 0)
      tmp = (tmp)->entry.rbe_left;
    else
      tmp = (tmp)->entry.rbe_right;
  }
  return (parent);
};

static void rbtree_free_impl(struct rbnode *node, void (*free_cb)(void*)) {
  if (node == NULL) return;
  if (free_cb != NULL) free_cb(node->content);
  rbtree_free_impl(node->entry.rbe_left, free_cb);
  rbtree_free_impl(node->entry.rbe_right, free_cb);
  free(node);
}

void rbtree_free(struct rbtree *head, void (*free_cb)(void*)) {
  rbtree_free_impl(head->rbh_root, free_cb);  
}
