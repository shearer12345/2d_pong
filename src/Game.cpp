#include "Game.hpp"

#include <GL/glew.h>
#include <SDL_image.h>
#include <SDL_log.h>
#include <glm/glm.hpp>

Game::Game(std::string windowName) : windowName(windowName) {
  int init = SDL_Init(SDL_INIT_EVERYTHING);

#ifdef NDEBUG
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
#else
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

  // error handling
  if (init != 0) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n",
                    SDL_GetError());
    SDL_Quit();
  }
  SDL_Log("SDL initialised OK!");

  createWindow();
  createContext();

  initGlew();
  initImageLoading();
}

Game::~Game() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(sdl_windowPtr);
  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO,
                 "SDL Clean up OK!");

  SDL_Quit();
}

void Game::createWindow() {
  // create window
  sdl_windowPtr = SDL_CreateWindow(windowName.c_str(), 100, 100, 600, 600,
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                       SDL_WINDOW_ALLOW_HIGHDPI);

  // error handling
  if (sdl_windowPtr == nullptr) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "SDL_CreateWindow init error: %s\n", SDL_GetError());
    SDL_Quit();
  }

  SDL_Log("SDL CreatedWindow OK!");
}

void Game::setGLAttributes() {
  int major = 4;
  int minor = 5;
  SDL_Log("Asking for OpenGL %d.%d context\n", major, minor);
  // set the opengl context version
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_CORE); // core profile
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);      // default, probably
}

void Game::createContext() {
  setGLAttributes();
  context = SDL_GL_CreateContext(sdl_windowPtr);

  // error handling
  if (context == NULL) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "SDL_GL_CreateContext init error: %s\n", SDL_GetError());
    SDL_Quit();
  }

  int major = -1;
  int minor = -1;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
  SDL_Log("Got an OpenGL %d.%d context\n", major, minor);
}

void Game::initImageLoading() {
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

  auto imgFlagsPost = IMG_Init(imgFlags);

  // error handling
  if (!(imgFlagsPost & imgFlags)) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n",
                    SDL_GetError());
    SDL_Quit();
  }
  SDL_Log("SDL_image initialised OK!\n");
}

void Game::initGlew() {
  // initialise GLEW - sets up the OpenGL function pointers for the version of
  // OpenGL we're using

  glewExperimental = GL_TRUE; // GLEW isn't perfect - see
  // https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
  GLenum rev = glewInit();

  // error handling
  if (GLEW_OK != rev) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "glewInit error: %s\n",
                    glewGetErrorString(rev));
    SDL_Quit();
  }

  SDL_Log("GLEW Init OK!");
}

void Game::handleInput() {
  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE,
                 "handleInput");
  // Event-based input handling
  // The underlying OS is event-based, so **each** key-up or key-down (for
  // example)
  // generates an event.
  //  - https://wiki.libsdl.org/SDL_PollEvent
  // In some scenarios we want to catch **ALL** the events, not just to present
  // state
  //  - for instance, if taking keyboard input the user might key-down two keys
  //  during a frame
  //    - we want to catch based, and know the order
  //  - or the user might key-down and key-up the same within a frame, and we
  //  still want something to happen (e.g. jump)
  //  - the alternative is to Poll the current state with SDL_GetKeyboardState

  SDL_Event event; // somewhere to store an event

  // NOTE: there may be multiple events per frame
  while (SDL_PollEvent(
      &event)) // loop until SDL_PollEvent returns 0 (meaning no more events)
  {
    switch (event.type) {
    case SDL_QUIT:
      done = true; // set donecreate remote branch flag if SDL wants to quit
                   // (i.e. if the OS has triggered a close event,
      //  - such as window close, or SIGINT
      break;

    // keydown handling - we should to the opposite on key-up for direction
    // controls (generally)
    case SDL_KEYDOWN:
      // Keydown can fire repeatable if key-repeat is on.
      //  - the repeat flag is set on the keyboard event, if this is a repeat
      //  event
      //  - in our case, we're going to ignore repeat events
      //  - https://wiki.libsdl.org/SDL_KeyboardEvent
      if (!event.key.repeat)
        switch (event.key.keysym.sym) {
        // hit escape to exit
        case SDLK_ESCAPE: {
          SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO,
                         "Escape key pressed");
          done = true;
        } break;
        }
      break;
    }
  }
}

void Game::updateSimulation(double simLength) // update simulation with an
                                              // amount of time to simulate for
                                              // (in seconds)
{
  // WARNING - we should calculate an appropriate amount of time to simulate -
  // not always use a constant amount of time
  // see, for example,
  // http://headerphile.blogspot.co.uk/2014/07/part-9-no-more-delays.html
  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE,
                 "updateSimulation (for %f seconds)", simLength);
}

void Game::preRender() {
  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE,
                 "preRender");
  int w = 600, h = 600;

  SDL_GetWindowSize(sdl_windowPtr, &w, &h);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,
              GL_ONE_MINUS_SRC_ALPHA); // don't need to do this every frame

  glViewport(0, 0, w, h);               // set viewpoint
  glClearColor(0.5f, 0.0f, 0.0f, 1.0f); // set clear colour
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::render() {
  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "render");
}

void Game::postRender() {
  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE,
                 "postRender");
  SDL_GL_SwapWindow(sdl_windowPtr);
  ; // present the frame buffer to the display (swapBuffers);
}

void Game::run() {
  while (!done) // loop until done flag is set)
  {
    run_once();
  }
}

void Game::run_once() {
  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG,
                 "=== Frame Start %i", frameCount++);

  handleInput(); // this should ONLY SET VARIABLES

  updateSimulation(); // this should ONLY SET VARIABLES according to simulation

  preRender();

  render(); // this should render the world state according to VARIABLES -

  postRender();

  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG,
                 "=== Frame Done!\n\n");
}
