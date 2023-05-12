#version 450 core
out vec4 FragColor;

in vec4 lineColor;

// texture samplers

void main()
{
	// linearly interpolate between both textures
	FragColor =lineColor;
}