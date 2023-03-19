cmake_minimum_required(VERSION 3.25)
project(chk_pixel)

# Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("ext/glfw")

# Add Glad
set(GLAD_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
glad_add_library(glad_gl_core_41 REPRODUCIBLE API gl:core=4.1)

# Add whereami
set(WAI_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/whereami/src")
add_library(whereami STATIC ${WAI_SOURCES_DIR}/whereami.c
                            ${WAI_SOURCES_DIR}/whereami.h)
target_include_directories(whereami PUBLIC ${WAI_SOURCES_DIR})

# Compile folders as libraries
add_subdirectory(src/common)
add_subdirectory(src/renderer)
add_subdirectory(src/soft)
add_subdirectory(src/window)

# Compile the main executable
add_executable(chk_pixel "src/main.c")
target_link_libraries(chk_pixel chk_common chk_renderer chk_soft chk_window)

# Sym-Link the data folder to the executable
add_custom_command(
    TARGET chk_pixel 
    COMMAND ${CMAKE_COMMAND} -E 
        create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/data 
                       $<TARGET_FILE_DIR:chk_pixel>/data
)
