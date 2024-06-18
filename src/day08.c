#include <stdbool.h>

#include "util/common.h"
#include "util/error.h"
#include "util/fileiter.h"
#include "util/intvec.h"
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

static bool node_is_goal(struct node node) {
    if (PART1) {
        int const goal1 = ((int)'Z' << 16) | ((int)'Z' << 8) | ((int)'Z');
        return node.key == goal1;
    } else {
        return (node.key & 0xff) == (int)'Z';
    }
}

#if LOG_DBG
static void node_log_dbg(struct node const node) {
    (void)node;
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
#else
#define node_log_dbg(node)
#endif

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

static ERRFN find_node(struct node **const node, struct nodevec *const nodes, int const key) {
    assert(node);
    assert(nodes);
    enum err e = OK;

    e = vec_bsearch(nodevec, node, nodes, &key, node_compare);
    if (e) {
        log_err("Could not find key %c%c%c", (char)(key >> 16), (char)(key >> 8), (char)key);
        return e;
    }
    node_log_dbg(**node);

    return OK;
}

static ERRFN get_steps_to_goal(
    i64 *const steps,
    struct nodevec *const nodes,
    struct str const instructions,
    int key
) {
    assert(nodes);

    enum err e = OK;
    struct node *node;
    for (size_t step = 0; step < STEPS_LIMIT; ++step) {
        e = find_node(&node, nodes, key);
        if (e) return e;
        if (node_is_goal(*node)) {
            *steps = (i64)step;
            return OK;
        }

        char inst = instructions.ptr[step % instructions.len];
        log_dbg("Instruction: %c", inst);

        key = inst == 'L' ? node->left : node->right;
    }
    log_err("Could not find goal");
    return ERR_INPUT;
}

int main(int argc, char *argv[]) {
    struct fileiter f;
    struct strbuf instructions;
    struct nodevec nodes = {0};
    struct intvec cycle_lengths = {0};

    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    // Parse instructions

    e = fileiter_line_strbuf_init(&instructions, &f);
    if (e) goto error;

    e = fileiter_skip_blank_line(&f);
    if (e) goto error;

    // Parse nodes

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

    // Sort nodes

    vec_sort(nodevec, nodes, node_compare);

    // Get initial positions

    if (PART1) {
        i64 steps;
        e = get_steps_to_goal(&steps, &nodes, strbuf_to_str(instructions), parse_key("AAA"));
        if (e) goto error;
        printf("%ld\n", steps);
    } else {
        for (size_t i = 0; i < nodes.len; ++i) {
            int const key = nodes.ptr[i].key;
            if ((key & 0xff) == (int)'A') {
                i64 steps;
                e = get_steps_to_goal(&steps, &nodes, strbuf_to_str(instructions), key);
                if (e) goto error;
                e = vec_push(intvec, &cycle_lengths, steps);
                if (e) goto error;
            }
        }

        // This outputs answer of "lcd(...)" that can be copy-pasted into
        // WolframAlpha to get final answer.
        // TODO: Add actual prime-factorization implementation of LCD
        printf("lcd(");
        for (size_t i = 0; i < cycle_lengths.len; ++i) {
            if (i != 0) printf(", ");
            printf("%ld", cycle_lengths.ptr[i]);
        }
        printf(")\n");
    }

    // Step through instructions

error:
    vec_deinit(intvec, &cycle_lengths);
    vec_deinit(nodevec, &nodes);
    strbuf_deinit(&instructions);
    fileiter_deinit(&f);
    return (int)e;
}
