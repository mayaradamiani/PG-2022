#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 inputColor;


void main()
{

	//...pode ter mais linhas de código aqui!
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
    inputColor = vec4(color.r, color.g, color.b, 1.0);

}