#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

#include "core.h"

typedef uint32_t Texture;

Texture
texture_load_from_file(const char *filepath);

void
texture_use(Texture tex);

#endif
