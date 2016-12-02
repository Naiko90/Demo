#ifndef MODEL_ASSET_H
#define MODEL_ASSET_H

#include <stdlib.h>
#include <stdio.h>

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Program.h"
#include "Texture.h"

/**
Abstract class representing a textured geometry asset.

Contains the essential information to be used to draw an arbitrary geometry with a single texture:
*/
class ModelAsset
{
public:

	ModelAsset();

	ModelAsset(Program * program, Texture * texture);

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
	Sets texture to be used with the model.

	@param texture Texture
	*/
	void m_setTexture(Texture* texture);

	/**
	Gets texture object.

	@result Texture
	*/
	Texture* m_getTexture() const;

	/**
	Set model matrix.

	@param model Model matrix
	*/
	void m_setModel(const glm::mat4& model);

	/**
	Get model matrix.

	@result Model matrix
	*/
	glm::mat4 m_getModel() const;

	/**
	Pure virtual method to be implemented in any derived class.
	*/
	virtual void m_draw(const glm::mat4& cameraMatrix) = 0;

protected:
	Program* _program;
	Texture* _texture;
	glm::mat4 _model;
};

#endif // !MODEL_ASSET_H

