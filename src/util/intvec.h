#ifndef INTVEC_H
#define INTVEC_H

#include "common.h"
#include "error.h"
#include "vec.h"

vec_define_struct(intvec, i64);

// TODO: Semantically, should nums be a pointer?
i64 intvec_sum(struct intvec nums);
i64 intvec_product(struct intvec nums);
enum err intvec_max(i64 *out, struct intvec nums);
enum err intvec_min(i64 *out, struct intvec nums);
void intvec_print(struct intvec nums);

#if LOG_DBG
#define intvec_log_dbg(nums) intvec__log_dbg(nums, __FILE_NAME__, __LINE__)
void intvec__log_dbg(struct intvec nums, char const *filename, size_t lineno);
#else
#define intvec_log_dbg(nums)
#endif

#endif  // INTVEC_H
