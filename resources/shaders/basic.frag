#version 410 
in vec2 vTexCoord;
in vec4 vPosition;
in vec4 vNormal;
in vec4 vTangent;
in vec3 vBiTangent;
in vec4 vShadowCoord;

out vec4 FragColor;

//Shadow uniforms
uniform sampler2D ShadowMap;

//texture uniforms
uniform float SpecPow;
uniform sampler2D Specular;
uniform sampler2D Diffuse;
uniform sampler2D Normal;

//Directional light uniforms
uniform vec3 dLColor;
uniform vec3 dLDirection;
uniform float dLCoefficient;

uniform vec3 cameraPos;

void main()
{
	vec2 texelSize = 1.0f / textureSize(ShadowMap, 0);
	
	//texture color
	vec4 oColor = texture(Diffuse, vTexCoord);	
	
	//light color
	vec4 lColor = vec4(dLColor, 1);	
	
	//light normal
	vec4 lNormal = normalize(vec4(dLDirection,0));
	

	//---------------------------------------Ambient Lighting-----------------------------------
	vec4 aColor = oColor * lColor;// * dLCoefficient;
	//------------------------------------------------------------------------------------------

	//---------------------------------------Normal Map-----------------------------------------
	mat3 TBN = mat3(normalize(vTangent.xyz), 
					normalize(vBiTangent), 
					normalize(vNormal.xyz));
	
	vec3 N = TBN * normalize(texture(Normal,vTexCoord).xyz * 2 -1);
	
	float lamb = max(0, dot(N, dLDirection));
	//------------------------------------------------------------------------------------------
	//---------------------------------------Specular Lighting----------------------------------
	vec3 e = normalize(cameraPos - vPosition.xyz);
	
	vec3 r = reflect(-dLDirection, N);
	
	float s = max(0, dot(e,r));
	s = pow(s, SpecPow);
	
	//s = min(1,s);
	//------------------------------------------------------------------------------------------
	//-------------------------------------Shadow Softening-------------------------------------
	
	float penumbra = texture(ShadowMap,vShadowCoord.xy).z * (2/6);
	
	//left neighbor
	if(texture(ShadowMap,vShadowCoord.xy - vec2(texelSize.x, 0)).z > vShadowCoord.z - 0.005f)
	{
		penumbra += texture(ShadowMap,vShadowCoord.xy - vec2(texelSize.x, 0)).z * (1/6);
	}
	//right neighbor 
	if(texture(ShadowMap,vShadowCoord.xy + vec2(texelSize.x, 0)).z > vShadowCoord.z - 0.005f)
	{
		penumbra += texture(ShadowMap, vTexCoord + vec2(texelSize.x, 0)).z * (1/6);
	}	
	//bottom penumbra
	if(texture(ShadowMap,vShadowCoord.xy - vec2(0, texelSize.y)).z > vShadowCoord.z - 0.005f)
	{
		penumbra += texture(ShadowMap,vShadowCoord.xy - vec2(0, texelSize.y)).z;
	}	
	//top penumbra
	if(texture(ShadowMap,vShadowCoord.xy + vec2(0, texelSize.y)).z > vShadowCoord.z - 0.005f)
	{
		penumbra += texture(ShadowMap,vShadowCoord.xy + vec2(0, texelSize.y)).z;
	}
	
	//if(texture(ShadowMap,vShadowCoord.xy).z < vShadowCoord.z - 0.005f)
	//{
		//lamb = 0;
	//}
	
	lamb *= penumbra;
	
	float amb = 0.2f;
	
	//ambientColor * ambCoeff + specularColor * spec + diffuseColor * lambert
//--------------------------------------------------------------------------------------------------
	FragColor = aColor * (amb + lamb + s);
	//FragColor = aColor;
	//FragColor = oColor * lamb * lColor + s * aColor;
	
	//FragColor = vec4(.5f, .5f, .5f, 1) + (oColor * lamb) + (lColor * s);
	FragColor.a = 1;
}

/*
	


*/