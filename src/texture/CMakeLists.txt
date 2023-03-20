add_library(chk_tex "tex.c" "tex.h")
target_link_libraries(chk_tex chk_common)

target_include_directories(chk_tex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
