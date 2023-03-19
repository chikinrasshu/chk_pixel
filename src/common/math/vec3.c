#include "vec3.h"
#include <math.h>

V3 v3(float x, float y, float z) { return (V3){x, y, z}; }

V3 v3_add(V3 a, V3 b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
V3 v3_sub(V3 a, V3 b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }
V3 v3_mul(V3 v, float scale) { return v3(v.x * scale, v.y * scale, v.z * scale); }
V3 v3_div(V3 v, float divisor) { return v3(v.x / divisor, v.y / divisor, v.z / divisor); }
V3 v3_face_normal(V3 a, V3 b, V3 c) { return v3_cross(v3_sub(b, a), v3_sub(c, a)); }
V3 v3_hadamard(V3 a, V3 b) { return v3(a.x * b.x, a.y * b.y, a.z * b.z); }
V3 v3_cross(V3 a, V3 b) { return v3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
float v3_dot(V3 a, V3 b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
float v3_mag_sq(V3 v) { return v3_dot(v, v); }
float v3_mag(V3 v) { return sqrtf(v3_mag_sq(v)); }
float v3_dist_sq(V3 a, V3 b) { return v3_mag_sq(v3_sub(b, a)); }
float v3_dist(V3 a, V3 b) { return sqrtf(v3_dist_sq(a, b)); }
