#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    uint8_t* digits;
    size_t size;
} BigNum;

BigNum bignum_fromu64(uint64_t num);
BigNum bignum_clone(const BigNum* b);
void bignum_free(BigNum* n);

BigNum bignum_add(const BigNum* n1, const BigNum* n2);
BigNum bignum_mult(const BigNum* n1, const BigNum* n2);
bool bignum_eq(BigNum n1, BigNum n2);

void bignum_print(BigNum n);
char* bignum_string(BigNum n);
