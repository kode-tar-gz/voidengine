#include <stdint.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "shader.h"
#include "core.h"
#include "texture.h"

#define ASSETS_DIR "../assets/"

int
main()
{
  int w_width  = 800;
  int w_height = 600;
  GLFWwindow *window = init_gl(w_width, w_height, "Void Engine");

  // get our vertex buffer object and vertex array object
  uint32_t VBO, VAO, EBO;
  init_buffers(&VBO, &VAO, &EBO);

  // get our shader
  Shader shader = shader_load_from_file("shaders/vertex.glsl", "shaders/fragment.glsl");
  shader_use(shader);
  gl_check_error();

  // get our texture
  Texture tex = texture_load_from_file(ASSETS_DIR"tex.jpg");
  texture_use(tex);

  while(!glfwWindowShouldClose(window)){
    shader_use(shader);
    float current_frame = glfwGetTime();
    shader_set_float(shader, "dt", current_frame);

    // rendering
    //render_pink_screen();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // cleanup
    glfwSwapBuffers(window);

    // check and poll events/callbacks
    glfwPollEvents();
    gl_check_error();
  }

  glfwTerminate();
  return 0;
}
