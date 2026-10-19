#define NOB_IMPLEMENTATION

#include "nob.h"

#define BUILD_DIR "../build/"
#define LINK_DIR  BUILD_DIR"link/"
#define SRC_DIR   "./"
#define INC_DIR   "../include/"

void
append_flags(Nob_Cmd *cmd)
{
  // platform-specific flags
#ifdef __APPLE__
  nob_cmd_append(cmd, "-Wall", "-Wextra", "-I/opt/homebrew/include");
  nob_cmd_append(cmd, "-L/opt/homebrew/lib", "-lglfw", "-framework", "Cocoa");
  nob_cmd_append(cmd, "-framework", "OpenGL", "-framework", "IOKit",);
  nob_cmd_append(cmd, "-framework", "CoreVideo", "-framework", "QuartzCore");
#else
  nob_cmd_append(cmd, "-Wall", "-Wextra", "-lglfw", "-lGL",);
  nob_cmd_append(cmd, "-lX11", "-lpthread", "-lXrandr", "-lXi", "-ldl");
#endif
}

int
compile_lib(Nob_Cmd *cmd_lib, const char *file, Nob_Cmd *cmd_main)
{
  // reset cmd_lib
  cmd_lib->count = 0;
  // get the filename without '.c' extension
  char file_no_ext[50] = {0};
  strncpy(file_no_ext, file, (strlen(file) - 2));

  // get the full path to the output file
  char final_file[50] = {0};
  strcpy(final_file, LINK_DIR);
  strcat(final_file, file_no_ext);
  strcat(final_file, ".o");

  // don't do anything if file hasn't been modified since last compilation
  if(!nob_needs_rebuild(final_file, &file, 1)){
    nob_log(NOB_INFO, "Skipping %s: Output '%s' is newer than input.", file, final_file);
    nob_cmd_append(cmd_main, strdup(final_file));
    return 0;
  }

  // compile
  nob_cc(cmd_lib);
  nob_cmd_append(cmd_lib, "-c", "-I"INC_DIR, "-o", final_file, file);
  append_flags(cmd_lib);
  if (!nob_cmd_run(cmd_lib)) return 1;

  // append object file to main compilation command
  nob_cmd_append(cmd_main, strdup(final_file));
  return 0;
}

int
main(int argc, char **argv)
{
  NOB_GO_REBUILD_URSELF(argc, argv);

  // making our build directories if they don't already exist
  if(!nob_mkdir_if_not_exists(BUILD_DIR)) return 1;
  if(!nob_mkdir_if_not_exists(LINK_DIR)) return 1;

  Nob_Cmd cmd_lib = {0};
  Nob_Cmd cmd_main = {0};
  Nob_File_Paths src_files = {0};

  // We start the command for compiling main and append libs as we go
  nob_cc(&cmd_main);
  nob_cmd_append(&cmd_main, "-I"INC_DIR, "-o", BUILD_DIR"main", "main.c");

  // reading through our whole source directory and
  // appending all source files inside to the list of files
  // to be compiled, except for the ones we don't want to compile
  nob_read_entire_dir(SRC_DIR, &src_files);
  for(int i = 0; i < src_files.count; i++){
    const char *file = src_files.items[i];
    const char *ext = strrchr(file, '.');

    //skip if we don't want the file to be compiled
    if(!strncmp(file, "nob.c",  strlen(file))  ||
       !strncmp(file, "main.c", strlen(file))  ||
       strcmp(ext, ".c")){
      continue;
    } else {
      // building all non-main files and appending to the main command
      if(compile_lib(&cmd_lib, file, &cmd_main) != 0) return 1;
    }
  }

  append_flags(&cmd_main);
  if (!nob_cmd_run(&cmd_main)) return 1;
}
