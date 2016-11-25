#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

#include <gl\glew.h>
#include <gl\freeglut.h>
#include <glm\fwd.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <SDL.h>
#include <SDL_image.h>

#include "Shader.h"
#include "Program.h"
#include "Camera.h"

/* Constants */
const char* cubeVShaderFileName = "shaders\\cube.v.glsl";
const char* cubeFShaderFileName = "shaders\\cube.f.glsl";
const char* skyboxVShaderFileName = "shaders\\skybox.v.glsl";
const char* skyboxFShaderFileName = "shaders\\skybox.f.glsl";
const char* textureFileName = "textures\\tex.png";
const std::vector<char*> cubemapTextureFilenames = 
{ 
	"textures\\right.JPG",
	"textures\\left.JPG",
	"textures\\top.JPG",
	"textures\\bottom.JPG",
	"textures\\back.JPG",
	"textures\\front.JPG" 
};

/* Globals */
glm::vec2 SCREEN_SIZE(1280, 768);
GLuint VBO_Cube;
GLuint VBO_Cube_Texture;
GLuint VBO_Skybox;
GLuint VAO_Skybox;
GLuint IBO_Cube;
GLuint textureHandle;
GLuint cubemapHandle;
Program* cubeProgram = NULL;
Program* skyboxProgram = NULL;
GLuint attribute_vertCoord, attribute_vertTextCoord, attribute_skyboxVertCoord;
GLuint uniform_mvp, uniform_texture, uniform_mvp_skybox, uniform_skybox;
Camera* camera = NULL;
GLfloat lastTime = 0.0;
GLfloat currentTime = 0.0;
GLfloat deltaTime = 0.0;
GLint xCoord = SCREEN_SIZE.x / 2;
GLint yCoord = SCREEN_SIZE.y / 2;

