#ifndef CORE_H
#define CORE_H

#include <stdint.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow *
init_gl(int w, int h, const char *title);

void
init_buffers(uint32_t *VBO, uint32_t *VAO, uint32_t *EBO);

void
render_pink_screen();

char *
load_source_file(const char *filepath);

#endif
