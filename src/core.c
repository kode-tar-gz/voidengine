#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core.h"

const float vertices[] = {
  // first triangle
   0.5f,  0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f,
  -0.5f,  0.5f, 0.0f,
};

const uint32_t indices[] = {
  0, 1, 3, // first triangle
  1, 2, 3 // second triangle
};

static bool wireframe = false;

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
  if(key == GLFW_KEY_Q && action == GLFW_PRESS){
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
init_buffers(uint32_t *VBO, uint32_t *VAO, uint32_t *EBO)
{
  // generate buffers
  glGenVertexArrays(1, VAO);
  glGenBuffers(1, EBO);
  glGenBuffers(1, VBO);

  // bind buffers
  glBindVertexArray(*VAO);
  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);

  // copy data into buffers
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

char *
load_source_file(const char *filepath)
{
  FILE *src_fd;
  if ((src_fd = fopen(filepath, "r")) == NULL){
    perror(filepath);
    exit(1);
  }

  fseek(src_fd, 0, SEEK_END);
  size_t fsize = ftell(src_fd);
  assert(fsize > 0);

  char *src = (char*)malloc(fsize + 1);
  fseek(src_fd, 0, SEEK_SET);
  if(fread(src, 1, fsize, src_fd) < fsize){
    perror(filepath);
    exit(1);
  }

  src[fsize] = '\0';
  printf("[INFO] Loaded %s (%zu bytes) successfully :3\n", filepath, fsize);

  fclose(src_fd);
  return src;
}
