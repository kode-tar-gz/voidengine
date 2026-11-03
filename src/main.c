#include <stdint.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "shader.h"
#include "core.h"

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
  uint32_t tex; glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  // set filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // get the texture data
  int t_width, t_height, nr_channels;
  char *t_filename = "../howitbe.png";
  uint8_t *t_data = stbi_load(t_filename, &t_width, &t_height, &nr_channels, 0);
  if(t_data == NULL) void_log(ERROR, "Failed to load image '%s'.\n", t_filename);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height,
	       0, GL_RGB, GL_UNSIGNED_BYTE, t_data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(t_data);

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
    gl_check_error();
  }

  glfwTerminate();
  return 0;
}