void CreateVertexBuffer()
{
	//    v7----- v6
	//   /|      /|
	//  v3------v2|
	//  | |     | |
	//  | |v4---|-|v5
	//  |/      |/
	//  v0------v1
	GLfloat vertices[] =
	{
		// Front
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// Top
		-1.0,  1.0,  1.0,
		1.0,  1.0,  1.0,
		1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
		// Back
		1.0, -1.0, -1.0,
		-1.0, -1.0, -1.0,
		-1.0,  1.0, -1.0,
		1.0,  1.0, -1.0,
		// Bottom
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, -1.0,  1.0,
		-1.0, -1.0,  1.0,
		// Left
		-1.0, -1.0, -1.0,
		-1.0, -1.0,  1.0,
		-1.0,  1.0,  1.0,
		-1.0,  1.0, -1.0,
		// Right
		1.0, -1.0,  1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
		1.0,  1.0,  1.0,
	};

	glGenBuffers(1, &VBO_Cube);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void CreateIndexBuffer()
{
	GLubyte indices[] =
	{
		// front
		0,  1,  2,
		2,  3,  0,
		// top
		4,  5,  6,
		6,  7,  4,
		// back
		8,  9, 10,
		10, 11,  8,
		// bottom
		12, 13, 14,
		14, 15, 12,
		// left
		16, 17, 18,
		18, 19, 16,
		// right
		20, 21, 22,
		22, 23, 20,
	};

	glGenBuffers(1, &IBO_Cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Cube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void CreateTextureBuffer()
{
	/* Load texture image and check for any error */
	SDL_Surface* texture = IMG_Load(textureFileName);
	if (texture == NULL)
	{
		std::cout << "Error loading the texture " << textureFileName << ": " << SDL_GetError() << "\n" << std::endl;
		exit(-1);
	}

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

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

void CreateIndexTextureBuffer()
{
	GLfloat texcoord[2 * 4 * 6] =
	{
		// Front
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0
	};

	/* Same coords for all the faces. */
	for (int i = 1; i < 6; i++)
	{
		memcpy(&texcoord[i * 4 * 2], &texcoord[0], 2 * 4 * sizeof(GLfloat));
	}

	glGenBuffers(1, &VBO_Cube_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube_Texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_STATIC_DRAW);
}

void CreateCubemapTextureBuffer()
{
	glGenTextures(1, &cubemapHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapHandle);

	for (int i = 0; i < cubemapTextureFilenames.size(); i++)
	{
		/* Load texture image and check for any error */
		SDL_Surface* texture = IMG_Load(cubemapTextureFilenames.at(i));
		if (texture == NULL)
		{
			std::cout << "Error loading the texture " << cubemapTextureFilenames.at(i) << ": " << SDL_GetError() << "\n" << std::endl;
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
}

void CreateIndexCubemapTextureBuffer()
{
	GLfloat skyboxVertices[] = {
		       
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &VAO_Skybox);
	glGenBuffers(1, &VBO_Skybox);
	glBindVertexArray(VAO_Skybox);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Skybox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}

void init()
{
	/* Define reset background color */
	glClearColor(0.5, 0.5, 0.5, 0.0);
	/* Cull triangles whose normal is not towards the camera */
	glEnable(GL_CULL_FACE);
	/* Enable other capabilities */
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Create buffers */
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateTextureBuffer();
	CreateIndexTextureBuffer();
	CreateCubemapTextureBuffer();
	CreateIndexCubemapTextureBuffer();

	/* Load the vertex and fragment shaders, and link them together. */
	cubeProgram = new Program({ Shader::shaderFromFile(cubeVShaderFileName, GL_VERTEX_SHADER),
		Shader::shaderFromFile(cubeFShaderFileName, GL_FRAGMENT_SHADER) });
	skyboxProgram = new Program({ Shader::shaderFromFile(skyboxVShaderFileName, GL_VERTEX_SHADER),
		Shader::shaderFromFile(skyboxFShaderFileName, GL_FRAGMENT_SHADER) });

	/* Retrieve the indexes associate with user-defined attribute variables
	and uniform variables. */
	attribute_vertCoord = cubeProgram->m_GetAttributeLocation("vertCoord");
	attribute_vertTextCoord = cubeProgram->m_GetAttributeLocation("vertTextCoord");
	uniform_mvp = cubeProgram->m_GetUniformLocation("mvp");
	uniform_texture = cubeProgram->m_GetUniformLocation("texture");
	attribute_skyboxVertCoord = skyboxProgram->m_GetAttributeLocation("vertCoord");
	uniform_mvp_skybox = skyboxProgram->m_GetUniformLocation("matrix");
	uniform_skybox = skyboxProgram->m_GetUniformLocation("skybox");

	/* Create new camera object and setup it. */
	camera = new Camera();
	camera->m_setPosition(glm::vec3(0, 0, 0));
	camera->m_setViewportAspectRatio(SCREEN_SIZE.x, SCREEN_SIZE.y);
}

void ComputeMVP(GLfloat t, GLfloat sign)
{
	float angle = SDL_GetTicks() / 1000.0 * glm::radians(sign*15.0);

	/* Define rotation matrix. */
	glm::mat4 rotate =
		glm::rotate(glm::mat4(1.0f), angle*3.0f, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0f), angle*2.0f, glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1.0f), angle*4.0f, glm::vec3(0, 0, 1));
	
	/* Scale the object. */
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.75f));

	/* Define Model Matrix
	Push the cube a bit in the background. */
	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(t, 0.0, -4.0));

	/* Compute a global transformation matrix to be applied on each vertex to get the final
	2D point on the screen. */
	glm::mat4 mvp = camera->m_MVP(model * scale * rotate);

	/* Send the matrix to the shader. */
	cubeProgram->UseProgram();
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
}

void Render()
{
	// Compute delta time
	currentTime = SDL_GetTicks();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	// Reset background color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region Skybox

	/* Skybox must be drawn at the background of all the other objects. */
	glDisable(GL_DEPTH_TEST);
	/* Activate program*/
	skyboxProgram->UseProgram();
	/* Compute view projection matrix and send it to the shader */
	glm::mat4 mvp = camera->m_projection() * glm::mat4(glm::mat3(camera->m_view()));
	glUniformMatrix4fv(uniform_mvp_skybox, 1, GL_FALSE, glm::value_ptr(mvp));
	/* send cubemap to the shader */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapHandle);
	glUniform1i(uniform_skybox, 0);
	/* Draw the skybox cube. */
	glBindVertexArray(VAO_Skybox);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	/* Re-enable depth testing.  */
	glEnable(GL_DEPTH_TEST);

#pragma endregion

	cubeProgram->UseProgram();

	/* Enable texture 0. */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glUniform1i(uniform_texture, 0);

	glEnableVertexAttribArray(attribute_vertCoord);
	// Describe the vertices array to OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
	glVertexAttribPointer(
		attribute_vertCoord,	// Attribute
		3,						// Number of element per vertex, i.e. x-y-z
		GL_FLOAT,				// Type of each element
		GL_FALSE,				// Take the value as-is, do not normalize it
		0,						// No extra data between each position
		0						// Offset of first element
	);

	glEnableVertexAttribArray(attribute_vertTextCoord);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube_Texture);
	glVertexAttribPointer(
		attribute_vertTextCoord,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);

	/* Compute offset orientation */
	camera->m_offsetOrientation(SCREEN_SIZE.y / 2 - yCoord, SCREEN_SIZE.x / 2 - xCoord);
	glutWarpPointer(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);

	/* Draw first cube. */
	ComputeMVP(1.5, 1.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Cube);
	GLint size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLubyte), GL_UNSIGNED_BYTE, 0);
	glutPostRedisplay();

	/* Draw second cube. */
	ComputeMVP(-1.5f, -1.6);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLubyte), GL_UNSIGNED_BYTE, 0);
	glutPostRedisplay();

	glDisableVertexAttribArray(attribute_vertCoord);

	glutSwapBuffers();
}

