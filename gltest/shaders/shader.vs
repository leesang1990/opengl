#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 redColor;
out vec3 interColor; //Interpolated


void main()
{
    gl_Position = vec4(position, 1.0f);
	//redColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);
	//interColor = color;
}