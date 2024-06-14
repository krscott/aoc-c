#include "str.h"
#include "string.h"

static void test_strbuf_from_owned_cstr(void) {
    char *cstr = strdup("Hello");

    struct strbuf sb = strbuf_from_owned_cstr(cstr);
    assert(sb.ptr == cstr);
    assert((size_t)sb.len == strlen(cstr));

    strbuf_deinit(&sb);
}

int main(void) { test_strbuf_from_owned_cstr(); }
