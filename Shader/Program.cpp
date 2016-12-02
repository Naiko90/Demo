#include "Program.h"

Program::Program(std::initializer_list<Shader> shaders/*, std::initializer_list<std::pair<GLuint, std::string>> attributes*/):_object(0)
{
	if (shaders.size() == 0)
	{
		throw std::runtime_error("No shaders were provided to create the program");
	}

	// Once all of the necessary shader objects have been created and compiled, 
	// it is necessary to link them together to create an executable shader program.
	_object = glCreateProgram();
	if (_object == 0)
	{
		throw std::runtime_error("Error in executing glCreateProgram");
	}

	// Associate the shader objects with the just created shader program
	for (Shader s : shaders)
	{
		glAttachShader(_object, s.getID());
	}

	/*// Bind each attribute to a defined location (if any)
	if (attributes.size() > 0)
	{
		m_assignAttributesLocation(attributes);
	}*/

	// Process all shader objects attached to program to generate a completed 
	// shader program
	glLinkProgram(_object);

	// Check if everything went well
	GLint success;
	glGetProgramiv(_object, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		std::string m("Compile error during the linking shaders phase:\n");

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

int Program::m_GetAttributeLocation(const GLchar * attrName) const
{
	if (attrName == NULL)
	{
		throw std::runtime_error("attrName equal to NULL");
	}
	
	GLuint attr = glGetAttribLocation(_object, attrName);

	if (attr == -1)
	{
		std::runtime_error(std::string("Could not retrieve attribute: ") + attrName);
	}

	return attr;

	/* for (auto& iter : _attributesLocation)
	{
		if (iter.second.compare(attrName) == 0)
		{
			return iter.first;
		}
	}

	// If we arrive here it's because the attrName could not be found
	std::runtime_error(std::string("Could not retrieve attribute: ") + attrName);

	return -1;*/
}

int Program::m_GetUniformLocation(const GLchar * uniformName) const
{
	if (uniformName == NULL)
	{
		throw std::runtime_error("uniformName equal to NULL");
	}

	int uniform = glGetUniformLocation(_object, uniformName);

	if (uniform == -1)
	{
		std::runtime_error(std::string("Could not retrieve uniform: ") + uniformName);
	}

	return uniform;
}

GLuint Program::getID() const
{
	return _object;
}

void Program::UseProgram(GLboolean status)
{
	if (status == GL_TRUE)
	{
		glUseProgram(_object);
	}
	else
	{
		glUseProgram(0);
	}
}

/* void Program::m_assignAttributesLocation(const std::initializer_list<std::pair<GLuint, std::string>>& attributes)
{
	for each (auto a in attributes)
	{
		_attributesLocation[a.first] = a.second;
		glBindAttribLocation(_object, a.first, a.second.c_str());
	}
}*/

Program::~Program()
{
	// Might be 0 if something went wrong
	if (_object != 0)
	{
		glDeleteProgram(_object);
	}
}