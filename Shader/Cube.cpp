#include "Cube.h"

Cube::Cube() :
	ModelAsset(0, 0),
	_VBO(),
	_VAO(0),
	_drawType(GL_TRIANGLES),
	_drawStart(0),
	_drawCount(0),
	_bufferUsage(GL_STATIC_DRAW)
{}

Cube::Cube(Program * program, Texture * texture, GLenum drawType, GLuint drawStart, GLuint drawCount, GLenum bufferUsage) :
	ModelAsset(program, texture),
	_VBO(),
	_VAO(0),
	_drawType(drawType),
	_drawStart(drawStart),
	_drawCount(drawCount),
	_bufferUsage(bufferUsage)
{}

Cube::~Cube()
{
	if (_VBO != 0)
	{
		glDeleteBuffers(2, _VBO);
	}
	if (_VAO != 0)
	{
		glDeleteVertexArrays(1, &_VAO);
	}
}

void Cube::m_createVertexArrayObject(const std::vector<GLfloat>& vertexData)
{
	// Creates VAO
	glGenVertexArrays(1, &_VAO);
	// Binds VAO so to use it
	glBindVertexArray(_VAO);
	
	// Generates VBOs
	glGenBuffers(2, _VBO);

	// Binds VBO
	glBindBuffer(GL_ARRAY_BUFFER, _VBO[POS_VB]);
	// Adds data to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	// Setup the vertex attributes pointer
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

	// Binds VBO
	glBindBuffer(GL_ARRAY_BUFFER, _VBO[TEXCOORD_VB]);
	// Adds data to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	// Setup the vertex attributes pointer
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
}

void Cube::m_computeModelMatrix(GLfloat t, GLfloat r, GLfloat s)
{
	float angle = SDL_GetTicks() / 1000.0 * glm::radians(r * 15.0);

	/* Define rotation matrix. */
	glm::mat4 rotate =
		glm::rotate(glm::mat4(1.0f), angle*3.0f, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0f), angle*2.0f, glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1.0f), angle*4.0f, glm::vec3(0, 0, 1));

	/* Scale the object. */
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(s));

	/* Define Model Matrix
	Push the cube a bit in the background. */
	glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(t, 0.0, -5.0));

	_model = translate * scale * rotate;
}


void Cube::m_draw(const glm::mat4 & cameraMatrix)
{
	// Bind the shader
	_program->UseProgram(GL_TRUE);

	// Compute MVP
	glm::mat4 mvp = cameraMatrix * _model;

	// Set the shader uniforms
	glUniformMatrix4fv(_program->m_GetUniformLocation("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	// Bind the texture
	_texture->m_bindTexture(GL_TEXTURE0);
	glUniform1i(_program->m_GetUniformLocation("tex"), 0);

	// Bind VAO and draw
	glBindVertexArray(_VAO);
	glDrawArrays(_drawType, _drawStart, _drawCount);

	// Clean everything
	glBindVertexArray(0);
	_program->UseProgram(GL_FALSE);
}
