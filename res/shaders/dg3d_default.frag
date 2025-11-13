#version 460 core

out vec4 FinalColor;

in vec2 TexCoord;

uniform sampler2D uTexture;

void main()
{
    FinalColor = texture(uTexture, TexCoord);
}