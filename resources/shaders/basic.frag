#version 410 
in vec2 vTexCoord;
in vec4 vNormal;

out vec4 FragColor;

uniform sampler2D Diffuse;

void main()
{
	vec4 oColor = texture(Diffuse, vTexCoord);
	
	vec4 lColor = vec4(0.8f, 0.4f, 0.6f, 1);
	
	vec4 lNormal = normalize(vec4(1,-1,1,0));
	
	float lamb = max(0, dot(-lNormal, vNormal));
	
	FragColor = oColor * lamb * lColor;
	
	//FragColor = oColor;
}