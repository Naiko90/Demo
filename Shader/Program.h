#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <GL\glew.h>

#include "Shader.h"

class Program
{
public:

	/**
	Creates a program by linking together  list of Shader objects

	@param handles  The shaders to be linked together 
	@throws std::exception if any error occurs.
	*/
	Program(std::initializer_list<Shader> shaders);

	/**
	Retrieve the index associate with user-defined attribute variable
	within the program.

	@param attrName name of the attribute variable to be looked for
	@return Index associated to the attribute variable. -1 will be returned is the attribute does not exist.
	*/
	GLuint m_GetAttributeLocation(const GLchar* attrName) const;

	/**
	Retrieve the index associate with the uniform variable defined within
	the program.

	@param uniformName name of the uniform variable to be looked for
	@return Index associated to the uniform variable. -1 will be returned is the attribute does not exist.
	*/
	GLuint m_GetUniformLocation(const GLchar* uniformName) const;

	/**
	Retrievies program ID

	@result The progra,=m's object ID, as returned from glCreateProgram
	*/
	GLuint getID() const;

	/**
	Activate program
	*/
	void UseProgram();

	~Program();

private:
	GLenum _object;
};

#endif // !PROGRAM_H
