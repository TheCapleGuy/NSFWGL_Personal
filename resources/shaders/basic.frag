#version 410 
in vec2 vTexCoord;
in vec4 vNormal;
in vec4 vShadowCoord;

out vec4 FragColor;
uniform sampler2D ShadowMap;
uniform sampler2D Diffuse;
uniform vec3 dLColor;
uniform vec3 dLDirection;

void main()
{
	vec4 oColor = texture(Diffuse, vTexCoord);	
	vec4 lColor = vec4(dLColor, 1);	
	vec4 lNormal = normalize(vec4(dLDirection,0));	
	
	float lamb = max(0, dot(-lNormal, vNormal));
	
	if(texture(ShadowMap,vShadowCoord.xy).r < vShadowCoord.z)
	{
		lamb = texture(ShadowMap,vShadowCoord.xy).r; //0;
	}
	
	FragColor = oColor * lamb * lColor;
}