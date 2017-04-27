#pragma once

#include "SDL.h"
#include <string>

class Game {
public:
  std::string windowName;
  SDL_Window *sdl_windowPtr;    // pointer to the SDL_Window
  SDL_GLContext context = NULL; // the SDL_GLContext

  int frameCount = 0;
  std::string frameLine = "";
  bool done = false;

  Game(std::string windowName = "Game");
  ~Game();

  void run();
  void run_once();

protected:
  void createWindow();
  void createContext();
  void initImageLoading();
  void initGlew();
  void setGLAttributes();
  virtual void handleInput();
  virtual void updateSimulation(double simLength = 0.02);
  void preRender();
  virtual void render();
  void postRender();
};
