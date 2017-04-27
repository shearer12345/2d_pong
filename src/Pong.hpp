#pragma once

#include "GLSLProgram.hpp"
#include "Game.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "OrthoCamera.hpp"
#include "Quad.hpp"
#include "Texture.hpp"
#include "TrackingOrthoCamera.hpp"
#include "Triangle.hpp"

class Pong : public Game {
public:
  GLSLProgram *glslProgram = nullptr; // I've used standard pointers throughout,
                                      // unique_ptr or shared_ptr would be
                                      // better practice
  std::vector<Model *> modelList;
  std::vector<Model *> scoreMarkerList;

  OrthoCamera *orthoCamera = nullptr;
  TrackingOrthoCamera *trackingCamera = nullptr;
  Camera *camera = orthoCamera; // pointer to the active camera

  Mesh *quad = nullptr;
  Mesh *triangle = nullptr;

  Model *ball = nullptr;
  Model *batLeft = nullptr;
  Model *batRight = nullptr;
  Model *wallLeft = nullptr;
  Model *wallRight = nullptr;

  GLfloat batSpeed = 100.0f;

  glm::ivec2 score = glm::vec2(0);
  Pong(std::string windowName = "Pong");
  ~Pong();

protected:
  const int scoreLimit = 5;
  void updateSimulation(double simLength = 0.02) override;
  void render() override;
  void handleInput() override;
  void handleBallCollision(Model *other);
  void handleBatCollision(Model *bat, Model *other);
  void incrementScore(int player);
};
