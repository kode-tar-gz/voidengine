#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_col;

out vec3 new_col;

void main()
{
  gl_Position = vec4(v_pos, 1.0f);
  new_col = v_col;
}
