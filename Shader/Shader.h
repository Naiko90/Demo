#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <GL\glew.h>

class Shader
{
public:
	/**
	Creates a shader from a source code string.

	@param shaderCode  The source code for the shader.
	@param shaderType  GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
	@throws std::exception if any error occurs.

	*/
	Shader(const std::string& shaderCode, GLenum shaderType);

	/**
	Creates a shader from a text file.

	@param filePath    The path to the text file containing the shader source.
	@param shaderType  GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
	@throws std::exception if any error occurs.
	*/
	static Shader shaderFromFile(const std::string& filePath, GLenum shaderType);

	/**
	Retrievies shader ID

	@result The shader's object ID, as returned from glCreateShader
	*/
	GLuint getID() const;

	~Shader();

private:
	GLuint _object;
};

#endif // !SHADER_H

