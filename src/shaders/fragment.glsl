#version 430 core
#extension GL_ARB_separate_shader_objects : enable

#define pi 3.1415926536
#define pi2 1.57079632679
#define pi3 1.0471975512
//layout(origin_upper_left) in vec4 gl_FragCoord;

layout (location = 3) uniform vec3 cameraDir;

out vec4 FragColor;
uniform float time;
uniform sampler2D diffuseMap;
uniform sampler2D reflectionMap;

in vec4 loc;
in vec4 lp;
in vec4 wp;
in vec3 n;
in vec2 tc;

vec3 ls[2] = {
    normalize(vec3(-1, 1, 1)),
    normalize(vec3(1, 1, 1))
};

void main()
{
    vec3 nn = normalize(n);
    nn = normalize(cross(dFdx(wp.xyz), dFdy(wp.xyz)));
   // float le = length(lp.xyz);
   // vec3 nn2 = nn - 0.5* wp.xyz;
   //nn = le > 0.83f ? nn : nn2;

    float blick1 = pow(abs(dot(nn, ls[0])), 5);


    float angle = atan(nn.y / nn.x) / (pi * 2) + 0.2;
    vec3 diffuse = vec3(1,0,0.4);
float k = 14;
//    if(sin(lp.x * k) + sin(lp.y * k) + sin(lp.z * k) < -0.5)
//        diffuse = vec3(1,1,1);
   //FragColor  = mix(vec4(1), vec4(0.7), fract(gl_FragCoord.y * 0.05) > 0.5 != fract(gl_FragCoord.x * 0.05) > 0.5 ? 1 : 0);
    FragColor = vec4( diffuse * 0.3 + blick1, 1.0f);

}
