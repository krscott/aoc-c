#include "intvec.h"

#include <stdio.h>

#include "error.h"

i64 intvec_sum(struct intvec nums) {
    i64 total = 0;
    for (ssize_t i = 0; i < nums.len; ++i) {
        total += nums.buf[i];
    }
    return total;
}

i64 intvec_product(struct intvec nums) {
    i64 total = 1;
    for (ssize_t i = 0; i < nums.len; ++i) {
        total *= nums.buf[i];
    }
    return total;
}

enum err intvec_min(i64 *out, struct intvec nums) {
    if (nums.len <= 0) return err_trace(ERR_INPUT);

    i64 min = nums.buf[0];
    for (ssize_t i = 1; i < nums.len; ++i) {
        if (min > nums.buf[i]) {
            min = nums.buf[i];
        }
    }

    *out = min;
    return OK;
}

enum err intvec_max(i64 *out, struct intvec nums) {
    if (nums.len <= 0) return err_trace(ERR_INPUT);

    i64 max = nums.buf[0];
    for (ssize_t i = 1; i < nums.len; ++i) {
        if (max < nums.buf[i]) {
            max = nums.buf[i];
        }
    }

    *out = max;
    return OK;
}

void intvec_print(struct intvec nums) {
    printf("[");
    for (ssize_t i = 0; i < nums.len; ++i) {
        if (i > 0) printf(", ");
        printf("%ld", nums.buf[i]);
    }
    printf("]");
}

#if LOG_DBG
void intvec__log_dbg(struct intvec nums, char const *filename, size_t lineno) {
    fprintf(stderr, "DEBUG %s:%d [", filename, lineno);
    for (ssize_t i = 0; i < nums.len; ++i) {
        if (i > 0) fprintf(stderr, ", ");
        fprintf(stderr, "%ld", nums.buf[i]);
    }
    fprintf(stderr, "]\n");
}
#endif
