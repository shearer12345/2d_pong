// tag::C++11check[]
#define STRING2(x) #x
#define STRING(x) STRING2(x)

#if __cplusplus < 201103L
	#pragma message("WARNING: the compiler may not be C++11 compliant. __cplusplus version is : " STRING(__cplusplus))
#endif
// end::C++11check[]

// tag::includes[]
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_log.h>

#define GLM_FORCE_RADIANS // suppress a warning in GLM 0.9.5
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
// end::includes[]

// tag::using[]
// see https://isocpp.org/wiki/faq/Coding-standards#using-namespace-std
// don't use the whole namespace, either use the specific ones you want, or just type std::
using std::max;
using std::string;
// end::using[]


// tag::globalVariables[]
std::string exeName;
SDL_Window *win = nullptr; //pointer to the SDL_Window
SDL_GLContext context = NULL; //the SDL_GLContext
int frameCount = 0;
std::string frameLine = "";
std::string vertexShaderPath = "assets/shaders/vertexShader.glsl";
std::string fragmentShaderPath = "assets/shaders/fragmentShader.glsl";
// end::globalVariables[]


//our variables
bool done = false;



// tag::gameState[]
//the translation vector we'll pass to our GLSL program
glm::vec3 position1 = { -0.5f, -0.5f, 0.0f};
glm::vec3 velocity1 = { 0.1f, 0.1f, 0.0f};

glm::vec3 position2 = { 0.8f, -0.5f , 0.0f};
glm::vec3 velocity2 = { -0.2f, 0.15f, 0.0f};
// end::gameState[]

// tag::GLVariables[]
//our GL and GLSL variables
//programIDs
GLuint theProgram; //GLuint that we'll fill in to refer to the GLSL program (only have 1 at this point)

//attribute locations
GLint positionLocation; //GLuint that we'll fill in with the location of the `position` attribute in the GLSL
GLint vertexColorLocation; //GLuint that we'll fill in with the location of the `vertexColor` attribute in the GLSL

//uniform location
GLint modelMatrixLocation;
GLint viewMatrixLocation;
GLint projectionMatrixLocation;

GLuint vertexDataBufferObject;
GLuint vertexArrayObject;
// end::GLVariables[]



// end Global Variables
/////////////////////////

// tag::initialise[]
void initialise()
{
	int init = SDL_Init(SDL_INIT_EVERYTHING);

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

	// error handling
	if (init != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "SDL initialisation failed!");
		SDL_Quit();
	}


	SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_INFO, "SDL initialised OK!");
}
// end::initialise[]

// tag::createWindow[]
void createWindow()
{
	//get executable name, and use as window title
	int beginIdxWindows = exeName.rfind("\\"); //find last occurrence of a backslash
	int beginIdxLinux = exeName.rfind("/"); //find last occurrence of a forward slash
	int beginIdx = max(beginIdxWindows, beginIdxLinux);
	std::string exeNameEnd = exeName.substr(beginIdx + 1);
	const char *exeNameCStr = exeNameEnd.c_str();

	//create window
	win = SDL_CreateWindow(exeNameCStr, 100, 100, 600, 600, SDL_WINDOW_OPENGL); //same height and width makes the window square ...

	//error handling
	if(win == nullptr)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_CRITICAL, "SDL Window Creation failed!");
		SDL_Quit();
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_INFO, "SDL CreatedWindow OK!");
}
// end::createWindow[]

// tag::setGLAttributes[]
void setGLAttributes()
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
void createContext()
{
	setGLAttributes();
	context = SDL_GL_CreateContext(win);

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
void initGlew()
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

// tag::loadShader[]
std::string loadShader(const string filePath) {
    std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);

	//error handling
	if (!fileStream)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "GLSL Shader could not be loaded - cannot read file - %s", filePath);
        SDL_Quit();
	}

	string fileData( (std::istreambuf_iterator<char>(fileStream)),
	                 (std::istreambuf_iterator<char>()          ));

	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "GLSL Shader Loaded from '%s'", filePath.c_str());
	return fileData;
}
// end::loadShader[]

// tag::createShader[]
GLuint createShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	//error check
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	//error handling
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	SDL_assert_release(status != GL_FALSE);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
			case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
			case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_CRITICAL, "GLSL Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
		SDL_Quit();
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "GLSL Shader created with ID %i", shader);
	return shader;
}
// end::createShader[]

// tag::createProgram[]
GLuint createProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	//error handling
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	SDL_assert_release(status != GL_FALSE);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_CRITICAL, "GLSL Linker failure: %s", strInfoLog);
		delete[] strInfoLog;
		SDL_Quit();
	}

	SDL_assert_release(program != 0);

	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "GLSL program created with ID %i", program);
	return program;
}
// end::createProgram[]

// tag::initializeProgram[]
void initializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, loadShader(vertexShaderPath)));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, loadShader(fragmentShaderPath)));

	theProgram = createProgram(shaderList);

	//clean up shaders (we don't need them anymore as they are not in theProgram
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

	// tag::glGetAttribLocation[]
	positionLocation = glGetAttribLocation(theProgram, "position");
	vertexColorLocation = glGetAttribLocation(theProgram, "vertexColor");

	//error handling
	SDL_assert_release( positionLocation != -1);
	SDL_assert_release( vertexColorLocation != -1);
	// end::glGetAttribLocation[]

	// tag::glGetUniformLocation[]
	modelMatrixLocation = glGetUniformLocation(theProgram, "modelMatrix");
	viewMatrixLocation = glGetUniformLocation(theProgram, "viewMatrix");
	projectionMatrixLocation = glGetUniformLocation(theProgram, "projectionMatrix");

	//error handling
	SDL_assert_release( modelMatrixLocation != -1);
	SDL_assert_release( viewMatrixLocation != -1);
	SDL_assert_release( projectionMatrixLocation != -1);
	// end::glGetUniformLocation[]

	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "GLSL program creation and attribute/uniform location population OK!");
}
// end::initializeProgram[]


