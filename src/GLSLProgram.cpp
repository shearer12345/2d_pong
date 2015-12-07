#include "GLSLProgram.hpp"
#include "SDL2/SDL_assert.h"
#include <algorithm>
#include <fstream>

// tag::GLSLProgram[]
GLSLProgram::GLSLProgram(std::string newVertexShaderPath,
                       std::string newFragmentShaderPath)
					   : // initializer list follows
					   vertexShaderPath(newVertexShaderPath),
					   fragmentShaderPath(newFragmentShaderPath)
{
	//assuming only a single GL context

	std::vector<GLuint> shaderList;
	// vertexShaderPath = newVertexShaderPath;
	// fragmentShaderPath = newFragmentShaderPath;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, loadShader(vertexShaderPath)));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, loadShader(fragmentShaderPath)));

	programID = createProgram(shaderList);
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "in constructor, programID %i", programID);

	//clean up shaders (we don't need them anymore as they are not in theProgram
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

	// tag::glGetAttribLocation[]
	positionLocation = glGetAttribLocation(programID, "position");
	vertexColorLocation = glGetAttribLocation(programID, "vertexColor");

	//error handling
	SDL_assert_release( positionLocation != -1);
	SDL_assert_release( vertexColorLocation != -1);
	// end::glGetAttribLocation[]

	// tag::glGetUniformLocation[]
	modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");

	//error handling
	SDL_assert_release( modelMatrixLocation != -1);
	SDL_assert_release( viewMatrixLocation != -1);
	SDL_assert_release( projectionMatrixLocation != -1);
	// end::glGetUniformLocation[]


}
// end::GLSLProgram[]

// tag::~GLSLProgram[]
GLSLProgram::~GLSLProgram()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "GLSL program deletion OK!");
	glDeleteProgram(programID);
}
// end::~GLSLProgram[]

void GLSLProgram::use()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "Using GLSL program with ID %i", programID);
	glUseProgram(programID);
}
// tag::createShader[]
GLuint GLSLProgram::createShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	//error check
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	//error handling
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	SDL_assert_release(status != GL_FALSE);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
			case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
			case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_CRITICAL, "GLSL Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
		SDL_Quit();
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "GLSL Shader created with ID %i", shader);
	return shader;
}
// end::createShader[]

// tag::createProgram[]
GLuint GLSLProgram::createProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	//error handling
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	SDL_assert_release(status != GL_FALSE);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_CRITICAL, "GLSL Linker failure: %s", strInfoLog);
		delete[] strInfoLog;
		SDL_Quit();
	}

	SDL_assert_release(program != 0);

	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "GLSL program created with ID %i", program);
	return program;
}
// end::createProgram[]

// tag::loadShader[]
std::string GLSLProgram::loadShader(const std::string filePath) {
    std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);

	//error handling
	if (!fileStream)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "GLSL Shader could not be loaded - cannot read file - %s", filePath);
        SDL_Quit();
	}

	std::string fileData( (std::istreambuf_iterator<char>(fileStream)),
	                 (std::istreambuf_iterator<char>()          ));

	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "GLSL Shader Loaded from '%s'", filePath.c_str());
	return fileData;
}
// end::loadShader[]
