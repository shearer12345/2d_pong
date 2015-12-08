#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "GLSLProgram.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "OrthoCamera.hpp"
#include "TrackingOrthoCamera.hpp"
#include "PerspectiveCamera.hpp"

class Pong {
public:
    std::string windowName;
    SDL_Window *sdl_windowPtr; //pointer to the SDL_Window
    SDL_GLContext context = NULL; //the SDL_GLContext

    int frameCount = 0;
    std::string frameLine = "";
    bool done = false;

    GLSLProgram* glslProgram = nullptr;
    std::vector<Model*> modelList;
    std::vector<Camera*> cameraList;



    Pong(std::string windowName = "Pong");
    ~Pong();

    void run();
    void run_once();

private:
    void createWindow();
    void createContext();
    void initGlew();
    void setGLAttributes();
    void handleInput();
    void updateSimulation(double simLength = 0.02);
    void preRender();
    void render();
    void postRender();
};
