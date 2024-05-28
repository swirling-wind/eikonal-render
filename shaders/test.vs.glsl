#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform float pos_offset;

void main()
{
    gl_Position = vec4(aPos.x + pos_offset, aPos.yz, 1.0);
    ourColor = aColor;
}
