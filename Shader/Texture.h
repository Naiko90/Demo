#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdexcept>

#include <gl\glew.h>
#include <SDL.h>
#include <SDL_image.h>

class Texture
{
public:

	/**
	Creates a texture object from a source file.

	@param sourcePath Path of the file to be loaded
	@throws std::exception if any error occurs.
	*/
	Texture(const char* sourcePath);

	/**
	Deletes the texture object with glDeleteTextures
	*/
	~Texture();

	/**
	Bind texture

	@param textureUnit Specifies which texture unit to make active. It must be one of GL_TEXTUREi, where i ranges from zero to the value of GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS minus one. The initial value is GL_TEXTURE0. 
	@param samplerBuffer 
	*/
	void m_bindTexture(GLenum textureUnit);

private:
	GLuint _TEX;
};

#endif // !TEXTURE_H

