#include "ModelAsset.h"

ModelAsset::ModelAsset() :_program(0), _texture(0)
{
}

ModelAsset::ModelAsset(Program * program, Texture * texture)
{
	_program = program;
	_texture = texture;
}

void ModelAsset::m_setProgram(Program* program)
{
	_program = program;
}

Program * ModelAsset::m_getProgram() const
{
	return _program;
}

void ModelAsset::m_setTexture(Texture * texture)
{
	_texture = texture;
}

Texture * ModelAsset::m_getTexture() const
{
	return _texture;
}

void ModelAsset::m_setModel(const glm::mat4 & model)
{
	_model = model;
}

glm::mat4 ModelAsset::m_getModel() const
{
	return _model;
}