void Finalize()
{
	glDeleteBuffers(1, &VBO_Cube);
	glDeleteBuffers(1, &IBO_Cube);
	glDeleteBuffers(1, &VBO_Cube_Texture);
	glDeleteTextures(1, &textureHandle);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_SIZE.x, SCREEN_SIZE.y);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Demo");

	/* Print instructions */
	std::cout << "Instructions:" << std::endl;
	std::cout << "- Use WASD to move around the scene, and the mouse to change orientation." << std::endl;
	std::cout << "- Press Q to exit the application." << std::endl;

	/* Place cursor in the middle of the screen, and hide it. */
	glutWarpPointer(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);
	glutSetCursor(GLUT_CURSOR_NONE);

	/* Inizialize callbacks */
	glutDisplayFunc(Render);
	glutReshapeFunc([](int width, int height)
	{
		SCREEN_SIZE.x = width;
		SCREEN_SIZE.y = height;
		glViewport(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y);
	});
	glutKeyboardFunc([](unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 'w':
			camera->m_offsetPosition(FORWARD, deltaTime);
			break;
		case 'd':
			camera->m_offsetPosition(RIGHT, deltaTime);
			break;
		case 's':
			camera->m_offsetPosition(BACKWARD, deltaTime);
			break;
		case 'a':
			camera->m_offsetPosition(LEFT, deltaTime);
			break;
		case 'q':
			exit(0);
			break;
		}
	});
	glutMotionFunc([](int x, int y)
	{
		xCoord = x;
		yCoord = y;
	});
	glutPassiveMotionFunc([](int x, int y)
	{
		xCoord = x;
		yCoord = y;
	});

	/* Inizialize Glew. */
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cout << "Error: " << glewGetErrorString(res) << std::endl;
		return -1;
	}

	init();

	// Save current time before entering the game loop
	lastTime = SDL_GetTicks();

	glutMainLoop();

	Finalize();

	return 0;
}