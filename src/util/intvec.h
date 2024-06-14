#ifndef INTVEC_H
#define INTVEC_H

#include "common.h"
#include "error.h"
#include "log.h"
#include "vec.h"

vec_define_struct(intvec, i64);
span_define_struct(intspan, i64);

#define intvec_to_span(v) vec_to_span(intvec, intspan, (v))

#define intvec_sum(v) intspan_sum(intvec_to_span(v))
i64 intspan_sum(struct intspan nums);
#define intvec_product(v) intspan_product(intvec_to_span(v))
i64 intspan_product(struct intspan nums);
#define intvec_max(v) intspan_max(intvec_to_span(v))
i64 intspan_max(struct intspan nums, i64 default_);
#define intvec_min(v) intspan_min(intvec_to_span(v))
i64 intspan_min(struct intspan nums, i64 default_);
#define intvec_print(v) intspan_print(intvec_to_span(v))
void intspan_print(struct intspan nums);

#if LOG_DBG
#define intvec_log_dbg(nums) intvec__log_dbg(nums, my_basename(__FILE__), __LINE__)
void intvec__log_dbg(struct intvec nums, char const *filename, size_t lineno);
#else
#define intvec_log_dbg(nums)
#endif

#endif  // INTVEC_H
