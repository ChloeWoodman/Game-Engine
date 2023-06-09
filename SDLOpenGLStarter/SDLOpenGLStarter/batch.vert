#version 140

in vec3 vertexPos3DLocation;
in vec2 vUV;
// Texture id input attribute
in int vTextureId;

// uv output
out vec2 uv;

// Texture id output
// Have to use flat to indicate don't want to interpolate
// textureId for each fragment like uv is 
flat out int textureId;

uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 transformMat;

void main(){
	vec4 v = vec4(vertexPos3DLocation.xyz, 1);
	v = projMat* viewMat* transformMat* v;
	gl_Position = v;
	uv = vUV;
	
	// Texture associated with vertex
	textureId = vTextureId;
}

