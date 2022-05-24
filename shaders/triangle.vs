#version 330 core
layout (location = 0) in vec2 vertex; // <vec2 position>
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

out vec3 the_color;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    the_color = aColor;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}