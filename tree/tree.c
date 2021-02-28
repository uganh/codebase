#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define TREE_SUCCESS 0
#define TREE_FAILURE -1

/*
 * Binary Search Tree
 * */

#define TREE_COMMON                                                            \
  long key;                                                                    \
  void *value;                                                                 \
  tree_t *parent;                                                              \
  tree_t *children[2]

#define CHILD(node, dir) ((tree_t *)(node))->children[dir]

#define LFT_CHILD(node) CHILD(node, 0)
#define RGT_CHILD(node) CHILD(node, 1)

/*
 * 0 for left and 1 for right
 * */
#define DIRECTION(node) (RGT_CHILD(((tree_t *)(node))->parent) == (node))

#define RB_RED 0
#define RB_BLACK 1
/*
 * NIL is always black
 * */
#define RB_COLOR(node) ((node) ? (((rbtree_t *)node)->color) : RB_BLACK)
/*
 * R-value macro
 * */
#define XRB_COLOR(node) ((rbtree_t *)(node))->color

typedef struct tree tree_t;

struct tree {
  TREE_COMMON;
};

tree_t *tree_new(long key, void *value) {
  tree_t *node = (tree_t *)malloc(sizeof(tree_t));
  /* Initialize */
  node->key       = key;
  node->value     = value;
  node->parent    = NULL;
  LFT_CHILD(node) = NULL;
  RGT_CHILD(node) = NULL;
  return node;
}

void tree_free(tree_t **root_ptr) {
  if (root_ptr) {
    tree_t *x = *root_ptr;
    if (x) {
      tree_free(&LFT_CHILD(x));
      tree_free(&RGT_CHILD(x));
      free(x);
      *root_ptr = NULL;
    }
  }
}

tree_t *tree_min(tree_t *node) {
  if (node) {
    while (LFT_CHILD(node)) {
      node = LFT_CHILD(node);
    }
  }
  return node;
}

tree_t *tree_max(tree_t *node) {
  if (node) {
    while (RGT_CHILD(node)) {
      node = RGT_CHILD(node);
    }
  }
  return node;
}

void tree_insert(tree_t **root_ptr, tree_t *z) {
  if (!root_ptr || !z) {
    return;
  }

  // Invariant: y is the parent of x
  tree_t *y = NULL, *x = *root_ptr;
  while (x) {
    y = x;
    if (z->key < x->key) {
      x = LFT_CHILD(x);
    } else {
      x = RGT_CHILD(x);
    }
  }

  /* y is LEAF now */
  z->parent = y;
  if (!y) {
    *root_ptr = z;
  } else if (z->key < y->key) {
    LFT_CHILD(y) = z;
  } else {
    RGT_CHILD(y) = z;
  }
}

/*
 * Replace subtree u by v
 * */
static void tree_transplant(tree_t **root_ptr, tree_t *u, tree_t *v) {
  if (!u->parent) {
    *root_ptr = v;
  } else if (LFT_CHILD(u->parent) == u) {
    LFT_CHILD(u->parent) = v;
  } else if (RGT_CHILD(u->parent) == u) {
    RGT_CHILD(u->parent) = v;
  } else {
    assert(0 && "Bad tree structure");
  }

  if (v) {
    v->parent = u->parent;
  }
}

void tree_delete(tree_t **root_ptr, tree_t *z) {
  if (!LFT_CHILD(z)) {
    tree_transplant(root_ptr, z, RGT_CHILD(z));
  } else if (!RGT_CHILD(z)) {
    tree_transplant(root_ptr, z, LFT_CHILD(z));
  } else {
    // y is the successor of z
    // y has no left child
    tree_t *y = tree_min(RGT_CHILD(z));
    if (y->parent != z) {
      tree_transplant(root_ptr, y, RGT_CHILD(y));
      RGT_CHILD(y)         = RGT_CHILD(z);
      RGT_CHILD(z)->parent = y;
    }
    tree_transplant(root_ptr, z, y);
    LFT_CHILD(y)         = LFT_CHILD(z);
    LFT_CHILD(y)->parent = y;
  }
  /* It's the caller's responsibility to free z */
}

void tree_walk(tree_t *node) {
  if (node != NULL) {
    tree_walk(LFT_CHILD(node));
    // Do things
    tree_walk(RGT_CHILD(node));
  }
}

