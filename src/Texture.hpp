#pragma once

#include "GLSLProgram.hpp"

class Texture {
public:
  GLuint textureID = 0;
  std::string imagePath;

  Texture(std::string imagePath);

  void invert_image(int pitch, int height, void *image_pixels);

  // void draw(GLenum mode = GL_TRIANGLES);
  // void bufferData(std::vector<Vertex> newVertexData = {});

private:
  // GLuint vertexDataBufferObject = 0;
  // GLuint vertexArrayObject = 0;
  // void setupVertexArrayObject();
};
