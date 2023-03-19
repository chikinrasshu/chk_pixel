#include "vec2.h"
#include <math.h>

V2 v2(float x, float y) { return (V2){x, y}; }

V2 v2_add(V2 a, V2 b) { return v2(a.x + b.x, a.y + b.y); }
V2 v2_sub(V2 a, V2 b) { return v2(a.x - b.x, a.y - b.y); }
V2 v2_mul(V2 v, float scale) { return v2(v.x * scale, v.y * scale); }
V2 v2_div(V2 v, float divisor) { return v2(v.x / divisor, v.y / divisor); }
V2 v2_normal(V2 v) { return v2(-v.y, v.x); }
V2 v2_edge_normal(V2 a, V2 b) { return v2_normal(v2_sub(b, a)); }
V2 v2_hadamard(V2 a, V2 b) { return v2(a.x * b.x, a.y * b.y); }
float v2_dot(V2 a, V2 b) { return (a.x * b.x) + (a.y * b.y); }
float v2_mag_sq(V2 v) { return v2_dot(v, v); }
float v2_mag(V2 v) { return sqrtf(v2_mag_sq(v)); }
float v2_dist_sq(V2 a, V2 b) { return v2_mag_sq(v2_sub(b, a)); }
float v2_dist(V2 a, V2 b) { return sqrtf(v2_dist_sq(a, b)); }
