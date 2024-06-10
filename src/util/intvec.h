#ifndef INTVEC_H
#define INTVEC_H

#include "common.h"
#include "vec.h"

vec_define_struct(intvec, i32);

i32 intvec_sum(struct intvec nums);
i32 intvec_product(struct intvec nums);

#endif  // INTVEC_H
