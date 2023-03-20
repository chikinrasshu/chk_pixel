#include "soft_gl.h"

// Forward declarations
void chk_soft_gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
bool chk_soft_gl_check_shader_compilation(GLuint shader);
bool chk_soft_gl_check_shader_linking(GLuint program);

// Constants
const char *g_chk_soft_gl_vert_shader_source =
    "#version 410 core\n"
    "layout (location = 0) in vec2 pos;\n"
    "layout (location = 1) in vec2 uv;\n"
    "out vec2 frag_uv;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(pos, 0.0, 1.0);\n"
    "    frag_uv = uv;\n"
    "}";

const char *g_chk_soft_gl_frag_shader_source =
    "#version 410 core\n"
    "uniform sampler2D tex;\n"
    "in vec2 frag_uv;\n"
    "out vec4 out_col;\n"
    "void main()\n"
    "{\n"
    "    out_col = texture(tex, frag_uv);\n"
    "}";

static const float g_chk_soft_gl_fs_quad_vertices[] = {
    -1, -1,
    1, -1,
    -1, 1,
    1, 1};

static const GLfloat g_chk_soft_gl_fs_quad_tex_coords[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f};

// Implementation
bool chk_soft_gl_init(SoftGL *gl, int win_w, int win_h)
{
    chk_error_if(!gl, "gl was NULL.") return false;

    // - Setup the debug callback
    if (GLAD_GL_ARB_debug_output)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(chk_soft_gl_debug_callback, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

        chk_log_if(CHK_SOFT_GL_LOG, "Registered the debugging callback");
    }

    // - Setup the fixed pipeline
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // - Set the default clear color
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    // - Create the VAO and the VBOs
    glGenVertexArrays(1, &gl->vao);
    glBindVertexArray(gl->vao);

    chk_logf_if(CHK_SOFT_GL_LOG, "Created vertex array id:%u", gl->vao);

    // - Vertex Buffer Object
    glGenBuffers(chk_array_count(gl->vbos), gl->vbos);
    glBindBuffer(GL_ARRAY_BUFFER, gl->vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_chk_soft_gl_fs_quad_vertices), g_chk_soft_gl_fs_quad_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, gl->vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_chk_soft_gl_fs_quad_tex_coords), g_chk_soft_gl_fs_quad_tex_coords, GL_STATIC_DRAW);

    chk_logf_if(CHK_SOFT_GL_LOG, "Created buffers id:%u and id:%u", gl->vbos[0], gl->vbos[1]);

    // - Sending the Vertex Data
    glBindBuffer(GL_ARRAY_BUFFER, gl->vbos[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, gl->vbos[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);

    size_t sent_bytes = 2 * sizeof(GLfloat) + 2 * sizeof(GLfloat);
    chk_logf_if(CHK_SOFT_GL_LOG, "Sent %zu bytes to the GPU for the vertices", sent_bytes);

    // - Texture bindings
    glGenTextures(1, &gl->texture);
    glBindTexture(GL_TEXTURE_2D, gl->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    chk_logf_if(CHK_SOFT_GL_LOG, "Created texture id:%u", gl->texture);

    // - Create the shader program
    gl->shader_program = glCreateProgram();

    chk_logf_if(CHK_SOFT_GL_LOG, "Created shader program id:%u", gl->shader_program);

    // - Compile the vertex shader
    gl->vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(gl->vert_shader, 1, &g_chk_soft_gl_vert_shader_source, NULL);
    glCompileShader(gl->vert_shader);
    if (!chk_soft_gl_check_shader_compilation(gl->vert_shader))
        return false;

    chk_logf_if(CHK_SOFT_GL_LOG, "Compiled vertex shader id:%u", gl->vert_shader);

    // - Compile the fragment shader
    gl->frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(gl->frag_shader, 1, &g_chk_soft_gl_frag_shader_source, NULL);
    glCompileShader(gl->frag_shader);
    if (!chk_soft_gl_check_shader_compilation(gl->frag_shader))
        return false;

    chk_logf_if(CHK_SOFT_GL_LOG, "Compiled fragment shader id:%u", gl->frag_shader);

    //  - Link both shaders with the program
    glAttachShader(gl->shader_program, gl->vert_shader);
    glAttachShader(gl->shader_program, gl->frag_shader);
    glLinkProgram(gl->shader_program);
    if (!chk_soft_gl_check_shader_linking(gl->shader_program))
        return false;

    chk_logf_if(CHK_SOFT_GL_LOG, "Linked shader program id:%u with vertex shader id:%u and fragment shader id:%u",
                gl->shader_program, gl->vert_shader, gl->frag_shader);

    //  - We're done.
    glUseProgram(gl->shader_program);

    gl->texture_location = glGetUniformLocation(gl->shader_program, "tex");
    gl->texture_unit = 0;

    chk_soft_gl_update_viewport(gl, 0, 0, win_w, win_h);

    chk_log_if(CHK_SOFT_GL_LOG, "Initted softGL");
    return true;
}

void chk_soft_gl_destroy(SoftGL *gl)
{
    chk_error_if(!gl, "gl was NULL.") return;

    if (gl->shader_program)
        glDeleteProgram(gl->shader_program);
    if (gl->vert_shader)
        glDeleteShader(gl->vert_shader);
    if (gl->frag_shader)
        glDeleteShader(gl->frag_shader);
    if (gl->texture)
        glDeleteTextures(1, &gl->texture);
    if (gl->vbos[0] || gl->vbos[1])
        glDeleteBuffers(chk_array_count(gl->vbos), gl->vbos);
    if (gl->vao)
        glDeleteVertexArrays(1, &gl->vao);

    chk_log_if(CHK_SOFT_GL_LOG, "Destroyed softGL");
}

SoftGL *chk_soft_gl_create(int win_w, int win_h)
{
    SoftGL *gl = chk_alloc(sizeof(*gl));
    if (!chk_soft_gl_init(gl, win_w, win_h))
    {
        chk_free(gl);
        return NULL;
    }

    return gl;
}

void chk_soft_gl_free(SoftGL *gl)
{
    chk_error_if(!gl, "gl was NULL.") return;

    chk_soft_gl_destroy(gl);
    chk_free(gl);
}

void chk_soft_gl_update_viewport(SoftGL *gl, int x, int y, int w, int h)
{
    chk_error_if(!gl, "gl was NULL.") return;

    glViewport(x, y, w, h);
    gl->vp_x = x;
    gl->vp_y = y;
    gl->vp_w = w;
    gl->vp_h = h;
}

void chk_soft_gl_update_tex(SoftGL *gl, Bitmap *bmp)
{
    chk_error_if(!gl, "gl was NULL.") return;
    chk_error_if(!bmp, "bmp was NULL.") return;
    chk_errorf_if(bmp->bpp != 4, "Unsupported bitmap bpp of %d.", bmp->bpp) return;

    glActiveTexture(GL_TEXTURE0 + gl->texture_unit);
    glBindTexture(GL_TEXTURE_2D, gl->texture);

    // glPixelStorei(GL_PACK_ALIGNMENT, 1);
    // glPixelStorei(GL_UNPACK_ROW_LENGTH, (GLint)bmp->stride);

    int format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp->w, bmp->h, 0, format, GL_UNSIGNED_BYTE, bmp->memory);
}

void chk_soft_gl_clear_region(SoftGL *gl, int x, int y, int w, int h)
{
    chk_error_if(!gl, "gl was NULL.") return;

    int saved_x = gl->vp_x;
    int saved_y = gl->vp_y;
    int saved_w = gl->vp_w;
    int saved_h = gl->vp_h;

    chk_soft_gl_update_viewport(gl, x, y, w, h);
    glClear(GL_COLOR_BUFFER_BIT);
    chk_soft_gl_update_viewport(gl, saved_x, saved_y, saved_w, saved_h);
}

void chk_soft_gl_present(SoftGL *gl, Bitmap *bmp)
{
    chk_error_if(!gl, "gl was NULL.") return;
    chk_error_if(!gl->shader_program, "Missing shader program.");

    chk_soft_gl_update_tex(gl, bmp);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gl->shader_program);

    glActiveTexture(GL_TEXTURE0 + gl->texture_unit);
    glBindTexture(GL_TEXTURE_2D, gl->texture);
    glUniform1i(gl->texture_location, gl->texture_unit);

    glBindVertexArray(gl->vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// Implementation of Forwarded functions
bool chk_soft_gl_check_shader_compilation(GLuint shader)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar info_log[512];
        glGetShaderInfoLog(shader, chk_array_count(info_log), NULL, info_log);

        fprintf(stderr, "chk_soft_gl_check_shader_compilation: Shader compilation failed:\n");
        fprintf(stderr, "chk_soft_gl_check_shader_compilation: \t - %s", info_log);
        return false;
    }
    return true;
}

bool chk_soft_gl_check_shader_linking(GLuint program)
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar info_log[512];
        glGetProgramInfoLog(program, chk_array_count(info_log), NULL, info_log);

        fprintf(stderr, "chk_soft_gl_check_shader_linking: Shader program linking failed:\n");
        fprintf(stderr, "chk_soft_gl_check_shader_linking: \t - %s\n", info_log);
        return false;
    }
    return true;
}

// Callback
void chk_soft_gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    const char *src_name = "Unknown";
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        src_name = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        src_name = "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        src_name = "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        src_name = "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        src_name = "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        src_name = "Other";
        break;
    }

    const char *type_name = "Unknown";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        type_name = "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type_name = "Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type_name = "Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        type_name = "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        type_name = "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        type_name = "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        type_name = "Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        type_name = "Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        type_name = "Other";
        break;
    }

    const char *severity_name = "Unknown";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        severity_name = "High";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        severity_name = "Medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        severity_name = "Low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        severity_name = "Notification";
        break;
    }

    fprintf(stderr, "chk_soft_gl_debug_callback: Source: %s, Type: %s, Severity: %s =>\n", src_name, type_name, severity_name);
    fprintf(stderr, "chk_soft_gl_debug_callback: \t - %s\n", message);
}
