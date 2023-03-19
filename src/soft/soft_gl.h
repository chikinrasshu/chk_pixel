#pragma once

#include <bitmap.h>
#include <common.h>

#include <glad/gl.h>

typedef struct s_SoftGL
{
    GLuint vao;
    GLuint vbos[2];
    GLuint vert_shader, frag_shader, shader_program;
    GLuint texture;
    GLint texture_location, texture_unit;

    int vp_x, vp_y, vp_w, vp_h;
} SoftGL;

bool chk_soft_gl_init(SoftGL *gl, int win_w, int win_h);
void chk_soft_gl_destroy(SoftGL *gl);

SoftGL *chk_soft_gl_create(int win_w, int win_h);
void chk_soft_gl_free(SoftGL *gl);

void chk_soft_gl_update_viewport(SoftGL *gl, int x, int y, int w, int h);
void chk_soft_gl_update_tex(SoftGL *gl, Bitmap *bmp);
void chk_soft_gl_clear_region(SoftGL *gl, int x, int y, int w, int h);
void chk_soft_gl_present(SoftGL *gl, Bitmap *bmp);
