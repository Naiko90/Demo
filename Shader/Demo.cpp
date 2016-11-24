#include <stdlib.h>
#include <stdio.h>

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

/* Constants */
const char* vShaderFileName = "shaders\\cube.v.glsl";
const char* fShaderFileName = "shaders\\cube.f.glsl";
const char* textureFileName = "textures\\tex.png";

/* Globals */
glm::vec2 SCREEN_SIZE(1280, 768);
GLuint VBO_Cube;
GLuint VBO_Cube_Texture;
GLuint IBO_Cube;
GLuint	textureHandle;
Program* gProgram = NULL;
GLuint attribute_vertCoord, attribute_vertTextCoord;
GLuint uniform_mvp, uniform_texture;

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

void init()
{
	/* Define reset background color */
	glClearColor(0.5, 0.5, 0.5, 0.0);
	/* Cull triangles which normal is not towards the camera */
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

	/* Load the vertex and fragment shaders, and link them together. */
	gProgram = new Program({ Shader::shaderFromFile(vShaderFileName, GL_VERTEX_SHADER),
		Shader::shaderFromFile(fShaderFileName, GL_FRAGMENT_SHADER) });
	
	/* Retrieve the indexes associate with user-defined attribute variables
	and uniform variables. */
	attribute_vertCoord = gProgram->m_GetAttributeLocation("vertCoord");
	attribute_vertTextCoord = gProgram->m_GetAttributeLocation("vertTextCoord");
	uniform_mvp = gProgram->m_GetUniformLocation("mvp");
	uniform_texture = gProgram->m_GetUniformLocation("texture");
}

void ComputeMPV(GLfloat t, GLfloat sign)
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

	/* Define View Matrix
	This is a re-implementation of glutLookAt(eye, center, up)
	eye: position of the camera
	center: where the camera is pointed to
	up: top of the camera */
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));

	/* Define Projection Matrix
	This is a re-implementation of gluPerspective(fovy, aspect, zNear, zFar)
	fovy: vertical field of view (45° for a common 60° horizontal FOV in 4:3 resolution)
	aspect: screen aspect ratio (width/height)
	zNear and zFar: clipping plane (min/max depth) */
	glm::mat4 projection = glm::perspective(45.0, 1.0*SCREEN_SIZE.x / SCREEN_SIZE.y, 0.1, 10.0);

	/* Compute a global transformation matrix to be applied on each vertex to get the final 
	2D point on the screen. */
	glm::mat4 mvp = projection*view*model*scale*rotate;
	
	/* Send the matrix to the shader. */
	gProgram->UseProgram();
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
}

void Render()
{
	// Reset background color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gProgram->UseProgram();

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

	/* Draw first cube. */
	ComputeMPV(1.5, 1.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Cube);
	GLint size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLubyte), GL_UNSIGNED_BYTE, 0);
	glutPostRedisplay();

	/* Draw second cube. */
	ComputeMPV(-1.5f, -1.6);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLubyte), GL_UNSIGNED_BYTE, 0);
	glutPostRedisplay();

	glDisableVertexAttribArray(attribute_vertCoord);

	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	SCREEN_SIZE.x = width;
	SCREEN_SIZE.y = height;
	glViewport(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y);
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

	/* Inizialize callbacks */
	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);

	/* Inizialize Glew. */
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cout << "Error: " << glewGetErrorString(res) << std::endl;
		return -1;
	}

	init();

	glutMainLoop();

	Finalize();

	return 0;
}