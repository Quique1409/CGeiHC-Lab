#version 330
layout (location =0) in vec3 pos;
out vec4 vColor;
uniform mat4 model;
uniform mat4 projection;
void main()
{
	gl_Position=projection*model*vec4(pos,1.0f);
	//vColor=vec4(color,1.0f);
	vColor=vec4(0.0f,0.5f,0.0f,1.0f);
}