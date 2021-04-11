#shader vertex
#version 460

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_color;
layout (location = 3) in float a_tex_index;

uniform mat4 u_transform;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 uv;
out vec3 color;
out float tex_index;

void main()
{
    gl_Position = u_projection * u_view * u_transform * vec4(a_pos, 1.0);
    
    uv        = a_uv;
    color     = a_color;
    tex_index = a_tex_index;
}

#shader fragment
#version 460

in vec2 uv;
in vec3 color;
in float tex_index;

out vec4 frag_color;

uniform sampler2D u_textures[32];

void main()
{
    frag_color = texture(u_textures[int(tex_index)], uv) * vec4(color, 1);
}