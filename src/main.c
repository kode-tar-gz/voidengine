#include <stdio.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

GLFWwindow *
init_gl(int w, int h, const char *title)
{
  // initialize GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // create the window
  GLFWwindow *window = glfwCreateWindow(w, h, title, NULL, NULL);
  if (!window){
    printf("[ERROR] Failed to create window.\n");
    glfwTerminate();
  }
  glfwMakeContextCurrent(window);

  // initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    printf("[ERROR] Failed to initalize GLAD.\n");
    glfwTerminate();
  }

  glViewport(0, 0, w, h);
  return window;
}

void
render_pink_screen()
{
  // pseudo delta-time; it's not actually delta-time but it'll work for this
  static GLclampf dt = 0.0f;
  static bool ascending = true;

  glClear(GL_COLOR_BUFFER_BIT);

  glClearColor(dt*2, dt, dt*2, dt);
  if(dt <= 1.0f && ascending == true){
    dt += 1.0f/256.0f;
  } else {
    dt -= 1.0f/256.0f;
    ascending = false;
  }
  if(dt <= 0.0f) ascending = true;
}

void
render_triangle()
{
  // coordinates for our triangle's vertices
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  // get our vertex buffer object
  uint32_t VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // copy data into buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void
handle_input(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
  }
}

void
framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    // TEMPORARY. Just silencing the pesky 'unused parameter' compiler warning
    glfwGetWindowMonitor(window);
}

int
main()
{
  int width  = 800;
  int height = 600;
  GLFWwindow *window = init_gl(width, height, "Void Engine");

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  const char *vertex_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 v_pos;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(v_pos.x, v_pos.y, v_pos.z, 1.0);\n"
    "}\0";

  const char *frag_shader_src =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(0.7f, 0.0f, 1.0f, 1.0f);"
    "{\0";

  Shader vshader = shader_load_from_str(SHADER_VERTEX, vertex_shader_src);
  shader_compile(vshader);
  Shader fshader = shader_load_from_str(SHADER_FRAGMENT, frag_shader_src);
  shader_compile(fshader);

  while(!glfwWindowShouldClose(window)){
    // input
    handle_input(window);

    // rendering
    render_pink_screen();

    // cleanup
    glfwSwapBuffers(window);

    // check and poll events/callbacks
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
