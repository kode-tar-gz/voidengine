#include <stdio.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GL/freeglut.h>

void render_scene()
{
  static GLclampf dt = 0.0f;
  static bool ascending = true;

  glClear(GL_COLOR_BUFFER_BIT);
  glutPostRedisplay();

  glClearColor(dt*2, dt, dt*2, dt);
  if(dt <= 1.0f && ascending == true){
    dt += 1.0f/256.0f;
  } else {
    dt -= 1.0f/256.0f;
    ascending = false;
  }
  if(dt <= 0.0f) ascending = true;

  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  glutInitWindowSize(1280, 720);
  glutInitWindowPosition(100, 100);
  int window_id = glutCreateWindow("Hello, OpenGL!");
  printf("[INFO] Window ID: %d\n", window_id);

  if(!gladLoadGL()){
    printf("[ERROR] Failed to initalize GLAD.\n");
    return 1;
  }

  glutDisplayFunc(render_scene);
  glutMainLoop();

  return 0;
}
