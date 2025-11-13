#version 460 core
layout (location = 0) in vec4 aPosAndTex;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPosAndTex.xy, 0.0, 1.0); 
    TexCoords = aPosAndTex.zw;
} 