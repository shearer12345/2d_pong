#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

class GLSLProgram {
public:
  std::string vertexShaderPath;
  std::string fragmentShaderPath;

  GLuint programID = 0;

  // attribute locations
  GLint vertexPositionLocation = -1;
  GLint vertexColorLocation = -1;
  GLint vertexUVLocation = -1;

  // uniform location
  GLint modelMatrixLocation = -1;
  GLint viewMatrixLocation = -1;
  GLint projectionMatrixLocation = -1;
  GLint textureSamplerLocation = -1;

  GLSLProgram(
      std::string newVertexShaderPath = "shaders/vertexShader.glsl",
      std::string newFragmentShaderPath = "shaders/fragmentShader.glsl");
  ~GLSLProgram();

  void use();

private:
  std::string loadShader(const std::string filePath);
  GLuint createShader(GLenum eShaderType, const std::string &strShaderFile);
  GLuint createProgram(const std::vector<GLuint> &shaderList);
};
