/* linmath.h - a minimal linear algebra library for OpenGL (public domain)
   https://github.com/datenwolf/linmath.h */
#ifndef LINMATH_H
#define LINMATH_H
#include <math.h>

typedef float mat4x4[4][4];

typedef float vec4[4];

typedef float vec3[3];

typedef float vec2[2];

static void mat4x4_identity(mat4x4 M)
{
    int i, j;
    for(i=0; i<4; ++i)
        for(j=0; j<4; ++j)
            M[i][j] = i==j ? 1.0f : 0.0f;
}

static void mat4x4_mul(mat4x4 M, const mat4x4 a, const mat4x4 b)
{
    int i, j, k;
    for(i=0; i<4; ++i) for(j=0; j<4; ++j) {
        M[i][j] = 0.0f;
        for(k=0; k<4; ++k)
            M[i][j] += a[i][k] * b[k][j];
    }
}

static void mat4x4_ortho(mat4x4 M, float l, float r, float b, float t, float n, float f)
{
    mat4x4_identity(M);
    M[0][0] = 2.0f/(r-l);
    M[1][1] = 2.0f/(t-b);
    M[2][2] = 2.0f/(n-f);
    M[3][0] = (l+r)/(l-r);
    M[3][1] = (t+b)/(b-t);
    M[3][2] = (n+f)/(f-n);
}

static void mat4x4_rotate_Z(mat4x4 M, const mat4x4 a, float angle)
{
    mat4x4_identity(M);
    float s = sinf(angle), c = cosf(angle);
    M[0][0] =  c; M[0][1] = s;
    M[1][0] = -s; M[1][1] = c;
    for(int i=0; i<4; ++i) for(int j=0; j<4; ++j) {
        float v = 0.0f;
        for(int k=0; k<4; ++k)
            v += a[i][k] * M[k][j];
        M[i][j] = v;
    }
}

#endif /* LINMATH_H */
