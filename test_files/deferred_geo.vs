#version 330

uniform mat4 camera;
uniform mat4 model;

in vec3 vert;
in vec2 vertTexCoord;
in vec3 vertNormal;

out vec2 texCoord;
out vec3 normal;
out vec3 pos;

void main()
{
    texCoord = vertTexCoord;
    pos = vert;
    normal = vertNormal;

    gl_Position = camera * model * vec4(vert, 1);
}