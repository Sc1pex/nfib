#include "num.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

size_t digits(uint64_t x) {
    size_t d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

BigNum bignum_fromu64(uint64_t num) {
    BigNum b;
    b.size = digits(num);
    b.digits = malloc(b.size);

    for (size_t i = 0; i < b.size; i++) {
        b.digits[i] = num % 10;
        num /= 10;
    }

    return b;
}

BigNum bignum_add(BigNum n1, BigNum n2) {
    BigNum s;
    s.size = 0;
    s.digits = malloc(MAX(n1.size, n2.size));

    int carry = 0;
    for (; s.size < MIN(n1.size, n2.size); s.size++) {
        s.digits[s.size] = n1.digits[s.size] + n2.digits[s.size] + carry;
        carry = s.digits[s.size] / 10;
        s.digits[s.size] = s.digits[s.size] % 10;
    }
    for (; s.size < n1.size; s.size++) {
        s.digits[s.size] = n1.digits[s.size] + carry;
        carry = s.digits[s.size] / 10;
        s.digits[s.size] = s.digits[s.size] % 10;
    }
    for (; s.size < n2.size; s.size++) {
        s.digits[s.size] = n2.digits[s.size] + carry;
        carry = s.digits[s.size] / 10;
        s.digits[s.size] = s.digits[s.size] % 10;
    }

    if (carry) {
        s.digits = realloc(s.digits, s.size + 1);
        s.digits[s.size] = carry;
        s.size++;
    }

    return s;
}

void bignum_free(BigNum* n) {
    if (n->digits) {
        free(n->digits);
        n->digits = NULL;
    }
    n->size = 0;
}

void bignum_print(BigNum n) {
    for (size_t i = 0; i < n.size; i++) {
        size_t idx = n.size - i - 1;
        printf("%d", n.digits[idx]);
    }
}
