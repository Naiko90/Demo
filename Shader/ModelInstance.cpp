#include "ModelInstance.h"

ModelInstance::ModelInstance() : Cube()
{
}

ModelInstance::~ModelInstance()
{
}

void ModelInstance::m_setModel(const glm::mat4 & model)
{
	_model = model;
}

void ModelInstance::m_draw(const glm::mat4& cameraMatrix)
{
	// Bind the shader
	m_getProgram()->UseProgram(GL_TRUE);

	// Compute MVP
	glm::mat4 mvp = cameraMatrix * _model;

	// Set the shader uniforms
	glUniformMatrix4fv(m_getProgram()->m_GetAttributeLocation("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform1i(m_getProgram()->m_GetAttributeLocation("tex"), 0); // Set to 0 because the texture will be bound to GL_TEXTURE0

	// Bind the texture
	m_getTexture()->m_bindTexture(GL_TEXTURE0);

	// Bind VAO and draw
	m_bindVAO();
	glDrawArrays(_drawType, _drawStart, _drawCount);

	// Unbind everything
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	m_getProgram()->UseProgram(GL_FALSE);
}
