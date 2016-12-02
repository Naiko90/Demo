#include "Texture.h"

Texture::Texture(const char* sourcePath)
{
	/* Loads texture image and check for any error */
	SDL_Surface* texture = IMG_Load(sourcePath);
	if (texture == NULL)
	{
		throw std::runtime_error("Error loading the texture " + std::string(sourcePath) + ": " + SDL_GetError() + "\n");
	}
	
	glGenTextures(1, &_TEX);
	glBindTexture(GL_TEXTURE_2D, _TEX);

	/* When MAGnifying the image, use LINEAR filtering. */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/* When MINifying the image, use LINEAR filtering. */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,		// Target
		0,					// Level. 0 = one resolution, no mipmap
		GL_RGBA,			// Internal format
		texture->w,			// Width
		texture->h,			// Height
		0,					// Border. Always 0 in OpenGL ES
		GL_RGBA,			// Format
		GL_UNSIGNED_BYTE,	// Type
		texture->pixels
	);

	// Free memory
	SDL_FreeSurface(texture);
}

Texture::~Texture()
{
	if (_TEX != 0)
	{
		glDeleteTextures(1, &_TEX);
	}
}

void Texture::m_bindTexture(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, _TEX);
}