tree_t *tree_search(tree_t *node, long key) {
  while (node != NULL && key != node->key) {
    if (key < node->key) {
      node = LFT_CHILD(node);
    } else {
      node = RGT_CHILD(node);
    }
  }
  return node;
}

tree_t *tree_succ(tree_t *node) {
  if (node) {
    if (RGT_CHILD(node)) {
      return tree_min(RGT_CHILD(node));
    }
    tree_t *parent;
    while ((parent = node->parent) && node == RGT_CHILD(parent)) {
      node = parent;
    }
    return parent;
  }
}

void tree_print(FILE *file, tree_t *node, unsigned indent) {
  if (node) {
    for (unsigned i = 0; i < indent; ++i) {
      fprintf(file, " ");
    }
    if (!node->parent) {
      fprintf(file, "* ");
    } else if (LFT_CHILD(node->parent) == node) {
      fprintf(file, "+ ");
    } else if (RGT_CHILD(node->parent) == node) {
      fprintf(file, "- ");
    } else {
      assert(0 && "Bad tree structure");
    }
    fprintf(file, "%ld\n", node->key);
    tree_print(file, LFT_CHILD(node), indent + 1);
    tree_print(file, RGT_CHILD(node), indent + 1);
  }
}

/*
 * Left (dir = 0) or right (dir = 1) rotate
 *
 *     y             x
 *    / \           / \
 *   x   c   <=>   a   y
 *  / \               / \
 * a   b             b   c
 * */
void tree_rotate(tree_t **root_ptr, tree_t *x, int dir) {
  assert(dir == 0 || dir == 1);

  /* After rotate, y is parent of x */
  tree_t *y = CHILD(x, !dir);
  assert(y && "Invalid rotate");

  CHILD(x, !dir) = CHILD(y, dir);
  if (CHILD(y, dir)) {
    CHILD(y, dir)->parent = x;
  }

  y->parent = x->parent;
  if (!x->parent) {
    *root_ptr = y;
  } else if (LFT_CHILD(x->parent) == x) {
    LFT_CHILD(x->parent) = y;
  } else if (RGT_CHILD(x->parent) == x) {
    RGT_CHILD(x->parent) = y;
  } else {
    assert(0 && "Bad tree structure");
  }

  CHILD(y, dir) = x;
  x->parent     = y;
}

/*
 * Red-black properties:
 *
 * 1. Every node is either red or black
 * 2. The root is black
 * 3. Every leaf (NIL) is black
 * 4. If a node is red, then both its children are black
 * 5. For each node all simple paths from the node to descendant leaves
 *    contain the same number of black nodes
 * */

typedef struct {
  TREE_COMMON;
  int color;
} rbtree_t;

tree_t *rbtree_new(long key, void *value) {
  rbtree_t *node = (rbtree_t *)malloc(sizeof(rbtree_t));
  /* Initialize */
  node->key       = key;
  node->value     = value;
  node->parent    = NULL;
  LFT_CHILD(node) = NULL;
  RGT_CHILD(node) = NULL;
  XRB_COLOR(node) = RB_RED;
  return (tree_t *)node;
}

static unsigned rbtree_verify(tree_t *node) {
  if (node) {
    /* Structure verifing */
    assert(!LFT_CHILD(node) || LFT_CHILD(node)->parent == node);
    assert(!RGT_CHILD(node) || RGT_CHILD(node)->parent == node);

    /* Property 1 */
    assert(RB_COLOR(node) == RB_BLACK || RB_COLOR(node) == RB_RED);
    if (node->parent == NULL) {
      /* Root, property 2 */
      assert(RB_COLOR(node) == RB_BLACK);
    }

    if (RB_COLOR(node) == RB_RED) {
      /* Property 4 */
      assert(RB_COLOR(LFT_CHILD(node)) == RB_BLACK);
      assert(RB_COLOR(RGT_CHILD(node)) == RB_BLACK);
    }

    /* Property 5 */
    unsigned bh = rbtree_verify(LFT_CHILD(node));
    assert(bh == rbtree_verify(RGT_CHILD(node)));

    return bh + (RB_COLOR(node) == RB_BLACK);
  }

  return 0;
}

