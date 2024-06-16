#include "util/error.h"
#include "util/fileiter.h"
#include "util/log.h"
#include "util/str.h"
#include "util/vec.h"

#define NODE_KEY_LEN 3
#define NODE_LINE_LEN 16
#define NODE_LEFT_I 7
#define NODE_RIGHT_I 12
#define STEPS_LIMIT 999999

struct node {
    int key;
    int left;
    int right;
};
vec_define_struct(nodevec, struct node);

static int node_compare(void const *const a, void const *const b) {
    return *(int const *)a - *(int const *)b;
}

static void node_log_dbg(struct node const node) {
    log_dbg(
        "%c%c%c = (%c%c%c, %c%c%c)",
        (char)(node.key >> 16),
        (char)(node.key >> 8),
        (char)node.key,
        (char)(node.left >> 16),
        (char)(node.left >> 8),
        (char)node.left,
        (char)(node.right >> 16),
        (char)(node.right >> 8),
        (char)node.right
    );
}

static int parse_key(char const *const s) {
    return (((int)(s[0])) << 16) | (((int)(s[1])) << 8) | ((int)(s[2]));
}

static ERRFN parse_node(struct node *const node, struct str const line) {
    assert(node);

    if (line.len != NODE_LINE_LEN) {
        log_err("Expected node line len %d, got: %.*s", NODE_LINE_LEN, (int)line.len, line.ptr);
        return ERR_INPUT;
    }

    *node = (struct node){
        .key = parse_key(line.ptr),
        .left = parse_key(&line.ptr[NODE_LEFT_I]),
        .right = parse_key(&line.ptr[NODE_RIGHT_I]),
    };

    return OK;
}

int main(int argc, char *argv[]) {
    struct fileiter f;
    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    struct strbuf instructions;
    e = fileiter_line_strbuf_init(&instructions, &f);
    if (e) goto error;

    e = fileiter_skip_blank_line(&f);
    if (e) goto error;

    struct nodevec nodes = {0};

    for (;;) {
        struct str line;
        e = fileiter_line(&line, &f);
        if (e == ERR_NONE) break;
        if (e) goto error;

        struct node node;
        e = parse_node(&node, line);
        if (e) goto error;

        e = vec_push(nodevec, &nodes, node);
        if (e) goto error;
    }

    vec_sort(nodevec, &nodes, node_compare);

    int curr_key = parse_key("AAA");
    int goal_key = parse_key("ZZZ");
    size_t steps = 0;
    for (; steps < STEPS_LIMIT; ++steps) {
        struct node *node = NULL;
        e = vec_bsearch(nodevec, &node, &nodes, &curr_key, node_compare);
        if (e) {
            log_err("Could not find key");
            goto error;
        }
        node_log_dbg(*node);
        if (node->key == goal_key) break;
        char inst = instructions.ptr[steps % instructions.len];
        log_dbg("Instruction: %c", inst);
        int next_key = inst == 'L' ? node->left : node->right;
        if (next_key == curr_key) {
            log_err("Infinite loop");
            e = ERR_NONE;
            goto error;
        }
        curr_key = next_key;
    }
    if (steps == STEPS_LIMIT) {
        log_err("Could not find goal");
        e = ERR_NONE;
        goto error;
    }
    printf("%zu\n", steps);

error:
    vec_deinit(nodevec, &nodes);
    strbuf_deinit(&instructions);
    fileiter_deinit(&f);
    return (int)e;
}
