#include "colors.h"

uint32_t chk_packed_rgba_from_u8(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { return (a << 24) | (b << 16) | (g << 8) | r; }
uint32_t chk_packed_rgba_from_float(float r, float g, float b, float a) { return chk_packed_rgba_from_u8((uint8_t)(r * 255.0f), (uint8_t)(g * 255.0f), (uint8_t)(b * 255.0f), (uint8_t)(a * 255.0f)); }
uint32_t chk_packed_rgba(RGBA color) { return chk_packed_rgba_from_float(color.r, color.g, color.b, color.a); }
