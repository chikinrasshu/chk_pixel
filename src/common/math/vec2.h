#pragma once

union u_V3;
union u_V4;

typedef union u_V2
{
    float raw[2];
    struct
    {
        float x, y;
    };
    struct
    {
        float w, h;
    };
    struct
    {
        float u, v;
    };
    struct
    {
        float s, t;
    };
} V2;

V2 v2(float x, float y);

V2 v2_add(V2 a, V2 b);
V2 v2_sub(V2 a, V2 b);
V2 v2_mul(V2 v, float scale);
V2 v2_div(V2 v, float divisor);
V2 v2_normal(V2 v);
V2 v2_edge_normal(V2 a, V2 b);
V2 v2_hadamard(V2 a, V2 b);
float v2_dot(V2 a, V2 b);
float v2_mag_sq(V2 v);
float v2_mag(V2 v);
float v2_dist_sq(V2 a, V2 b);
float v2_dist(V2 a, V2 b);
