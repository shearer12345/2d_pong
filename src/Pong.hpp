#pragma once

#include "Game.hpp"

#include "GLSLProgram.hpp"

#include "Mesh.hpp"
#include "Triangle.hpp"
#include "Quad.hpp"

#include "Model.hpp"
#include "AABB.hpp"
#include "OrthoCamera.hpp"
#include "TrackingOrthoCamera.hpp"
#include "PerspectiveCamera.hpp"

class Pong : public Game {
public:

    GLSLProgram* glslProgram = nullptr;
    std::vector<Model*> modelList;
    std::vector<Camera*> cameraList;

    Model* batLeft = nullptr;
    Model* batRight = nullptr;

    GLfloat batSpeed = 50.0f;

    Pong(std::string windowName = "Pong");
    ~Pong();

protected:
    void updateSimulation(double simLength = 0.02) override;
    void render() override;
    void handleInput() override;
};
