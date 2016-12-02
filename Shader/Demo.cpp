#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

#include <gl\glew.h>
#include <gl\freeglut.h>
#include <glm\fwd.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Shader.h"
#include "Program.h"
#include "Camera.h"
#include "Cube.h"
#include "Skybox.h"

/* Constants */
const char* cubeVShaderFileName = "shaders\\cube.v.glsl";
const char* cubeFShaderFileName = "shaders\\cube.f.glsl";
const char* skyboxVShaderFileName = "shaders\\skybox.v.glsl";
const char* skyboxFShaderFileName = "shaders\\skybox.f.glsl";
const char* sourcePathtextureFileName = "textures\\tex.png";
const std::vector<char*> cubemapTextureFilenames = 
{ 
	"textures\\right.JPG",
	"textures\\left.JPG",
	"textures\\top.JPG",
	"textures\\bottom.JPG",
	"textures\\back.JPG",
	"textures\\front.JPG" 
};
const GLfloat zoomSensitivity = -0.005;
const GLuint N_CUBE = 2;

/* Globals */
glm::vec2 SCREEN_SIZE(1280, 768);
Camera* camera = nullptr;
std::vector<ModelAsset*> modelAsset;
Skybox* skybox = nullptr;

/* Input callbacks data */
GLfloat lastTime = 0.0;
GLfloat currentTime = 0.0;
GLfloat deltaTime = 0.0;
GLint xCoord = SCREEN_SIZE.x / 2;
GLint yCoord = SCREEN_SIZE.y / 2;
GLint yScroll = 0;

void CreateSkybox()
{
	std::vector<GLfloat> vertices = {

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
	Program* skyboxProgram = new Program({ Shader::shaderFromFile(skyboxVShaderFileName, GL_VERTEX_SHADER),
		Shader::shaderFromFile(skyboxFShaderFileName, GL_FRAGMENT_SHADER) });

	skybox = new Skybox(cubemapTextureFilenames, vertices);
	skybox->m_setProgram(skyboxProgram);
}

void CreateInstances()
{
	std::vector<GLfloat> vertexData =
	{
		//  X     Y     Z       U     V

		// Bottom
		-1.0f,-1.0f,-1.0f,		0.0f, 0.0f,
		 1.0f,-1.0f,-1.0f,		1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,		0.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,		1.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,		1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,		0.0f, 1.0f,

		// Top
		-1.0f, 1.0f,-1.0f,		0.0f, 0.0f,			
		-1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,		1.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,		1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,

		// Front
		-1.0f,-1.0f, 1.0f,		1.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,		0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,		0.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f,

		// Back
		-1.0f,-1.0f,-1.0f,		0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,		0.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,		1.0f, 0.0f,
		 1.0f,-1.0f,-1.0f,		1.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,		0.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,		1.0f, 1.0f,

		// Left
		-1.0f,-1.0f, 1.0f,		0.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,		1.0f, 0.0f,
		-1.0f,-1.0f,-1.0f,		0.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,		0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,		1.0f, 0.0f,

		// Right
		1.0f,-1.0f, 1.0f,		1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,		1.0f, 0.0f,
		1.0f, 1.0f,-1.0f,		0.0f, 0.0f,
		1.0f,-1.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,		0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 1.0f
	};

	// Load the vertex and fragment shaders, and link them together.
	Program* cubeShaders = new Program({ Shader::shaderFromFile(cubeVShaderFileName, GL_VERTEX_SHADER),
		Shader::shaderFromFile(cubeFShaderFileName, GL_FRAGMENT_SHADER) });

	// Load the texture associated with the model asset object
	Texture* cubeTexture = new Texture(sourcePathtextureFileName);

	// Instantiates n new cube objects
	for (int i = 0; i < N_CUBE; i++)
	{
		// 6 faces * 2 triangles * 3 coordinates
		Cube* a = new Cube(cubeShaders, cubeTexture, GL_TRIANGLES, 0, 6 * 2 * 3, GL_STATIC_DRAW);
		a->m_createVertexArrayObject(vertexData);
		modelAsset.push_back(a);
	}
}

void init()
{
	/* Define reset background color */
	glClearColor(0.5, 0.5, 0.5, 0.0);
	/* Cull triangles whose normal is not towards the camera */
	glEnable(GL_CULL_FACE);
	/* Enable other capabilities */
	glEnable(GL_DEPTH_TEST);
	// GL_BLEND cause problems in drawing the Cube objects
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Create elements */
	CreateSkybox();
	CreateInstances();

	/* Create new camera object and setup it. */
	camera = new Camera();
	camera->m_setPosition(glm::vec3(0, 0, 0));
	camera->m_setViewportAspectRatio(SCREEN_SIZE.x, SCREEN_SIZE.y);
}

void Render()
{
	// Compute delta time
	currentTime = SDL_GetTicks();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	// Reset background color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw skybox
	glm::mat4 mvp = camera->m_projection() * glm::mat4(glm::mat3(camera->m_view()));
	skybox->m_draw(mvp);

	/* Compute offset orientation */
	camera->m_offsetOrientation(SCREEN_SIZE.y / 2 - yCoord, SCREEN_SIZE.x / 2 - xCoord);
	glutWarpPointer(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);

	/* Draw first cube. */
	((Cube*)modelAsset[0])->m_computeModelMatrix(1.5, 1.0, 0.6);
	modelAsset[0]->m_draw(camera->m_matrix());
	glutPostRedisplay();

	/* Draw second cube. */
	((Cube*)modelAsset[1])->m_computeModelMatrix(-1.5, -1.6, 0.6);
	modelAsset[1]->m_draw(camera->m_matrix());
	glutPostRedisplay();

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_SIZE.x, SCREEN_SIZE.y);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Demo");

	/* Print instructions */
	std::cout << "Instructions:" << std::endl;
	std::cout << "- Use WASDXZ to move forward, left, backwards, right, up and down, respectively, and the mouse to change orientation." << std::endl;
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
		case 'z':
			camera->m_offsetPosition(DOWN, deltaTime);
			break;
		case 'x':
			camera->m_offsetPosition(UP, deltaTime); 
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
	glutMouseWheelFunc([](int button, int direction, int x, int y)
	{
		yScroll += glm::sign(direction) * y;
	});
	glutIdleFunc([](void)
	{
		GLfloat FOV = camera->m_fieldOfView() + yScroll * zoomSensitivity;
		if (FOV < 5.0) FOV = 5.0;
		if (FOV > 130) FOV = 130.0;
		camera->m_setFieldOfView(FOV);
		yScroll = 0;
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

	return 0;
}