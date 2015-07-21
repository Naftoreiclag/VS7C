// Irrlicht texture
uniform sampler2D myTexture;

void main() {
	// Color is from the texture
    vec4 col = texture2D(myTexture, vec2(gl_TexCoord[0]));
	
	// Use vertex colors
    //col *= gl_Color;
    gl_FragColor = col;
}
