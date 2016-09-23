#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#define Materials  main

#define CURRENT_DIR(picName) ("E:\\visual studio 2013\\Projects\\opengl\\Debug\\"##picName)


class Shader
{
public:
	GLuint Program;
	Shader(const GLchar* vertexPath,const GLchar* fragmentPath);
	void Use();

	GLint getUniformLocation(const std::string& attributeName) const;
	void setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);
	static const char* UNIFORM_COLOR;
	static const char* UNIFORM_TEXTURE;
	static const GLchar* vertexShaderSource;
	static const GLchar* fragmentShaderSource;
};

//SOIL.lib; opengl32.lib; glfw3.lib; glew32s.lib
//..\soil\src;..\glew-1.11.0\include;..\glfw-3.2\include;
//..\soil\lib;..\glew-1.11.0\lib;..\glfw-3.2\lib;

#endif //SHADER_H

//https://github.com/StarEngine/engine/blob/develop/src/Sound/AudioManager.cpp
//https://github.com/ejoy/ejoy2d
//http://learnopengl-cn.readthedocs.io/zh/latest/