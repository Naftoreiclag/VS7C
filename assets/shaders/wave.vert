uniform float time;

void main(void)
{
	vec4 vert = vec4(gl_Vertex);
	
	vert.y += sin(vert.y + time);

	gl_Position = gl_ModelViewProjectionMatrix * vert;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

