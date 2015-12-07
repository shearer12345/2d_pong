#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <vector>

class GLSLProgram {
public:
	std::string vertexShaderPath;
	std::string fragmentShaderPath;

	GLuint programID;

	//attribute locations
	GLint positionLocation; //GLuint that we'll fill in with the location of the `position` attribute in the GLSL
	GLint vertexColorLocation; //GLuint that we'll fill in with the location of the `vertexColor` attribute in the GLSL

	//uniform location
	GLint modelMatrixLocation;
	GLint viewMatrixLocation;
	GLint projectionMatrixLocation;

	GLSLProgram(std::string newVertexShaderPath = "assets/shaders/vertexShader.glsl", std::string newFragmentShaderPath = "assets/shaders/fragmentShader.glsl");
	~GLSLProgram();

	void use();

private:
	std::string loadShader(const std::string filePath);
	GLuint createShader(GLenum eShaderType, const std::string &strShaderFile);
	GLuint createProgram(const std::vector<GLuint> &shaderList);
};
