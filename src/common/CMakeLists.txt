add_library(
  chk_common
  "arena.c"
  "arena.h"
  "bitmap.c"
  "bitmap.h"
  "colors.c"
  "colors.h"
  "common.c"
  "common.h"
  "math/vec2.c"
  "math/vec2.h"
  "math/vec3.c"
  "math/vec3.h"
  "math/vec4.c"
  "math/vec4.h")
target_link_libraries(chk_common whereami)

target_include_directories(chk_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
