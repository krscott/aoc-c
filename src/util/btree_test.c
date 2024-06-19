#include "btree.h"

#include <string.h>

struct testnode {
    char const *key;
    int value;
};
btree_define_struct(testtree, struct testnode);

static size_t const testtree_degree = 4;

static int testtree_compare(void const *a, void const *b, void *user_data) {
    (void)user_data;
    return strcmp(a, b);
}

void test_btree(void) {
    enum err e = OK;
    struct testtree t = {0};
    e = btree_insert(testtree, &t, (struct testnode){.key = "foo", .value = 1});
    assert(e == OK);

    struct testnode n;
    e = btree_get(testtree, &n, &t, "foo");
    assert(e == OK);
    assert(0 == strcmp("foo", n.key));
    assert(n.value == 1);

    btree_deinit(testtree, &t);
}

int main(void) { test_btree(); }
