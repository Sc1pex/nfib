#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct {
    char* digits;
    size_t size;
} BigNum;

BigNum bignum_fromu64(uint64_t num);
BigNum bignum_add(BigNum n1, BigNum n2);
void bignum_free(BigNum* n);
void bignum_print(BigNum n);
