#version 460 core

out vec4 FinalColor;

in vec2 TexCoord;

uniform sampler2D texture_one;

void main()
{
    FinalColor = texture(texture_one, TexCoord);
}