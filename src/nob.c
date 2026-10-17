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

#ifdef __APPLE__
  nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-I"INC_DIR);
  nob_cmd_append(&cmd, "-o", BUILD_DIR"main", SRC_DIR"main.c", SRC_DIR"glad.c");
  nob_cmd_append(&cmd, "-I/opt/homebrew/include", "-L/opt/homebrew/lib", "-lglfw");
  nob_cmd_append(&cmd, "-framework", "Cocoa", "-framework", "OpenGL", "-framework", "IOKit",);
  nob_cmd_append(&cmd, "-framework", "CoreVideo", "-framework", "QuartzCore");
#endif
#ifdef __linux__
  // building our engine
  nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-I"INC_DIR);
  nob_cmd_append(&cmd, "-o", BUILD_DIR"main", SRC_DIR"main.c", SRC_DIR"glad.c");
  nob_cmd_append(&cmd, "-lglfw3", "-lGL", "-lX11", "-lpthread", "-lXrandr", "-lXi", "-ldl");
#endif
  if (!nob_cmd_run(&cmd)) return 1;
}
