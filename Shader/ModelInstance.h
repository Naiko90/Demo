#ifndef MODEL_INSTANCE_H
#define MODEL_INSTANCE_H

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "ModelAsset.h"

class ModelInstance : public Cube
{
public:
	ModelInstance();
	~ModelInstance();

	/**
	Set model matrix for this specific instance of the model asset

	@param model Model matrix
	*/
	void m_setModel(const glm::mat4& model);
	
	/**
	Virtual method to be used to draw the model.

	@param cameraMatrix Camera projection matrix
	*/
	void m_draw(const glm::mat4& cameraMatrix);

private:
	Cube* asset;
	glm::mat4 _model;
};

#endif // !MODEL_INSTANCE_H

