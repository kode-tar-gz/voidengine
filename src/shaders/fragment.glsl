#version 330 core

in vec3 new_col;
in vec2 tex_coord;
out vec4 frag_color;

uniform sampler2D our_tex;

void main()
{
  vec4 tex_col = texture(our_tex, tex_coord);
  frag_color = vec4(new_col, 1.0f) * tex_col;
}
