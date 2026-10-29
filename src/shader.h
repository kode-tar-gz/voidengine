#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef uint32_t Shader;

Shader
shader_load_from_file(const char *vertex, const char *fragment);

void
shader_use(Shader shader);

#endif
