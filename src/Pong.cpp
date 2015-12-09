#include "Pong.hpp"

Pong::Pong(std::string windowName)
			:
			Game::Game(windowName)
{
	glslProgram = new GLSLProgram();

	texture = new Texture("assets/textures/Opengl-logo.svg.png");

	quad = new Quad(glslProgram);

	triangle = new Triangle(glslProgram);


	batLeft = new Model("batLeft", quad, texture, glm::vec3(-80.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	batLeft->scale = (glm::vec3(2.0f, 10.0f, 1.0f));
	modelList.push_back(batLeft);

	batRight = new Model("batRight", quad, texture, glm::vec3(80.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	batRight->scale = (glm::vec3(2.0f, 10.0f, 1.0f));
	modelList.push_back(batRight);


	wallLeft = new Model("wallLeft", quad, texture, glm::vec3(-100.00f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), false);
	wallLeft->scale = (glm::vec3(5.0f, 100.0f, 1.0f));
	modelList.push_back(wallLeft);

	wallRight = new Model("wallRight", quad, texture, glm::vec3(100.00f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), false);
	wallRight->scale = (glm::vec3(5.0f, 100.0f, 1.0f));
	modelList.push_back(wallRight);

	Model* wallTop = new Model("wallTop", quad, texture, glm::vec3(0.00f, 100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), false);
	wallTop->scale = (glm::vec3(100.0f, 5.0f, 1.0f));
	wallTop->bounceAxis = 1;
	modelList.push_back(wallTop);

	Model* wallBottom = new Model("wallBottom", quad, texture, glm::vec3(0.00f, -100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), false);
	wallBottom->scale = (glm::vec3(100.0f, 5.0f, 1.0f));
	wallBottom->bounceAxis = 1;
	modelList.push_back(wallBottom);


	ball = new Model("ball", triangle, texture, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-50.0f, 20.0f, 0.0f));
	ball->scale = (glm::vec3(15.0f, 15.0f, 1.0f));
	ball->angleVelocityDegs = 55.0f;
	modelList.push_back(ball);

	glm::vec3* tmp = new glm::vec3(1.0f, 1.0f, 0.0f);
	trackingCamera = new TrackingOrthoCamera(glslProgram, ball, glm::vec2(200.0f));
	orthoCamera = new OrthoCamera(glslProgram, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(200.0f));
	camera = orthoCamera;
}

Pong::~Pong()
{

}

// tag::render[]
void Pong::render()
{
	Game::render();

	glslProgram->use();

	camera->activate();

	for(std::vector<Model*>::iterator it = modelList.begin(); it != modelList.end(); ++it)
	{
		Model* model = *it;
		model->draw();
	}

	orthoCamera->activate();
	for(std::vector<Model*>::iterator it = scoreMarkerList.begin(); it != scoreMarkerList.end(); ++it)
	{
		Model* model = *it;
		model->draw();
	}



}
// end::render[]

void Pong::incrementScore(int player)
{
	score[player]++;
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "  incrementScore for player %d", player);

	GLfloat direction = (player == 0) ? -1.0f : 1.0f;
	Model * scoreMarker = new Model("scoreMarker", triangle, texture, glm::vec3(direction * 8 * (9 - score[player]), -80.0f, 0.0f), glm::vec3(0.0f, 20.0f, 0.0f), false);
	scoreMarker->scale = (glm::vec3(5.0f, 5.0f, 1.0f));
	scoreMarker->angleVelocityDegs = 95.0f;
	scoreMarkerList.push_back(scoreMarker);

	if (score[0] >= scoreLimit || score[1] >= scoreLimit)
	{
			SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "  SCORE LIMIT HIT!");
			done = true;
	}
}

void Pong::handleBallCollision(Model * other)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "  Ball Collision with %s", other->name.c_str());

	glm::vec3 vel = ball->getVelocity();

	vel[other->bounceAxis] *= -1.0f;

	ball->setVelocity(vel);

	if (other == wallLeft)
	{
		ball->position = glm::vec3(0.0f);
		incrementScore(1);
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "  Hit wallLeft. Score now %d:%d", score[0], score[1]);
	}
	else if (other == wallRight)
	{
		ball->position = glm::vec3(0.0f);
		incrementScore(0);
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "  Hit wallRight. Score now %d:%d", score[0], score[1]);
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "  Ball Collision complete");
}


