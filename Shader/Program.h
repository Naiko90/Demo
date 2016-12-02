#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <map>

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
	Program(std::initializer_list<Shader> shaders/*, std::initializer_list<std::pair<GLuint, std::string>> attributes*/);

	/**
	Retrieve the index associate with user-defined attribute variable
	within the program.

	@param attrName name of the attribute variable to be looked for
	@return Index associated to the attribute variable. -1 will be returned is the attribute does not exist.
	*/
	int m_GetAttributeLocation(const GLchar* attrName) const;

	/**
	Retrieve the index associate with the uniform variable defined within
	the program.

	@param uniformName name of the uniform variable to be looked for
	@return Index associated to the uniform variable. -1 will be returned is the attribute does not exist.
	*/
	int m_GetUniformLocation(const GLchar* uniformName) const;

	/**
	Retrievies program ID

	@result The progra,=m's object ID, as returned from glCreateProgram
	*/
	GLuint getID() const;

	/**
	Activate/De-activate program

	@param active Status
	*/
	void UseProgram(GLboolean status);

	~Program();

private:
	GLenum _object;
	std::map<GLuint, std::string> _attributesLocation;

	/**
	Assign each attribute a specific location.

	@param attributes List of pair, one for each attribute we want to define. The first element of the pair is the location we want to assign to an attribute, while the second element it the name of the attribute as defined in the shader.
	*/
	/* void m_assignAttributesLocation(const std::initializer_list<std::pair<GLuint, std::string>>& attributes);*/
};

#endif // !PROGRAM_H
