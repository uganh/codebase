#include <assert.h>
#include <stddef.h>

#define CHILD(node, dir) (node)->children[dir]

#define LFT_CHILD(node) CHILD(node, 0)
#define RGT_CHILD(node) CHILD(node, 1)

#define SUBTREE_SIZE(node) (node)->size

#define maintain(node)                                                         \
  do {                                                                         \
    SUBTREE_SIZE(node) =                                                       \
      SUBTREE_SIZE(LFT_CHILD(node)) + SUBTREE_SIZE(RGT_CHILD(node)) + 1;       \
  } while (0)

typedef struct tree tree_t;

struct tree {
  tree_t *children[2];
  size_t size;
};

/* Size = 0 */
static tree_t NIL;

static void tree_rotate(tree_t **node_ptr, int dir) {
  tree_t *p = *node_ptr;
  tree_t *z = CHILD(p, !dir);
  assert(z != &NIL && "Invalid rotate");
  CHILD(p, !dir) = CHILD(z, dir);
  CHILD(z, dir)  = p;
  maintain(p);
  maintain(z);
  *node_ptr = z;
}

/*
 * Find the k-th (from 1) node and splay it to root
 * */
void splay(tree_t **node_ptr, size_t k) {
  tree_t *g = *node_ptr;
  assert(0 < k && k <= SUBTREE_SIZE(g));

  size_t left_size = SUBTREE_SIZE(LFT_CHILD(g)) + 1;
  if (k == left_size) {
    return;
  }

  int dir;
  if (k > left_size) {
    k -= left_size;
    dir = 1;
  } else {
    dir = 0;
  }

  tree_t *p = CHILD(g, dir);
  left_size = SUBTREE_SIZE(LFT_CHILD(p)) + 1;

  if (left_size != k) {
    int dir2;
    if (k > left_size) {
      k -= left_size;
      dir2 = 1;
    } else {
      dir2 = 0;
    }

    splay(&CHILD(p, dir2), k);

    if (dir == dir2) {
      /*
       * Case 2:
       *
       *     g
       *    /
       *   p
       *  /
       * z
       * */
      tree_rotate(node_ptr, !dir);
    } else {
      /*
       * Case 3:
       *
       *   g
       *  /
       * p
       *  \
       *   z
       * */
      tree_rotate(&CHILD(g, dir), dir);
    }
  }

  /*
   * Case 1:
   *
   *   g ()
   *  /
   * z
   * */
  tree_rotate(node_ptr, !dir);
}

void merge(tree_t **dst_ptr, tree_t *src) {
  splay(dst_ptr, (*dst_ptr)->size);
  tree_t *dst = *dst_ptr;
  RGT_CHILD(dst) = src;
  maintain(dst);
}

tree_t *split(tree_t **ori_ptr, size_t k) {
  splay(ori_ptr, k);
  tree_t *ori = *ori_ptr;
  tree_t *drv = RGT_CHILD(ori);
  RGT_CHILD(ori) = &NIL;
  return drv;
}

int main(void) {

  return 0;
}