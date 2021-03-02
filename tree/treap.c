#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CHILD(node, dir) (node)->children[dir]
#define SUBTREE_SIZE(node) (node)->subtree_size

typedef struct treap treap_t;
typedef struct treap_node treap_node_t;

struct treap_node {
  treap_node_t *children[2];
  int key, priority;
  size_t subtree_size;
};

struct treap {
  treap_node_t *root;
  unsigned seed;
};

static treap_node_t NIL = {{NULL, NULL}, 0, 0, 0};

/*
 * Left rotate (dir = 0) or right rotate (dir = 1)
 * */
static void treap_rotate(treap_node_t **node_ptr, int dir) {
  treap_node_t *p = *node_ptr;
  treap_node_t *z = CHILD(p, !dir);
  CHILD(p, !dir)  = CHILD(z, dir);
  CHILD(z, dir)   = p;
  SUBTREE_SIZE(p) = SUBTREE_SIZE(CHILD(p, 0)) + SUBTREE_SIZE(CHILD(p, 1)) + 1;
  SUBTREE_SIZE(z) = SUBTREE_SIZE(CHILD(z, 0)) + SUBTREE_SIZE(CHILD(z, 1)) + 1;
  *node_ptr       = z;
}

static void treap_node_insert(treap_node_t **node_ptr, treap_node_t *z) {
  treap_node_t *p = *node_ptr;
  if (p == &NIL) {
    *node_ptr = z;
  } else {
    /* 0 for left and 1 for right */
    int dir = z->key >= p->key;
    treap_node_insert(&CHILD(p, dir), z);
    if (p->priority < CHILD(p, dir)->priority) {
      treap_rotate(node_ptr, !dir);
    } else {
      SUBTREE_SIZE(p)++;
    }
  }
}

treap_t *treap_new(void) {
  treap_t *treap = (treap_t *)malloc(sizeof(treap_t));
  if (treap) {
    treap->root = &NIL;
    treap->seed = 47;
  }
  return treap;
}

void treap_insert(treap_t *treap, int key) {
  treap_node_t *z = (treap_node_t *)malloc(sizeof(treap_node_t));
  CHILD(z, 0)     = &NIL;
  CHILD(z, 1)     = &NIL;
  z->key          = key;
  z->priority     = rand_r(&treap->seed);
  z->subtree_size = 1;
  treap_node_insert(&treap->root, z);
}

void treap_node_remove(treap_node_t **node_ptr, int key) {
  treap_node_t *x = *node_ptr;
  if (x == &NIL) {
    return;
  }

  if (x->key == key) {
    if (CHILD(x, 0) == &NIL) {
      *node_ptr = CHILD(x, 1);
      free(x);
    } else if (CHILD(x, 1) == &NIL) {
      *node_ptr = CHILD(x, 0);
      free(x);
    } else {
      int dir = CHILD(x, 0)->priority > CHILD(x, 1)->priority;
      treap_rotate(node_ptr, dir);
      treap_node_remove(&CHILD(*node_ptr, dir), key);
    }
  } else if (key < x->key) {
    treap_node_remove(&CHILD(x, 0), key);
  } else {
    treap_node_remove(&CHILD(x, 1), key);
  }

  if ((x = *node_ptr) != &NIL) {
    SUBTREE_SIZE(x) = SUBTREE_SIZE(CHILD(x, 0)) + SUBTREE_SIZE(CHILD(x, 1)) + 1;
  }
}

void treap_remove(treap_t *treap, int key) {
  treap_node_remove(&treap->root, key);
}

size_t treap_rank(treap_t *treap, int key) {
  size_t r        = 0;
  treap_node_t *x = treap->root;
  while (x != &NIL) {
    if (key < x->key) {
      x = CHILD(x, 0);
    } else {
      r += SUBTREE_SIZE(CHILD(x, 0)) + 1;
      x = CHILD(x, 1);
    }
  }
  return r;
}

int main(void) {
  treap_t *treap = treap_new();

  treap_insert(treap, 6);
  treap_insert(treap, 2);
  treap_insert(treap, 4);
  treap_insert(treap, 9);
  treap_insert(treap, 7);
  treap_insert(treap, 10);

  printf("rank(9)=%zd\n", treap_rank(treap, 9));

  return 0;
}