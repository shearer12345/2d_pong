#pragma once

#include "Game.hpp"

#include "GLSLProgram.hpp"

#include "Texture.hpp"
#include "Mesh.hpp"
#include "Triangle.hpp"
#include "Quad.hpp"
#include "Circle.hpp"

#include "Model.hpp"
#include "AABB.hpp"
#include "OrthoCamera.hpp"
#include "TrackingOrthoCamera.hpp"

class Pong : public Game {
public:

    GLSLProgram* glslProgram = nullptr;
    std::vector<Model*> modelList;
    std::vector<Model*> scoreMarkerList;

    OrthoCamera * orthoCamera = nullptr;
    TrackingOrthoCamera * trackingCamera = nullptr;
    Camera * camera = orthoCamera;

    Texture* textureOpenGL = nullptr;
    Mesh* quad = nullptr;
    Mesh * triangle = nullptr;

    Model* ball = nullptr;
    Model* batLeft = nullptr;
    Model* batRight = nullptr;
    Model* wallLeft = nullptr;
    Model* wallRight = nullptr;

    GLfloat batSpeed = 100.0f;

    glm::ivec2 score = glm::vec2(0);
    Pong(std::string windowName = "Pong");
    ~Pong();

protected:
    const int scoreLimit = 5;
    void updateSimulation(double simLength = 0.02) override;
    void render() override;
    void handleInput() override;
    void handleBallCollision(Model * other);
    void handleBatCollision(Model * bat, Model * other);
    void incrementScore(int player);
};
