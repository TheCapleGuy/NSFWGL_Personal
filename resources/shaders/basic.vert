#version 410
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec4 Tangent;
layout(location = 3) in vec2 TexCoord;

out vec2 vTexCoord;
out vec4 vPosition;
out vec4 vNormal;
out vec4 vTangent;
out vec3 vBiTangent;
out vec4 vShadowCoord; //uv coordinates to sample the depth map

uniform mat4 LightMatrix;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	vPosition    = Position;

	vTexCoord    = TexCoord;

	vTangent 	 =  Model * Tangent;
	
	vNormal 	 =  Model * Normal;
	
	vBiTangent   = cross(vNormal.xyz, vTangent.xyz); 

	gl_Position  = Projection *  View *  Model * Position; 	
	
	vShadowCoord = LightMatrix * Model * Position;
}