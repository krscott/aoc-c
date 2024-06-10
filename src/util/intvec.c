#include "intvec.h"

i32 intvec_sum(struct intvec nums) {
    i32 total = 0;
    for (ssize_t i = 0; i < nums.len; ++i) {
        total += nums.buf[i];
    }
    return total;
}

i32 intvec_product(struct intvec nums) {
    i32 total = 1;
    for (ssize_t i = 0; i < nums.len; ++i) {
        total *= nums.buf[i];
    }
    return total;
}
