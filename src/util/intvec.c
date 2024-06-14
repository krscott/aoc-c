#include "intvec.h"

#include <stdio.h>

#include "error.h"

i64 intspan_sum(struct intspan const nums) {
    i64 total = 0;
    for (size_t i = 0; i < nums.len; ++i) {
        total += nums.ptr[i];
    }
    return total;
}

i64 intspan_product(struct intspan const nums) {
    i64 total = 1;
    for (size_t i = 0; i < nums.len; ++i) {
        total *= nums.ptr[i];
    }
    return total;
}

i64 intspan_max(struct intspan nums, i64 default_) {
    if (nums.len == 0) return default_;

    i64 max = nums.ptr[0];
    for (size_t i = 1; i < nums.len; ++i) {
        if (max < nums.ptr[i]) {
            max = nums.ptr[i];
        }
    }

    return max;
}

i64 intspan_min(struct intspan nums, i64 def) {
    if (nums.len == 0) return def;

    i64 min = nums.ptr[0];
    for (size_t i = 1; i < nums.len; ++i) {
        if (min > nums.ptr[i]) {
            min = nums.ptr[i];
        }
    }

    return min;
}

void intspan_print(struct intspan const nums) {
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
