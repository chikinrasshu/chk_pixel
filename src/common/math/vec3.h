#pragma once

union u_V2;
union u_V4;

typedef union u_V3
{
    float raw[3];
    struct
    {
        float x, y, z;
    };
    struct
    {
        float r, g, b;
    };
} V3;

V3 v3(float x, float y, float z);

V3 v3_add(V3 a, V3 b);
V3 v3_sub(V3 a, V3 b);
V3 v3_mul(V3 a, float scalar);
V3 v3_div(V3 a, float divisor);
V3 v3_face_normal(V3 a, V3 b, V3 c);
V3 v3_hadamard(V3 a, V3 b);
V3 v3_cross(V3 a, V3 b);
float v3_dot(V3 a, V3 b);
float v3_mag_sq(V3 v);
float v3_mag(V3 v);
float v3_dist_sq(V3 a, V3 b);
float v3_dist(V3 a, V3 b);
