#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "core.h"

int
main()
{
  int width  = 800;
  int height = 600;
  GLFWwindow *window = init_gl(width, height, "Void Engine");

  // get our vertex buffer object and vertex array object
  uint32_t VBO, VAO, EBO;
  init_buffers(&VBO, &VAO, &EBO);

  // get our shader
  Shader shader = shader_load_from_file("shaders/vertex.glsl", "shaders/fragment.glsl");
  shader_use(shader);

  while(!glfwWindowShouldClose(window)){
    shader_use(shader);
    float current_frame = glfwGetTime();
    shader_set_float(shader, "dt", current_frame);

    // rendering
    render_pink_screen();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // cleanup
    glfwSwapBuffers(window);

    // check and poll events/callbacks
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