void rbtree_insert(tree_t **root_ptr, tree_t *z) {
  tree_insert(root_ptr, z);

  /* After insertion, either property 2 or property 4 may be violated */

  while (RB_COLOR(z->parent) == RB_RED) {
    /*
     * Invariants:
     *
     * a. Node z is red
     * b. If z.p is the root, then z.p is black
     * c. If the tree violates any of the red-black properties, then it violates
     *    at most one of them, and the violation is of either property 2 or
     *    property 4
     * */

    // Grandparent exists
    int dir   = DIRECTION(z->parent);
    tree_t *u = CHILD(z->parent->parent, !dir);

    if (RB_COLOR(u) == RB_RED) {
      /*
       * Case 1:
       *
       *     g@B
       *     / \
       *   p@R u@R
       *   /
       * z@R
       * */
      XRB_COLOR(u) = XRB_COLOR(z->parent) = RB_BLACK;
      XRB_COLOR(z->parent->parent)        = RB_RED;
      z                                   = z->parent->parent;
    } else {
      if (DIRECTION(z) != dir) {
        /*
         * Case 2:
         *
         *   g@B
         *   / \
         * p@R u@B
         *   \
         *   z@R
         * */
        z = z->parent;
        tree_rotate(root_ptr, z, dir);
      }
      /*
       * Case 3:
       *
       *     g@B
       *     / \
       *   p@R u@B
       *   /
       * z@R
       * */
      XRB_COLOR(z->parent)         = RB_BLACK;
      XRB_COLOR(z->parent->parent) = RB_RED;
      tree_rotate(root_ptr, z->parent->parent, !dir);
    }
  }
  XRB_COLOR(*root_ptr) = RB_BLACK;
}

void rbtree_delete(tree_t **root_ptr, tree_t *z) {
  /* 1. Maintain node y as the node either removed from the tree
   *    or moved within the tree
   * 2. Keep track of the node x that moves into node y's original position
   * */
  tree_t *y = z;
  tree_t *x, *x_parent = y;
  int y_original_color = RB_COLOR(y);

  if (!LFT_CHILD(z)) {
    x = RGT_CHILD(z);
    tree_transplant(root_ptr, z, x);
  } else if (!RGT_CHILD(z)) {
    x = LFT_CHILD(z);
    tree_transplant(root_ptr, z, x);
  } else {
    /* Node y is the successor of z */
    y                = tree_min(RGT_CHILD(z));
    y_original_color = RB_COLOR(y);
    x                = RGT_CHILD(y);
    if (y->parent == z) {
      x_parent = y;
    } else {
      tree_transplant(root_ptr, y, x);
      RGT_CHILD(y)         = RGT_CHILD(z);
      RGT_CHILD(y)->parent = y;
      x_parent             = y;
    }
    tree_transplant(root_ptr, z, y);
    // Node y have no left child
    LFT_CHILD(y)         = LFT_CHILD(z);
    LFT_CHILD(y)->parent = y;
    XRB_COLOR(y)         = XRB_COLOR(z);
  }

  if (y_original_color == RB_RED) {
    /*
     * The red-black properties still hold when y is removed or moved
     *
     * 1. No black-heights in the tree have changed
     * 2. No red nodes have been made adjacent
     * 3. Since y could not have been the root if it was red, the root remains
     *    black
     * */
    return;
  }

  /* WARNING: x can be NULL, use x_parent instead x->parent */

  /* Now, node x is either "doubly black" or "red-and-black",
   * properties 1, 2, 4 may be violated */

  while (x != *root_ptr && RB_COLOR(x) == RB_BLACK) {
    /* Node x is doubly black */
    /* 0 for left and 1 for right */
    int dir = RGT_CHILD(x_parent) == x;
    /* Since node x is doubly black, sibling node s must exists (not NIL) */
    tree_t *s = CHILD(x_parent, !dir);

    if (RB_COLOR(s) == RB_RED) {
      /*
       * Case 1:
       *
       *    p@B
       *    / \
       *  x@B s@R
       *      / \
       *    n@B n'@B
       * */
      XRB_COLOR(s)        = RB_BLACK;
      XRB_COLOR(x_parent) = RB_RED;
      tree_rotate(root_ptr, x_parent, dir);
      s = CHILD(x_parent, !dir);
    }

    if (
      RB_COLOR(LFT_CHILD(s)) == RB_BLACK &&
      RB_COLOR(RGT_CHILD(s)) == RB_BLACK) {
      /*
       * Case 2:
       *
       *   p@?
       *   / \
       * x@B s@B
       *     / \
       *   n@B n'@B
       * */
      XRB_COLOR(s) = RB_RED;
      x            = x_parent;
      /* Always keep track of the parent of x */
      x_parent = x->parent;
    } else {
      if (RB_COLOR(CHILD(s, !dir)) == RB_BLACK) {
        /*
         * Case 3:
         *
         *   p@?
         *   / \
         * x@B s@B
         *     / \
         *   n@R n'@B
         * */
        XRB_COLOR(s)             = RB_RED;
        XRB_COLOR(CHILD(s, dir)) = RB_BLACK;
        tree_rotate(root_ptr, s, !dir);
        s = CHILD(x_parent, !dir);
      }
      /*
       * Case 4:
       *
       *   p@?
       *   / \
       * x@B s@B
       *     / \
       *   n@? n'@R
       * */
      XRB_COLOR(s)              = RB_COLOR(x_parent);
      XRB_COLOR(x_parent)       = RB_BLACK;
      XRB_COLOR(CHILD(s, !dir)) = RB_BLACK;
      tree_rotate(root_ptr, x_parent, dir);
      x = *root_ptr;
    }
  }

  XRB_COLOR(x) = RB_BLACK;
}

