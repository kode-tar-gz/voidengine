#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

Shader
shader_load_from_str(shader_kind_t kind, const char *src)
{
  uint32_t id = glCreateShader(kind);
  Shader shader = {.id = id,
		   .kind = kind,
		   .src = src};
  return shader;
}

int32_t
shader_compile(Shader shader)
{
  int32_t success;
  char info_log[512] = {0};

  // compile shader
  glShaderSource(shader.id, 1, &shader.src, NULL);
  glCompileShader(shader.id);

  // check and handle errors
  glGetShaderiv(shader.id, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(shader.id, 512, NULL, info_log);
    printf("[ERROR] SHADER %d COMPILATION FAILED: %s\n", shader.id, info_log);
  }

  return success;
}

void
shader_delete(int count, ...)
{
  va_list args;
  Shader shader;
  va_start(args, count);
  for(int i = 0; i < count; i++) {
    shader = va_arg(args, Shader);
    glDeleteShader(shader.id);
  }
  va_end(args);
}

uint32_t
shader_program_get()
{
  return glCreateProgram();
}

void
shader_program_attach(uint32_t prog, int count, ...)
{
  va_list args;
  Shader shader;
  va_start(args, count);
  for(int i = 0; i < count; i++) {
    shader = va_arg(args, Shader);
    glAttachShader(prog, shader.id);
  }
  va_end(args);
}

int32_t
shader_program_link(uint32_t prog)
{
  int32_t success;
  char info_log[512] = {0};

  glLinkProgram(prog);

  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(prog, 512, NULL, info_log);
    printf("[ERROR] PROGRAM %d LINKING FAILED: %s\n", prog, info_log);
  }

  return success;
}
