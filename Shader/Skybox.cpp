#include "Skybox.h"

Skybox::Skybox(const std::vector<char*>& texsPaths, const std::vector<GLfloat> vertex):_TEX(0), _VAO(0), _VBO(0)
{
#pragma region LoadTextures

	glGenTextures(1, &_TEX);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _TEX);

	for (int i = 0; i < texsPaths.size(); i++)
	{
		/* Load texture image and check for any error */
		SDL_Surface* texture = IMG_Load(texsPaths.at(i));
		if (texture == NULL)
		{
			std::cout << "Error loading the texture " << texsPaths.at(i) << ": " << SDL_GetError() << "\n" << std::endl;
			exit(-1);
		}

		glTexImage2D
		(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // They are all consecutive
			0,
			GL_RGB,
			texture->w,
			texture->h,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			texture->pixels
		);

		// Free memory
		SDL_FreeSurface(texture);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

#pragma endregion

#pragma region CreateVAOVBO

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertex.size(), &vertex[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

#pragma endregion
}

void Skybox::m_setProgram(Program * program)
{
	_program = program;
}

Program * Skybox::m_getProgram() const
{
	return _program;
}

void Skybox::m_draw(const glm::mat4& matrix)
{
	/* Skybox must be drawn at the background of all the other objects. */
	glDisable(GL_DEPTH_TEST);
	/* Activate program*/
	_program->UseProgram(GL_TRUE);
	/* Send the view projection matrix to the shader */
	glUniformMatrix4fv(_program->m_GetUniformLocation("matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
	/* Send cubemap to the shader */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _TEX);
	glUniform1i(_program->m_GetUniformLocation("skybox"), 0);
	/* Draw the skybox cube. */
	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	/* Re-enable depth testing.  */
	glEnable(GL_DEPTH_TEST);
	/* De-activate program */
	_program->UseProgram(GL_FALSE);
}

Skybox::~Skybox()
{
	if (_TEX != 0)
	{
		glDeleteTextures(1, &_TEX);
	}
	if (_VAO != 0)
	{
		glDeleteVertexArrays(1, &_VAO);
	}
	if (_VBO != 0)
	{
		glDeleteBuffers(1, &_VBO);
	}
}
