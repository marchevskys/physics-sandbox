#version 430 core
#extension GL_ARB_separate_shader_objects : enable

#define pi 3.1415926536
#define pi2 1.57079632679
#define pi3 1.0471975512
//layout(origin_upper_left) in vec4 gl_FragCoord;


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
    float le = length(lp.xyz);
    vec3 nn2 = nn - 0.5* wp.xyz;
   // nn = le > 0.83f ? nn : nn2;
    nn = normalize(nn);
    float blick1 = pow(abs(dot(nn, ls[0])), 5);
    float blick2 = pow(abs(dot(nn, ls[1])), 5);
    blick1 = sin(blick1 * 1) * 0.5 + 0.5;

    float angle = atan(nn.y / nn.x) / (pi * 2) + 0.2;

//    if(fract((wp.x + wp.y + wp.z)* 1.4) < 0.2)
//        discard;
       //FragColor  = mix(vec4(1), vec4(0.7), fract(gl_FragCoord.y * 0.05) > 0.5 != fract(gl_FragCoord.x * 0.05) > 0.5 ? 1 : 0);
    FragColor = vec4( vec3(blick1 + blick2), 1.0f);

}
