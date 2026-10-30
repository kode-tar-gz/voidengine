#ifndef CORE_H
#define CORE_H

#include <stdint.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef enum{
  INFO,
  LOG,
  WARNING,
  ERROR,
}Log_level;

GLFWwindow *
init_gl(int w, int h, const char *title);

void
init_buffers(uint32_t *VBO, uint32_t *VAO, uint32_t *EBO);

void
void_log(Log_level lvl, const char *str, ...);

static inline GLenum
__gl_check_error(const char *file, const char *line)
{
  GLenum err;
  while((err = glGetError()) != GL_NO_ERROR){
    char *error_str;
    switch(err){
    case GL_INVALID_ENUM:                  error_str = "INVALID_ENUM"; break;
    case GL_INVALID_VALUE:                 error_str = "INVALID_VALUE"; break;
    case GL_INVALID_OPERATION:             error_str = "INVALID_OPERATION"; break;
    case GL_OUT_OF_MEMORY:                 error_str = "OUT_OF_MEMORY"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: error_str = "INVALID_FRAMEBUFFER_OPERATION"; break;
    }
    void_log(ERROR, "%s:%s: %s\n", file, line, error_str);
  }
  return err;
}
#define gl_check_error() __gl_check_error(__FILE__, __LINE__)

void
render_pink_screen();

char *
load_source_file(const char *filepath);

#endif
