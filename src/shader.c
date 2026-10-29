#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "core.h"

int32_t
shader_compile(Shader shader, const char *src)
{
  int32_t success;
  char info_log[512] = {0};

  // compile shader
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  // check and handle errors
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    printf("[ERROR] SHADER %d COMPILATION FAILED: %s\n", shader, info_log);
  }

  return success;
}

void
shader_delete(Shader shader, char *src)
{
  glDeleteShader(shader);
  free((void*)src);
}

Shader
shader_program_get()
{
  return (Shader)glCreateProgram();
}

void
shader_program_attach(Shader prog, int count, ...)
{
  va_list args;
  uint32_t shader;
  va_start(args, count);
  for(int i = 0; i < count; i++) {
    shader = va_arg(args, uint32_t);
    glAttachShader(prog, shader);
  }
  va_end(args);
}

int32_t
shader_program_link(Shader prog)
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

// here are the functions that are actually available to external contexts

Shader
shader_load_from_file(const char *vertex, const char *fragment)
{
  // get the program
  Shader shader = shader_program_get();
  // get the vertex and fragment shader ids
  Shader vshader = glCreateShader(GL_VERTEX_SHADER);
  Shader fshader = glCreateShader(GL_FRAGMENT_SHADER);
  // get the source for those shaders and compile them
  char *vsrc = load_source_file(vertex);
  char *fsrc = load_source_file(fragment);
  shader_compile(vshader, vsrc);
  shader_compile(fshader, fsrc);

  // attach the shaders to the program
  shader_program_attach(shader, 2, vshader, fshader);
  shader_program_link(shader);

  // cleanup
  shader_delete(vshader, vsrc);
  shader_delete(fshader, fsrc);

  return shader;
}

void
shader_use(Shader shader)
{
  glUseProgram(shader);
}
