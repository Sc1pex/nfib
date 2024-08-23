#pragma once

#include "num.h"

typedef union {
    struct {
        BigNum x1;
        BigNum y1;
        BigNum x2;
        BigNum y2;
    };
    BigNum el[4];
} Mat2;

Mat2 mat_fromu64(uint64_t x1, uint64_t x2, uint64_t y1, uint64_t y2);

Mat2 mat_mat_mult(const Mat2* m1, const Mat2* m2);
Mat2 mat_exp(const Mat2* m, uint64_t e);

Mat2 mat_clone(const Mat2* m);
void mat_free(Mat2* m);
