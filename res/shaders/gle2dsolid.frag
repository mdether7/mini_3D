#version 460 core
out vec4 FinalColor;
in vec4 vs_pass_color;
uniform vec2 u_res;
uniform float u_time;

//1280w
//720h

void main()
{
   vec2 uv = (gl_FragCoord.xy / u_res.xy); //* 2.0 - 1.0; // 0,0 in center
   uv -= vec2(0.5f ,0.5f);
   uv.x *= u_res.x /u_res.y;
   vec3 gradient = vec3(smoothstep(0.25, 0.26,  fract(length(uv) * floor(u_time))));
   vec3 red = vec3(1.0, 0.0, 0.0);
   gradient += red;

   FinalColor = vec4(gradient, 1.0f);
}