#ifndef CUBE_H
#define CUBE_H

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "ModelAsset.h"

/**
Class representing a textured cube asset.
*/
class Cube : public ModelAsset
{
public:

	/**
	Instantiates a new model asset
	*/
	Cube();

	/**
	Instantiates a new model asset with the desired parameters
	*/
	Cube(Program* program, Texture* texture, GLenum drawType, GLuint drawStart, GLuint drawCount, GLenum bufferUsage);

	/**
	De-allocates all the resources.
	*/
	virtual ~Cube();

	/**
	Generates the VAO.

	@param vertexData Vertex data
	*/
	void m_createVertexArrayObject(const std::vector<GLfloat>& vertexData);

	/**
	Compute model matrix.

	@param t Translate
	@param r Rotate
	@param s Scale
	*/
	void m_computeModelMatrix(GLfloat t, GLfloat r, GLfloat s);

	/**
	Draw the model. This is unique for any geometry.

	@param cameraMatrix Camera projection matrix
	*/
	void m_draw(const glm::mat4& cameraMatrix);	

private:

	static const GLuint POSITION_LOCATION = 0;
	static const GLuint TEX_COORD_LOCATION = 1;
	static const GLuint POS_VB = 0;
	static const GLuint TEXCOORD_VB = 1;
	GLuint _VAO;
	GLuint _VBO[2];
	GLenum _drawType;
	GLint _drawStart;
	GLint _drawCount;
	GLenum _bufferUsage;

};

#endif // !CUBE_H

