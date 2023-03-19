#include "vec4.h"

V4 v4(float x, float y, float z, float w) { return (V4){x, y, z, w}; }

V4 v4_add(V4 a, V4 b) { return v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
V4 v4_sub(V4 a, V4 b) { return v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
V4 v4_mul(V4 v, float scale) { return v4(v.x * scale, v.y * scale, v.z * scale, v.w * scale); }
V4 v4_div(V4 v, float divisor) { return v4(v.x / divisor, v.y / divisor, v.z / divisor, v.w / divisor); }
V4 v4_hadamard(V4 a, V4 b) { return v4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
float v4_dot(V4 a, V4 b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w); }
float v4_mag_sq(V4 v) { return v4_dot(v, v); }
float v4_mag(V4 v) { return sqrtf(v4_mag_sq(v)); }
float v4_dist_sq(V4 a, V4 b) { return v4_mag_sq(v4_sub(b, a)); }
float v4_dist(V4 a, V4 b) { return sqrtf(v4_dist_sq(a, b)); }