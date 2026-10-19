#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef enum {
  SHADER_VERTEX   = GL_VERTEX_SHADER,
  SHADER_FRAGMENT = GL_FRAGMENT_SHADER,
  SHADER_GEOMETRY = GL_GEOMETRY_SHADER
} shader_kind_t;

typedef struct {
  uint32_t id;
  shader_kind_t kind;
  const char *src;
} Shader;

int
shader_compile(Shader shader);

Shader
shader_load_from_str(shader_kind_t kind, const char *src);

#endif
