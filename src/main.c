#include <stdio.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow *init_gl(int w, int h, const char *title)
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

void render_scene()
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

void handle_input(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    // TEMPORARY. Just silencing the pesky 'unused parameter' compiler warning
    glfwGetWindowMonitor(window);
}

int main()
{
  int width  = 800;
  int height = 600;
  GLFWwindow *window = init_gl(width, height, "Void Engine");

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  while(!glfwWindowShouldClose(window)){
    // input
    handle_input(window);

    // rendering
    render_scene();

    // cleanup
    glfwSwapBuffers(window);

    // check and poll events/callbacks
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
