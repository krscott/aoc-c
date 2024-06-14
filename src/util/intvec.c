#include "intvec.h"

#include <assert.h>
#include <stdio.h>

#include "error.h"

i64 intvec_sum(struct intvec const nums) {
    i64 total = 0;
    for (size_t i = 0; i < nums.len; ++i) {
        total += nums.ptr[i];
    }
    return total;
}

i64 intvec_product(struct intvec const nums) {
    i64 total = 1;
    for (size_t i = 0; i < nums.len; ++i) {
        total *= nums.ptr[i];
    }
    return total;
}

ERRFN intvec_min(i64 *const out, struct intvec const nums) {
    assert(out);
    if (nums.len <= 0) return err_trace(ERR_INPUT);

    i64 min = nums.ptr[0];
    for (size_t i = 1; i < nums.len; ++i) {
        if (min > nums.ptr[i]) {
            min = nums.ptr[i];
        }
    }

    *out = min;
    return OK;
}

ERRFN intvec_max(i64 *const out, struct intvec const nums) {
    assert(out);
    if (nums.len <= 0) return err_trace(ERR_INPUT);

    i64 max = nums.ptr[0];
    for (size_t i = 1; i < nums.len; ++i) {
        if (max < nums.ptr[i]) {
            max = nums.ptr[i];
        }
    }

    *out = max;
    return OK;
}

void intvec_print(struct intvec const nums) {
    printf("[");
    for (size_t i = 0; i < nums.len; ++i) {
        if (i > 0) printf(", ");
        printf("%ld", nums.ptr[i]);
    }
    printf("]");
}

#if LOG_DBG
void intvec__log_dbg(struct intvec const nums, char const *const filename, size_t const lineno) {
    fprintf(stderr, "DEBUG %s:%zu [", filename, lineno);
    for (size_t i = 0; i < nums.len; ++i) {
        if (i > 0) fprintf(stderr, ", ");
        fprintf(stderr, "%ld", nums.ptr[i]);
    }
    fprintf(stderr, "]\n");
}
#endif
