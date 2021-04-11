#version 460

in vec3 normal;
in vec3 tangent;
in vec2 uv;

out vec4 frag_color;

// Material
layout (binding = 0) uniform sampler2D u_albedo;

void main()
{
    frag_color = texture(u_albedo, uv);
}