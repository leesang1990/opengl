#version 330 core

in vec4 redColor;
in vec3 interColor;
uniform vec4 changeColor; // ��OpenGL����������趨�������

out vec4 color;

void main()
{
   //color = redColor;
   //color = vec4(interColor, 1.0f);
   color = changeColor;
}