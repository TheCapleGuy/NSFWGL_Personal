#version 410
layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 3) in vec2 TexCoord;

out vec2 vTexCoord;
out vec4 vNormal;
out vec4 vShadowCoord; //uv coordinates to sample the depth map

uniform mat4 lightMatrix;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{

	vTexCoord = TexCoord;

	vNormal 	 = Projection *  View *  Model * Normal;
	gl_Position  = Projection *  View *  Model * Position; 	
	
	vShadowCoord = lightMatrix * Position;
}