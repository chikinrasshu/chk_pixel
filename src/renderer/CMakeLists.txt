add_library(chk_renderer "cmd_list.c" "cmd_list.h" "renderer.c" "renderer.h")
target_link_libraries(chk_renderer chk_common chk_window)

target_include_directories(chk_renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
