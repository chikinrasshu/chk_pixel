add_library(chk_tex "tex.c" "tex.h" "tex_loader.c" "tex_loader.h")
target_link_libraries(chk_tex chk_common spng)

target_include_directories(chk_tex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
