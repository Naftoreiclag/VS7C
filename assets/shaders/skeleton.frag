uniform sampler2D tex;

void main (void) {
    vec4 col = texture2D(tex, vec2(gl_TexCoord[0]));
    col *= gl_Color;
    gl_FragColor = col;
}
