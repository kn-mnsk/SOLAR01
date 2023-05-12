#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

//out vec3 ourColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 lineColor;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	lineColor =aColor;
}