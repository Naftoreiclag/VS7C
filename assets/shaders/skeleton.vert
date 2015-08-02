// Bone transforms
uniform mat4 bones[64];

// Bones that affect this vertex
attribute vec4 boneIndices;

// How much each bone affects it
attribute vec4 boneWeights;


void main()
{
	int currBone = int(boneIndices.x);
	vec4 newVert = (bones[currBone] * gl_Vertex) * boneWeights.x;
	
	currBone = int(boneIndices.y);
	newVert = (bones[currBone] * gl_Vertex) * boneWeights.y + newVert;
	
	currBone = int(boneIndices.z);
	newVert = (bones[currBone] * gl_Vertex) * boneWeights.z + newVert;
	
	currBone = int(boneIndices.w);
	newVert = (bones[currBone] * gl_Vertex) * boneWeights.w + newVert;

	gl_Position = gl_ModelViewProjectionMatrix * newVert;
	
	// Textures
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

