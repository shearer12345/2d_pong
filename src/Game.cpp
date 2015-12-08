#include "Game.hpp"

// tag::includes[]
#include <iostream>

#include <iterator>
#include <vector>


#include <SDL2/SDL_log.h>

#define GLM_FORCE_RADIANS // suppress a warning in GLM 0.9.5
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game::Game(std::string windowName)
			:
			windowName(windowName)
{
	int init = SDL_Init(SDL_INIT_EVERYTHING);

	#ifdef NDEBUG
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
	#else
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
	#endif

	// error handling
	if (init != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "SDL initialisation failed!");
		SDL_Quit();
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_INFO, "SDL initialised OK!");

	createWindow();

	createContext();

	initGlew();

	glViewport(0,0,600,600); //should check what the actual window res is?
}

Game::~Game()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(sdl_windowPtr);
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "SDL Clean up OK!");

	SDL_Quit();
}

// tag::createWindow[]
void Game::createWindow()
{
	//create window
	sdl_windowPtr = SDL_CreateWindow(windowName.c_str(), 100, 600, 600, 600, SDL_WINDOW_OPENGL); //same height and width makes the window square ...

	//error handling
	if(sdl_windowPtr == nullptr)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_CRITICAL, "SDL Window Creation failed!");
		SDL_Quit();
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_INFO, "SDL CreatedWindow OK!");
}
// end::createWindow[]

// tag::setGLAttributes[]
void Game::setGLAttributes()
{
	int major = 3;
	int minor = 3;
	SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_INFO, "Built for OpenGL Version %i.%i", major, minor); //https://en.wikipedia.org/wiki/OpenGL_Shading_Language#Versions
	// set the opengl context version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
	SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_INFO, "SDL Set OpenGL context to version %i.%i ", major, minor);
}
// tag::setGLAttributes[]

// tag::createContext[]
void Game::createContext()
{
	setGLAttributes();
	context = SDL_GL_CreateContext(sdl_windowPtr);

	//error handling

	if (context == NULL)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_CRITICAL, "SDL OpenGL Context Creation failed!");
		SDL_Quit();
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_INFO, "SDL Created OpenGL context OK!");
}
// end::createContext[]

// tag::initGlew[]
void Game::initGlew()
{
	glewExperimental = GL_TRUE; //GLEW isn't perfect - see https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
	const GLenum rev = glewInit();

	//error handling
	if (GLEW_OK != rev){
		SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_CRITICAL, "GLEW Error: %s", glewGetErrorString(rev));
		SDL_Quit();
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_INFO, "GLEW Init OK!");
}
// end::initGlew[]

// tag::handleInput[]
void Game::handleInput()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "handleInput");
	//Event-based input handling
	//The underlying OS is event-based, so **each** key-up or key-down (for example)
	//generates an event.
	//  - https://wiki.libsdl.org/SDL_PollEvent
	//In some scenarios we want to catch **ALL** the events, not just to present state
	//  - for instance, if taking keyboard input the user might key-down two keys during a frame
	//    - we want to catch based, and know the order
	//  - or the user might key-down and key-up the same within a frame, and we still want something to happen (e.g. jump)
	//  - the alternative is to Poll the current state with SDL_GetKeyboardState

	SDL_Event event; //somewhere to store an event

	//NOTE: there may be multiple events per frame
	while (SDL_PollEvent(&event)) //loop until SDL_PollEvent returns 0 (meaning no more events)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			done = true; //set donecreate remote branch flag if SDL wants to quit (i.e. if the OS has triggered a close event,
							//  - such as window close, or SIGINT
			break;

			//keydown handling - we should to the opposite on key-up for direction controls (generally)
		case SDL_KEYDOWN:
			//Keydown can fire repeatable if key-repeat is on.
			//  - the repeat flag is set on the keyboard event, if this is a repeat event
			//  - in our case, we're going to ignore repeat events
			//  - https://wiki.libsdl.org/SDL_KeyboardEvent
			if (!event.key.repeat)
				switch (event.key.keysym.sym)
				{
					//hit escape to exit
					case SDLK_ESCAPE:
					{
						SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "Escape key pressed");
						done = true;
					}
					break;
				}
			break;
		}
	}
}
// end::handleInput[]

// tag::updateSimulation[]
void Game::updateSimulation(double simLength) //update simulation with an amount of time to simulate for (in seconds)
{
	//WARNING - we should calculate an appropriate amount of time to simulate - not always use a constant amount of time
			// see, for example, http://headerphile.blogspot.co.uk/2014/07/part-9-no-more-delays.html
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "updateSimulation (for %f seconds)", simLength);

}
// end::updateSimulation[]

// tag::preRender[]
void Game::preRender()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "preRender");
	glViewport(0, 0, 600, 600); //set viewpoint
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f); //set clear colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the window (technical the scissor box bounds)
}
// end::preRender[]

// tag::render[]
void Game::render()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "render");
}
// end::render[]

// tag::postRender[]
void Game::postRender()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "postRender");
	SDL_GL_SwapWindow(sdl_windowPtr);; //present the frame buffer to the display (swapBuffers);
}
// end::postRender[]


void Game::run()
{
	while (!done) //loop until done flag is set)
	{
		run_once();
	}
}

void Game::run_once()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "=== Frame Start %i", frameCount++);

	handleInput(); // this should ONLY SET VARIABLES

	updateSimulation(); // this should ONLY SET VARIABLES according to simulation

	preRender();

	render(); // this should render the world state according to VARIABLES -

	postRender();

	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "=== Frame Done!\n\n");
}
