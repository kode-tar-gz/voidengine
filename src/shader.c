#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int
compile_shader(unsigned int shaderid, const char *shader_src)
{
  int success;
  char info_log[512];

  // compile shader
  glShaderSource(shaderid, 0, &shader_src, NULL);
  glCompileShader(shaderid);

  // check and handle errors
  glGetShaderiv(shaderid, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(shaderid, 512, NULL, info_log);
    printf("[ERROR] SHADER %d COMPILATION FAILED: %s\n", shaderid, info_log);
  }

  return success;
}
