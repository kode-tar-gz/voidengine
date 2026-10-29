#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef uint32_t Shader;

Shader
shader_load_from_file(const char *vertex, const char *fragment);

void
shader_use(Shader shader);

void
shader_set_bool(Shader shader, const char *name, bool val);

void
shader_set_int(Shader shader, const char *name, int val);

void
shader_set_float(Shader shader, const char *name, float val);

#endif
