#pragma once

union u_V2;
union u_V4;

typedef union u_V4
{
    float raw[4];
    struct
    {
        float x, y, z, w;
    };
    struct
    {
        float r, g, b, a;
    };
} V4;

V4 v4(float x, float y, float z, float w);

V4 v4_add(V4 a, V4 b);
V4 v4_sub(V4 a, V4 b);
V4 v4_mul(V4 a, float scalar);
V4 v4_div(V4 a, float divisor);
V4 v4_hadamard(V4 a, V4 b);
float v4_dot(V4 a, V4 b);
float v4_mag_sq(V4 v);
float v4_mag(V4 v);
float v4_dist_sq(V4 a, V4 b);
float v4_dist(V4 a, V4 b);
