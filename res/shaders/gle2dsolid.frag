#version 460 core
out vec4 FinalColor;
in vec4 vs_pass_color;
uniform vec2 u_res;
uniform float u_time;

float Circle(vec2 uv, float r, float blur)
{
   float d = length(uv);
   float c = smoothstep(r, r-blur, d);
   return c;
}

void main()
{
   vec2 uv = (gl_FragCoord.xy / u_res.xy) * 2.0 - 1.0; // 0,0 in center
   float m = smoothstep(fract(u_time * 0.2), 0.7, uv.x);
   float x = smoothstep(fract(u_time * 0.2), 0.8, uv.y);
   FinalColor = vec4(m, m, x, 1.0f);
}