#version 330 core

uniform float dt; // our delta time

in vec3 new_col;
out vec4 frag_color;

void main()
{
  frag_color = vec4(new_col, 1.0f);
}
