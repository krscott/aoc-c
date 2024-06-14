#include "str.h"
#include "string.h"

static void test_strbuf_from_owned_cstr(void) {
    char *cstr = strdup("Hello");

    struct strbuf sb = strbuf_from_owned_cstr(cstr);
    assert(sb.ptr == cstr);
    assert((size_t)sb.len == strlen(cstr));

    strbuf_deinit(&sb);
}

static void test_strbuf_init_copy_str(void) {
    char cstr[] = "Hello";
    struct str const s = str_from_cstr(cstr);

    struct strbuf sb;

    enum err e = strbuf_init_copy_str(&sb, s);
    assert(e == OK);
    assert(sb.ptr != cstr);
    assert(sb.len == strlen(cstr));
    assert(strcmp(sb.ptr, cstr) == 0);

    strbuf_deinit(&sb);
}

static void test_strbuf_push(void) {
    struct strbuf sb;
    enum err e = strbuf_init_copy_cstr(&sb, "foo");
    assert(e == OK);
    assert(strcmp(sb.ptr, "foo") == 0);

    e = strbuf_push(&sb, 'b');
    assert(e == OK);
    assert(strcmp(sb.ptr, "foob") == 0);

    strbuf_deinit(&sb);
}

static void test_strbuf_replace(void) {
    struct strbuf sb;
    enum err e = strbuf_init_copy_cstr(&sb, "Hello, world!");
    assert(e == OK);

    e = strbuf_replace(&sb, "!", "?");
    assert(e == OK);
    assert(strcmp(sb.ptr, "Hello, world?") == 0);

    e = strbuf_replace(&sb, "Hello", "Sup");
    assert(e == OK);
    assert(strcmp(sb.ptr, "Sup, world?") == 0);

    e = strbuf_replace(&sb, "Sup", "How's it going");
    assert(e == OK);
    assert(strcmp(sb.ptr, "How's it going, world?") == 0);

    strbuf_deinit(&sb);
}

static void test_strbuf_replace2(void) {
    struct strbuf sb;
    enum err e = strbuf_init_copy_cstr(&sb, "foo:  a  bc  def  ");
    assert(e == OK);

    for (;;) {
        e = strbuf_replace(&sb, " ", "");
        if (e == ERR_NONE) break;
        assert(e == OK);
    }
    assert(strcmp(sb.ptr, "foo:abcdef") == 0);

    strbuf_deinit(&sb);
}

static void test_strbuf_replace_all(void) {
    struct strbuf sb;
    enum err e = strbuf_init_copy_cstr(&sb, "foo bar baz");
    assert(e == OK);

    e = strbuf_replace_all(&sb, " ", "q p");
    assert(strcmp(sb.ptr, "fooq pbarq pbaz") == 0);

    strbuf_deinit(&sb);
}

int main(void) {
    test_strbuf_from_owned_cstr();
    test_strbuf_init_copy_str();
    test_strbuf_push();
    test_strbuf_replace();
    test_strbuf_replace2();
    test_strbuf_replace_all();
}
