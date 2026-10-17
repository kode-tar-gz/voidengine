# Void Engine

A simple 3D game engine written completely in C with OpenGL[^1].
Mostly done for recreational and educational purposes, but I also plan on making a full game with this engine.

You can clone GLFW3 and build it from source or install it through other means, this engine relies on it. Everything else is contained in this repo.

We use [nob.h](https://github.com/tsoding/nob.h) as our build system, the idea being that you shouldn't need anything other than the C compiler to build this engine. First you bootstrap the build system by running:
```bash
cc -o nob nob.c
```
And from then on, all you need to do to build the program is run `./nob`, even after changing `nob.c`.


[^1]: We use GLFW for windowing and GLAD for wrangling OpenGL functions.