void Pong::handleBatCollision(Model * bat, Model * other)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "  Bat Collision with %s", other->name.c_str());
	if (other != ball)
	{
		glm::vec3 vel = bat->getVelocity();

		vel.y *= -1.0f;

		bat->setVelocity(vel);
	}
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "  Bat Collision complete");
}
// tag::updateSimulation[]
void Pong::updateSimulation(double simLength) //update simulation with an amount of time to simulate for (in seconds)
{
	Game::updateSimulation(simLength);

	// collision detection
	for(std::vector<Model*>::iterator it2 = modelList.begin(); it2 != modelList.end(); ++it2)
	{
		Model* model2 = *it2;
		for(std::vector<Model*>::iterator it1 = modelList.begin(); it1 != modelList.end(); ++it1)
		{
			Model* model1 = *it1;
			if (it1==it2) break;
			if (model1->name == model2->name) break;

			SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "Testing for collision between %s and %s", model1->name.c_str(), model2->name.c_str());
			bool intersection = Model::intersects(model1, model2);
			if (intersection)
			{
				if (model1 == ball) handleBallCollision(model2);
				else if (model2 == ball) handleBallCollision(model1);

				if (model1 == batLeft) handleBatCollision(batLeft, model2);
				else if (model2 == batLeft) handleBatCollision(batLeft, model1);

				if (model1 == batRight) handleBatCollision(batRight, model2);
				else if (model2 == batRight) handleBatCollision(batRight, model1);

			}
		}
	}
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "Collision Detection Complete");


	// update Positions and angle from Velocities and angular velocities
	for(std::vector<Model*>::iterator it = modelList.begin(); it != modelList.end(); ++it)
	{
		Model* model = *it;
		model->position += float(simLength) * model->getVelocity();
		model->angleDegs += float(simLength) * model->angleVelocityDegs;

		glm::vec3 vel = model->getVelocity();
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "  %s->velocity: %f, %f, %f", model->name.c_str(), vel.x, vel.y, vel.z);
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "  %s->position: %f, %f, %f", model->name.c_str(), model->position.x, model->position.y, model->position.z);
	}

	// update Positions and angle from Velocities and angular velocities
	for(std::vector<Model*>::iterator it = scoreMarkerList.begin(); it != scoreMarkerList.end(); ++it)
	{
		Model* model = *it;
		model->position += float(simLength) * model->getVelocity();
		model->angleDegs += float(simLength) * model->angleVelocityDegs;

		glm::vec3 vel = model->getVelocity();
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "  %s->velocity: %f, %f, %f", model->name.c_str(), vel.x, vel.y, vel.z);
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "  %s->position: %f, %f, %f", model->name.c_str(), model->position.x, model->position.y, model->position.z);
	}
}
// end::updateSimulation[]



void Pong::handleInput()
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

		glm::vec3 batLeftVel = batLeft->getVelocity();
		glm::vec3 batRightVel = batRight->getVelocity();

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

					// some very bad key handling
					case SDLK_w:
					{
						SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "W key pressed");
						batLeftVel.y = batSpeed;

					}
					break;

					case SDLK_s:
					{
						SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "S key pressed");
						batLeftVel.y = -batSpeed;
					}
					break;


					// some very bad key handling
					case SDLK_UP:
					{
						SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "W key pressed");
						batRightVel.y = batSpeed;

					}
					break;

					case SDLK_DOWN:
					{
						SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "S key pressed");
						batRightVel.y = -batSpeed;
					}
					break;
				}

			break;
		}
		batLeft->setVelocity(batLeftVel);
		batRight->setVelocity(batRightVel);
	}
}
