#version 460

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_tangent;
layout (location = 3) in vec2 a_uv;

out vec3 normal;
out vec3 tangent;
out vec2 uv;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_transform;

void main()
{
    gl_Position = u_projection * u_view * u_transform * vec4(a_pos, 1.0);

    normal  = a_normal;
    tangent = a_tangent;
    uv      = a_uv;
}