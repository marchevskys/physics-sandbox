#version 430 core
#extension GL_ARB_separate_shader_objects : enable
layout (location = 0) uniform mat4 model;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 projection;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec4 loc;
out vec4 lp;
out vec4 wp;
out vec3 n;
out vec2 tc;


void main()
{
    //float a = float(gl_VertexID);
    loc = vec4(model[3][0], model[3][1], model[3][2], model[3][3]);

    lp = vec4(position, 1.0f);
    wp = model * lp;

    n = mat3(model) * normal;
    tc = texCoord;
    gl_Position  =  projection * view * wp ;

}
