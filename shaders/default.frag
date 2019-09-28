in vec2 v_texcoord0;
out vec4 fragColor;
uniform sampler2D u_texture0;

void main()
{
    fragColor = texture(u_texture0, v_texcoord0);
}
