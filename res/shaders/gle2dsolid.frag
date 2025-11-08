#version 460 core
out vec4 FinalColor;
in vec4 vs_pass_color;
uniform vec2 u_res; // RESOLUTION!!
void main()
{
   vec2 uv = (gl_FragCoord.xy / u_res.xy) * 2.0 - 1.0;
   FinalColor = vec4(uv.x, 0.0f, 0.0f, 1.0f);
}