uniform float time;

void main(void)
{
	vec4 vert = vec4(gl_Vertex);
	
	vert.y += sin(vert.z + time);
	vert.x += sin(vert.z + time);

	gl_Position = gl_ModelViewProjectionMatrix * vert;
}

