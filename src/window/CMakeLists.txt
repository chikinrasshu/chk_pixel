add_library(chk_window "window.h" "window.c")
target_link_libraries(chk_window chk_common glfw glad_gl_core_41)

target_include_directories(chk_window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
