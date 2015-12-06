#version 330

in vec3 vert;

uniform mat4 camera;
uniform mat4 model;

void main()
{
    gl_Position = camera * model * vec4(vert, 1);
}
