#include "math.h"

Vec2 vec_fromu64(uint64_t x, uint64_t y) {
    Vec2 v;
    v.x = bignum_fromu64(x);
    v.y = bignum_fromu64(y);
    return v;
}

void vec_free(Vec2* m) {
    bignum_free(&m->x);
    bignum_free(&m->y);
}

Mat2 mat_fromu64(uint64_t x1, uint64_t x2, uint64_t y1, uint64_t y2) {
    Mat2 m;
    m.x1 = bignum_fromu64(x1);
    m.x2 = bignum_fromu64(x2);
    m.y1 = bignum_fromu64(y1);
    m.y2 = bignum_fromu64(y2);
    return m;
}

BigNum mat_step(
    const BigNum* l1, const BigNum* l2, const BigNum* c1, const BigNum* c2
) {
    BigNum p1 = bignum_mult(l1, c1);
    BigNum p2 = bignum_mult(l2, c2);
    BigNum res = bignum_add(&p1, &p2);
    bignum_free(&p1);
    bignum_free(&p2);
    return res;
}
Mat2 mat_mat_mult(const Mat2* m1, const Mat2* m2) {
    Mat2 res;

    res.x1 = mat_step(&m1->x1, &m1->x2, &m2->x1, &m2->y1);
    res.x2 = mat_step(&m1->x1, &m1->x2, &m2->x2, &m2->y2);
    res.y1 = mat_step(&m1->y1, &m1->y2, &m2->x1, &m2->y1);
    res.y2 = mat_step(&m1->y1, &m1->y2, &m2->x2, &m2->y2);

    return res;
}

Vec2 mat_vec_mult(const Mat2* m, const Vec2* v) {
    Vec2 res;

    res.x = mat_step(&m->x1, &m->x2, &v->x, &v->y);
    res.y = mat_step(&m->y1, &m->y2, &v->x, &v->y);

    return res;
}

Mat2 mat_exp(const Mat2* m, uint64_t e) {
    if (e == 0) {
        Mat2 m = mat_fromu64(1, 0, 0, 1);
        return m;
    }

    if (e == 1) {
        Mat2 m1 = mat_clone(m);
        return m1;
    }

    if (e % 2 == 0) {
        Mat2 m1 = mat_exp(m, e / 2);
        Mat2 res = mat_mat_mult(&m1, &m1);
        mat_free(&m1);
        return res;
    } else {
        Mat2 m1 = mat_exp(m, e - 1);
        Mat2 res = mat_mat_mult(&m1, m);
        mat_free(&m1);
        return res;
    }
}

Mat2 mat_clone(const Mat2* m) {
    Mat2 res;
    for (int i = 0; i < 4; i++) {
        res.el[i] = bignum_clone(&m->el[i]);
    }
    return res;
}

void mat_free(Mat2* m) {
    for (int i = 0; i < 4; i++) {
        bignum_free(&m->el[i]);
    }
}
