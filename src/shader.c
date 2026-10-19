#include <stdio.h>
#include <stdint.h>

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
  int success;
  char info_log[512];

  // compile shader
  glShaderSource(shader.id, 0, &shader.src, NULL);
  glCompileShader(shader.id);

  // check and handle errors
  glGetShaderiv(shader.id, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(shader.id, 512, NULL, info_log);
    printf("[ERROR] SHADER %d COMPILATION FAILED: %s\n", shader.id, info_log);
  }

  return success;
}
