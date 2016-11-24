#include "Shader.h"

Shader::Shader(const std::string& shaderCode, GLenum shaderType):_object(0)
{
	// Create a shader object
	_object = glCreateShader(shaderType);
	if (_object == 0)
	{
		throw std::runtime_error("Error creating shader type " + shaderType);
	}

	// Associate the source of the shader with the shader object just created
	const char* code = shaderCode.c_str();
	glShaderSource(_object, 1, (const GLchar**)&code, NULL);

	// Compile shader
	glCompileShader(_object);

	// Check if everything went well
	GLint success;
	glGetShaderiv(_object, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		std::string m("Compile error in creating a shader:\n");

		GLint infoLogLength;
		glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(_object, infoLogLength, NULL, strInfoLog);
		m += strInfoLog;
		delete[] strInfoLog;

		glDeleteShader(_object);
		_object = 0;
		throw std::runtime_error(m);
	}
}

Shader Shader::shaderFromFile(const std::string& filePath, GLenum shaderType)
{
	// Open file
	std::ifstream f(filePath.c_str());
	if (!f.is_open())
	{
		throw std::runtime_error("Error in opening the file " + filePath + "\n");
	}

	// Read all file into a stringstring buffer
	std::stringstream b;
	b << f.rdbuf();

	// Create new Shader object and return it
	Shader s(b.str(), shaderType);
	return s;
}

GLenum Shader::getID() const
{
	return _object;
}

Shader::~Shader()
{
	// Might be 0 if something went wrong
	if (_object != 0)
	{
		glDeleteProgram(_object);
	}
}
