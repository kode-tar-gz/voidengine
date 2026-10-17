#define NOB_IMPLEMENTATION

#include "nob.h"

#define BUILD_DIR "../build/"
#define SRC_DIR "./"
#define INC_DIR "../include/"

int main(int argc, char **argv)
{
  NOB_GO_REBUILD_URSELF(argc, argv);

  // making our build directory
  if(!nob_mkdir_if_not_exists(BUILD_DIR)) return 1;

  Nob_Cmd cmd = {0};

  // building our engine
  nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra");
  nob_cmd_append(&cmd, "-o", BUILD_DIR"main", SRC_DIR"main.c", SRC_DIR"glad.c");
  nob_cmd_append(&cmd, "-I"INC_DIR, "-lGL", "-lGLU", "-lglut");
  if (!nob_cmd_run(&cmd)) return 1;
}
