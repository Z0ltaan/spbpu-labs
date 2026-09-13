#version 460

layout (location = 0) in vec3 aPos;
out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  Color = vec3(1.0, 1.0, 1.0);
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}

