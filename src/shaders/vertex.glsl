#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_col;

uniform float dt; // our delta time

out vec3 new_col;

void main()
{
  float speed = 0.5;
  float angle = dt * speed;

  float s = sin(angle);
  float c = cos(angle);

  mat3 rotx = mat3(1,    0,    0,
		   0,    c,   -s,
		   0,    s,    c);
  mat3 roty = mat3(c,    0,   -s,
		   0,    1,    0,
		   s,    0,    c);
  mat3 rotz = mat3(c,   -s,    0,
		   s,    c,    0,
		   0,    0,    1);

  mat3 final_rot = rotx * roty * rotz;
  vec3 rot_col = final_rot * v_col;

  gl_Position = vec4(v_pos, 1.0);
  new_col = rot_col;
}
