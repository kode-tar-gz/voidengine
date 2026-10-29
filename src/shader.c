#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

Shader
shader_load_from_str(shader_kind_t kind, char *src)
{
  uint32_t id = glCreateShader(kind);
  Shader shader = {.id = id,
		   .kind = kind,
		   .src = src};
  return shader;
}

Shader
shader_load_from_file(shader_kind_t kind, const char *filepath)
{
  FILE *src_fd;
  if ((src_fd = fopen(filepath, "r")) == NULL){
    perror(filepath);
    exit(1);
  }

  fseek(src_fd, 0, SEEK_END);
  size_t fsize = ftell(src_fd);
  assert(fsize > 0);

  char *src = (char*)malloc(fsize + 1);
  fseek(src_fd, 0, SEEK_SET);
  if(fread(src, 1, fsize, src_fd) < fsize){
    perror(filepath);
    exit(1);
  }
  src[fsize] = '\0';
  printf("Loaded %s (%zu bytes) successfully :3\n", filepath, fsize);

  fclose(src_fd);
  Shader shader = shader_load_from_str(kind, src);
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
    free((void*)shader.src);
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