// tag::initializeVertexBufferAndVertexArrayObject[]
void initializeVertexBufferAndVertexArrayObject()
{

	class myVertex
	{
	public:
		glm::vec3 position;
		glm::vec4 color;

		myVertex();

		myVertex(glm::vec3 pos, glm::vec4 col)
		{
			position = pos;
			color = col;
		};
	};
	// tag::vertexData[]
	//the data about our geometry
	const myVertex vertexData[] = {
	//	 X        Y            Z          R     G     B      A
		 myVertex(   glm::vec3( 0.000f,  0.500f,  0.000f),  glm::vec4(1.0f, 0.0f, 0.0f,  1.0f)),
		 myVertex(   glm::vec3(-0.433f, -0.250f,  0.000f),  glm::vec4(0.0f, 1.0f, 0.0f,  1.0f)),
		 myVertex(   glm::vec3( 0.433f, -0.250f,  0.000f),  glm::vec4(0.0f, 0.0f, 1.0f,  1.0f))
	};
	// end::vertexData[]

	//setup a GL VertexArrayObject that stores how to access data and from where
	glGenVertexArrays(1, &vertexArrayObject); //create a Vertex Array Object
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "Vertex Array Object created OK! ID is: %i", vertexArrayObject);

	glBindVertexArray(vertexArrayObject); //make the just created vertexArrayObject the active one

		glGenBuffers(1, &vertexDataBufferObject);
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "vertexDataBufferObject created OK! ID is: %i", vertexDataBufferObject);

		glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

			glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
			glEnableVertexAttribArray(vertexColorLocation); //enable attribute at index vertexColorLocation

			// tag::glVertexAttribPointer[]
			SDL_assert((sizeof(myVertex().position) %  alignof(myVertex)) == 0); //check will align without padding
			SDL_assert((sizeof(myVertex().color) %  alignof(myVertex)) == 0); //check will align without padding

			glVertexAttribPointer(positionLocation,    3, GL_FLOAT, GL_FALSE, sizeof(myVertex), (GLvoid *) 0);
			glVertexAttribPointer(vertexColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(myVertex), (GLvoid *) sizeof(myVertex().position));
			// end::glVertexAttribPointer[]

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it
}
// end::initializeVertexBufferAndVertexArrayObjectAndVertexArrayObject[]

// tag::loadAssets[]
void loadAssets()
{
	initializeProgram(); //create GLSL Shaders, link into a GLSL program, and get IDs of attributes and variables

	initializeVertexBufferAndVertexArrayObject(); //load data into a vertex buffer

	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "Loaded Assets OK!");
}
// end::loadAssets[]

// tag::handleInput[]
void handleInput()
{
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
void updateSimulation(double simLength = 0.02) //update simulation with an amount of time to simulate for (in seconds)
{
	//WARNING - we should calculate an appropriate amount of time to simulate - not always use a constant amount of time
			// see, for example, http://headerphile.blogspot.co.uk/2014/07/part-9-no-more-delays.html

	position1 += float(simLength) * velocity1;
	position2 += float(simLength) * velocity2;

}
// end::updateSimulation[]

// tag::preRender[]
void preRender()
{
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "Frame %i", frameCount++);
	glViewport(0, 0, 600, 600); //set viewpoint
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f); //set clear colour
	glClear(GL_COLOR_BUFFER_BIT); //clear the window (technical the scissor box bounds)
}
// end::preRender[]

// tag::render[]
void render()
{
	glUseProgram(theProgram); //installs the program object specified by program as part of current rendering state

	glBindVertexArray(vertexArrayObject);

	//set projectionMatrix - how we go from 3D to 2D
	glUniformMatrix4fv(projectionMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0)));

	//set viewMatrix - how we control the view (viewpoint, view direction, etc)
	glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));


	//set modelMatrix and draw for triangle 1
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position1);
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//set modelMatrix and draw for triangle 2
	modelMatrix = glm::translate(glm::mat4(1.0f), position2);
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);

	glUseProgram(0); //clean up
}
// end::render[]

// tag::postRender[]
void postRender()
{
	SDL_GL_SwapWindow(win);; //present the frame buffer to the display (swapBuffers)
}
// end::postRender[]

// tag::cleanUp[]
void cleanUp()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "SDL Clean up OK!");
}
// end::cleanUp[]

// tag::main[]
int main( int argc, char* args[] )
{
	exeName = args[0];
	//setup
	//- do just once
	initialise();
	createWindow();

	createContext();

	initGlew();

	glViewport(0,0,600,600); //should check what the actual window res is?

	//do stuff that only needs to happen once
	//- create shaders
	//- load vertex data
	loadAssets();

	while (!done) //loop until done flag is set)
	{
		handleInput(); // this should ONLY SET VARIABLES

		updateSimulation(); // this should ONLY SET VARIABLES according to simulation

		preRender();

		render(); // this should render the world state according to VARIABLES -

		postRender();

	}

	//cleanup and exit
	cleanUp();
	SDL_Quit();

	return 0;
}
// end::main[]
