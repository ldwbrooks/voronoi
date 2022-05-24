#version 330 core
in vec3 the_color;
out vec4 color;

void main()
{    
    color = vec4(the_color, 1.0);
}  