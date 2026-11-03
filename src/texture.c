#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "core.h"
#include "texture.h"

Texture
texture_load_from_file(const char *filepath)
{
  // get our texture
  Texture tex; glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  // set filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // get the texture data
  int t_width, t_height, nr_channels;
  uint8_t *t_data = stbi_load(filepath, &t_width, &t_height, &nr_channels, 0);
  if(t_data == NULL) void_log(ERROR, "Failed to load image '%s'.\n", filepath);
  // load the texture and its mipmap
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height,
	       0, GL_RGB, GL_UNSIGNED_BYTE, t_data);
  glGenerateMipmap(GL_TEXTURE_2D);

  //cleanup
  stbi_image_free(t_data);
  return tex;
}

void
texture_use(Texture tex)
{
  glBindTexture(GL_TEXTURE_2D, tex);
}
