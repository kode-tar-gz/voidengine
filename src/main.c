#include <stdio.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

const float vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f
};

const char *vertex_shader_src =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
  "}\0";

const char *frag_shader_src =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "  FragColor = vec4(0.7f, 0.0f, 1.0f, 1.0f);"
  "}\0";

bool wireframe = false;

void
framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    // TEMPORARY. Just silencing the pesky 'unused parameter' compiler warning
    glfwGetWindowMonitor(window);
}

void
error_callback(int error, const char* description)
{
  fprintf(stderr, "[ERROR] %d : %s\n", error, description);
}

void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
  }

  if(key == GLFW_KEY_W && action == GLFW_PRESS){
    wireframe = true ? wireframe == false : false;
    if(wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

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

  // set callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetErrorCallback(error_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSwapInterval(1);

  // initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    printf("[ERROR] Failed to initalize GLAD.\n");
    glfwTerminate();
  }

  glViewport(0, 0, w, h);
  return window;
}

void
init_buffers(uint32_t *VBO, uint32_t *VAO)
{
  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glBindVertexArray(*VAO);
  glBindBuffer(GL_ARRAY_BUFFER, *VBO);

  // copy data into buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // set the vertex attribute configuration
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
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

int
main()
{
  int width  = 800;
  int height = 600;
  GLFWwindow *window = init_gl(width, height, "Void Engine");

  // get our vertex buffer object and vertex array object
  uint32_t VBO, VAO;
  init_buffers(&VBO, &VAO);

  Shader vshader = shader_load_from_str(SHADER_VERTEX, vertex_shader_src);
  shader_compile(vshader);
  Shader fshader = shader_load_from_str(SHADER_FRAGMENT, frag_shader_src);
  shader_compile(fshader);

  uint32_t shader_program = shader_program_get();
  shader_program_attach(shader_program, 2, vshader, fshader);
  shader_program_link(shader_program);
  shader_delete(2, vshader, fshader);

  while(!glfwWindowShouldClose(window)){
    // rendering
    render_pink_screen();
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // cleanup
    glfwSwapBuffers(window);

    // check and poll events/callbacks
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