void rbtree_print(FILE *file, tree_t *node, unsigned indent) {
  if (node) {
    for (unsigned i = 0; i < indent; ++i) {
      fprintf(file, " ");
    }
    if (!node->parent) {
      fprintf(file, "* ");
    } else if (LFT_CHILD(node->parent) == node) {
      fprintf(file, "+ ");
    } else if (RGT_CHILD(node->parent) == node) {
      fprintf(file, "- ");
    } else {
      assert(0 && "Bad tree structure");
    }
    fprintf(file, "%ld@%c\n", node->key, (RB_COLOR(node) ? 'B' : 'R'));
    rbtree_print(file, LFT_CHILD(node), indent + 1);
    rbtree_print(file, RGT_CHILD(node), indent + 1);
  }
}

int main(void) {
  tree_t *tree = NULL, *target;

  tree_insert(&tree, tree_new(7, NULL));
  tree_insert(&tree, tree_new(4, NULL));
  tree_insert(&tree, tree_new(11, NULL));
  tree_insert(&tree, tree_new(3, NULL));
  tree_insert(&tree, tree_new(6, NULL));
  tree_insert(&tree, tree_new(9, NULL));
  tree_insert(&tree, target = tree_new(18, NULL));
  tree_insert(&tree, tree_new(2, NULL));
  tree_insert(&tree, tree_new(14, NULL));
  tree_insert(&tree, tree_new(19, NULL));
  tree_insert(&tree, tree_new(12, NULL));
  tree_insert(&tree, tree_new(17, NULL));
  tree_insert(&tree, tree_new(22, NULL));
  tree_insert(&tree, tree_new(20, NULL));

  printf("Tree test:\n");
  /* Figure 13-3 */
  tree_print(stdout, tree, 0);
  tree_rotate(&tree, target, 0);
  tree_print(stdout, tree, 0);

  tree_free(&tree);
  assert(tree == NULL);

  printf("Red-black tree test:\n");

  rbtree_insert(&tree, rbtree_new(11, NULL));
  rbtree_verify(tree);
  rbtree_insert(&tree, rbtree_new(2, NULL));
  rbtree_verify(tree);
  rbtree_insert(&tree, rbtree_new(14, NULL));
  rbtree_verify(tree);
  rbtree_insert(&tree, rbtree_new(1, NULL));
  rbtree_verify(tree);
  rbtree_insert(&tree, rbtree_new(7, NULL));
  rbtree_verify(tree);
  rbtree_insert(&tree, rbtree_new(15, NULL));
  rbtree_verify(tree);
  rbtree_insert(&tree, rbtree_new(5, NULL));
  rbtree_verify(tree);
  rbtree_insert(&tree, rbtree_new(8, NULL));
  rbtree_verify(tree);
  rbtree_insert(&tree, rbtree_new(4, NULL));
  rbtree_verify(tree);
  rbtree_print(stdout, tree, 0);
  tree_free(&tree);

  return 0;
}