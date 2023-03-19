#pragma once

#include "common.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"

typedef V4 RGBA;
typedef V3 RGB;
typedef V2 UV;

uint32_t chk_packed_rgba_from_u8(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint32_t chk_packed_rgba_from_float(float r, float g, float b, float a);
uint32_t chk_packed_rgba(RGBA color);
