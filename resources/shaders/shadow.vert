#version 410

layout(location = 0) in vec4 Position;


uniform mat4 LightMatrix;
uniform mat4 Model;

void main()
{
	gl_Position = LightMatrix * Model * Position;
}


//output to an image should be in clip space (-1,1)
//input from an image must be in texture (UV) space (0,1)