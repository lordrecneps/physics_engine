#version 330

uniform mat4 camera;
uniform mat4 model;

in vec3 vert;
in vec2 vertTexCoord;
in vec3 vertNormal;

out vec3 pos;

void main()
{
    pos = vec3(model * vec4(vert, 1));
    gl_Position = camera * model * vec4(vert, 1);
}