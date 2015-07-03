uniform mat4 mWorldViewProj;
uniform mat4 mInvWorld;
uniform mat4 mTransWorld;
uniform vec3 mLightPos;
uniform vec4 mLightColor;

void main(void) {
	// Calculate the final position of current vertex
	gl_Position = mWorldViewProj * gl_Vertex;
	
	// Calculate normal
	vec4 normal = vec4(gl_Normal, 0.0);
	normal = mInvWorld * normal;
	normal = normalize(normal);
	
	// ???
	vec4 worldpos = gl_Vertex * mTransWorld;
	
	vec4 lightVector = worldpos - vec4(mLightPos,1.0);
	lightVector = normalize(lightVector);
	
	vec4 modifiedColor = mLightColor * dot(-lightVector, normal);
	gl_FrontColor = gl_BackColor = vec4(modifiedColor.x, modifiedColor.y, modifiedColor.z, 0.0);
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
