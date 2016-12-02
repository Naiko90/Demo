#ifndef SKYBOX_H
#define SKYBOX_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <gl\glew.h>
#include <gl\freeglut.h>
#include <glm\fwd.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <SDL.h>
#include <SDL_image.h>

#include "Program.h"

class Skybox
{
public:

	/**
	Instantiates a new skybox object.

	@param texsPaths Vector containing the paths of all the 6 textures that will form the skybox.
	@param vertex Vertices to be used to draw the cube. 
	*/
	Skybox(const std::vector<char*>& texsPaths, const std::vector<GLfloat> vertex);

	/**
	Sets shader program.

	@param program Shader program handle.
	*/
	void m_setProgram(Program* program);

	/**
	Gets shader program.

	@result Shader program associated to the model.
	*/
	Program* m_getProgram() const;

	/**
	Draw the skybox.

	@param matrix Camera projection matrix
	*/
	void m_draw(const glm::mat4& matrix);

	~Skybox();

private:
	Program* _program;
	GLuint _VAO;
	GLuint _VBO;
	GLuint _TEX;

};

#endif // !SKYBOX_H

