#include "btree.h"

btree_define_struct(btree__anytree, void *);

ERRFN btree__pre_insert(struct btree__anytree *const tree, size_t const degree) {
    assert(tree);
    assert(degree > 0);
    (void)tree;
    (void)degree;
    return OK;
}

ERRFN btree__post_insert(
    struct btree__anytree *const tree,
    size_t const degree,
    btree_compare_fn_type const compare
) {
    assert(tree);
    assert(degree > 0);
    (void)*tree;
    (void)degree;
    (void)compare;
    return OK;
}

ERRFN btree__get(
    void *const node,
    struct btree__anytree const *const tree,
    size_t const degree,
    void const *const key,
    btree_compare_fn_type const compare
) {
    assert(tree);
    assert(degree > 0);
    (void)node;
    (void)tree;
    (void)degree;
    (void)key;
    (void)compare;
    return ERR_NONE;
}

void btree__deinit(struct btree__anytree *const tree, size_t const degree) {
    assert(tree);
    (void)tree;
    (void)degree;
}
