add_library(chk_soft 
    "soft.c" "soft.h"
    "soft_gl.c" "soft_gl.h"
)
target_link_libraries(chk_soft chk_common chk_renderer glad_gl_core_41)

target_include_directories(chk_soft PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
