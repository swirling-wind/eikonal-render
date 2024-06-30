#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float pos_offset;

void main()
{
    gl_Position = vec4(aPos.x + pos_offset, aPos.yz, 1.0);
    TexCoord = aTexCoord;
    ourColor = aColor;
}
