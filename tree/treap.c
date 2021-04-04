#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CHILD(node, dir) (node)->children[dir]
#define SUBTREE_SIZE(node) (node)->subtree_size
#define TREAP_MAINTAIN(p) \
  SUBTREE_SIZE(p) = SUBTREE_SIZE(CHILD(p, 0)) + SUBTREE_SIZE(CHILD(p, 1)) + 1

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
  CHILD(p, !dir) = CHILD(z, dir);
  CHILD(z, dir) = p;
  TREAP_MAINTAIN(p);
  TREAP_MAINTAIN(z);
  // SUBTREE_SIZE(p) = SUBTREE_SIZE(CHILD(p, 0)) + SUBTREE_SIZE(CHILD(p, 1)) +
  // 1; SUBTREE_SIZE(z) = SUBTREE_SIZE(CHILD(z, 0)) + SUBTREE_SIZE(CHILD(z, 1))
  // + 1;
  *node_ptr = z;
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

static void treap_node_remove(treap_node_t **node_ptr, int key) {
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
    TREAP_MAINTAIN(x);
    // SUBTREE_SIZE(x) = SUBTREE_SIZE(CHILD(x, 0)) + SUBTREE_SIZE(CHILD(x, 1)) +
    // 1;
  }
}

static void treap_node_split(treap_node_t *node, int key,
                             treap_node_t **lhs_ptr, treap_node_t **rhs_ptr) {
  if (node == &NIL) {
    *lhs_ptr = *rhs_ptr = &NIL;
  } else {
    treap_node_t *lhs, *rhs;
    if (key < node->key) {
      treap_node_split(CHILD(node, 0), key, &lhs, &rhs);
      CHILD(node, 0) = rhs;
      TREAP_MAINTAIN(node);
      *lhs_ptr = lhs;
      *rhs_ptr = node;
    } else {
      treap_node_split(CHILD(node, 1), key, &lhs, &rhs);
      CHILD(node, 1) = lhs;
      TREAP_MAINTAIN(node);
      *lhs_ptr = node;
      *rhs_ptr = rhs;
    }
  }
}

static treap_node_t *treap_node_merge(treap_node_t *lhs, treap_node_t *rhs) {
  if (lhs == &NIL) {
    return rhs;
  }
  if (rhs == &NIL) {
    return lhs;
  }
  if (lhs->priority > rhs->priority) {
    CHILD(lhs, 1) = treap_node_merge(CHILD(lhs, 1), rhs);
    TREAP_MAINTAIN(lhs);
    return lhs;
  } else {
    CHILD(rhs, 0) = treap_node_merge(lhs, CHILD(rhs, 0));
    TREAP_MAINTAIN(rhs);
    return rhs;
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
  assert(z && "Allocate failed");
  CHILD(z, 0) = &NIL;
  CHILD(z, 1) = &NIL;
  z->key = key;
  z->priority = rand_r(&treap->seed);
  z->subtree_size = 1;

#ifdef WITH_ROTATE
  treap_node_insert(&treap->root, z);
#else
  treap_node_t *lhs, *rhs;
  treap_node_split(treap->root, key, &lhs, &rhs);
  lhs = treap_node_merge(lhs, z);
  treap->root = treap_node_merge(lhs, rhs);
#endif
}

void treap_remove(treap_t *treap, int key) {
  treap_node_remove(&treap->root, key);
}

size_t treap_rank(treap_t *treap, int key) {
  size_t r = 0;
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