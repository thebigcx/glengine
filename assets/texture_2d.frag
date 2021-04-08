#version 460

in vec2 uv;
in vec3 color;
in float tex_index;

out vec4 frag_color;

uniform sampler2D u_textures[32];

void main()
{
    frag_color = texture(u_textures[int(tex_index)], uv);
}