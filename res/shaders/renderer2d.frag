#version 460 core

uniform sampler2D renderer2d_texture;

out vec4 Frag_Color;

in vec4 color;
in vec2 uv;
in flat int tex_flag;

void main()
{
    vec2 uv_flip = vec2(uv.x, 1.0 - uv.y);
    
    /**
     * Texture coordinates UVs
     * (0,1)    (1,1)      U = x-axis
     *                     V = y-axis
     * (0,0)    (1,0)
     */

    vec4 finalColor = (tex_flag == 1) ? texture(renderer2d_texture, uv_flip) : color;
    
    // Make pixels with red turn black
    if (finalColor.r > 0.01) {
        finalColor = vec4(1.0, 1.0, 1.0, 1.0);
    }

    // Reduce red by 50%
    //finalColor.r *= 0.5;
    Frag_Color = finalColor;
}