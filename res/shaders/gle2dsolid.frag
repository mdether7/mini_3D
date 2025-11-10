#version 460 core
out vec4 FinalColor;
in vec4 vs_pass_color;
uniform vec2 u_res;
uniform float u_time;

//1280w
//720h

vec3 three_circles(vec2 uv)
{
   vec3 color = vec3(0.0);
   float xd = 0;

   for (int i = 0; i < 10; i++) {

      vec2 circle_point = vec2(0.01 * i, 0.25);
      xd = length(uv - circle_point);
   }

   return  vec3(xd, xd, xd);
}

float sdSphere( vec3 p, float s )
{
  return length(p)-s;
}

void main()
{
   vec2 uv = (gl_FragCoord.xy / u_res.xy); //* 2.0 - 1.0; // 0,0 in center
   uv -= vec2(0.5f ,0.5f);
   uv.x *= u_res.x /u_res.y; // TODO make figure it out.

   float r = 0.0555;

   vec3 origin = vec3(0.0f);

   vec2 point_1 = vec2(cos(u_time) * 0.1, sin(u_time) * 0.1);
   vec2 point_2 = vec2(sin(u_time) * 0.2, cos(u_time) * 0.2);
   float dist_1 = smoothstep(r, r+0.005, length(uv - point_1));
   float dist_2 = smoothstep(r, r+0.002, length(uv - point_2));
   float dist_3 = sdSphere(origin, 0.2);

   FinalColor = vec4(dist_1, dist_2, dist_3, 1.0);
   FinalColor.r *= 0.2;
}

// Shader Cheat Sheet ///////////////////////////
// vec2 centered = uv - 0.5;  // Now ranges [-0.5, 0.5]
// float dist = length(centered);  // Distance from center

// // Or in one line:
// float dist = length(uv - 0.5);

// Distance from point (cx, cy) to uv
// float dist = length(uv - vec2(cx, cy));

// distance formula.
//float shape = sqrt(pow(uv.x - 1.0,2.)+pow(uv.y-1.0,2.));
// float shape_two = sqrt(pow(uv.x+0.5, 2.0) + pow(uv.y+0.5, 2.0));

// // Centering UV coordinates
// vec2 st = uv - 0.5; // or
// vec2 st = uv * 2.0 - 1.0;  // normalized device coordinates

// // Aspect ratio correction
// vec2 st = uv;
// st.x *= iResolution.x / iResolution.y;

// // Polar coordinates helper
// vec2 toCenter = uv - 0.5;

// // Direction vector
// vec2 dir = normalize(vec2(1.0, 1.0));

// // 2. Convert int to float when needed
// float x = float(i) / 10.0;

// // 3. Keep loops short (performance!)
// for (int i = 0; i < 20; i++) { }  // OK
// for (int i = 0; i < 1000; i++) { } // Too slow!
/////////////////////////////////////////

