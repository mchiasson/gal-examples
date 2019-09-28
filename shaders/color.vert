in vec2 a_position;
in vec4 a_color;
out vec4 v_color;

uniform Constants {
    vec4 viewRect;
} constants;


void main()
{
    v_color = a_color;
    gl_Position = vec4(2.0 * a_position.x / constants.viewRect.z - 1.0,
                       1.0 - 2.0 * a_position.y / constants.viewRect.w,
                       0.0, 1.0);
}
