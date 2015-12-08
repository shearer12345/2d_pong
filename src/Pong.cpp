#include "Pong.hpp"

Pong::Pong(std::string windowName)
			:
			Game::Game(windowName)
{
	glslProgram = new GLSLProgram();


	Mesh* quad = new Quad(glslProgram);

	Mesh* triangle = new Triangle(glslProgram);


	Model* batLeft = new Model("batLeft", quad, glm::vec3(-80.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), false);
	batLeft->scale = (glm::vec3(2.0f, 10.0f, 1.0f));
	modelList.push_back(batLeft);

	Model* batRight = new Model("batRight", quad, glm::vec3(80.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), false);
	batRight->scale = (glm::vec3(2.0f, 10.0f, 1.0f));
	modelList.push_back(batRight);

	Model* model2 = new Model("quad2", quad, glm::vec3(0.00f, -5.0f, 0.0f), glm::vec3(0.0f, 5.0f, 0.0f));
	modelList.push_back(model2);

	Model* model3 = new Model("triangle1", triangle, glm::vec3(25.50f, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f));
	modelList.push_back(model3);

	// Camera* camara = new PerspectiveCamera(glslProgram,
	// 											glm::vec3(0.0f, 0.0f, 1.0f),
	// 											glm::vec3(0.5f, 0.0f, -1.0f),
	// 											glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3* tmp = new glm::vec3(1.0f, 1.0f, 0.0f);
	TrackingOrthoCamera* trackingCamera = new TrackingOrthoCamera(glslProgram, model2, glm::vec2(200.0f));
	OrthoCamera* orthoCamera = new OrthoCamera(glslProgram, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(200.0f));


	cameraList.push_back(orthoCamera);
}

Pong::~Pong()
{

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

			bool intersection = Model::intersects(model1, model2);
			SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_VERBOSE, "Testing for collision between %s and %s", model1->name.c_str(), model2->name.c_str());
			if (intersection)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "Collision between %s and %s", model1->name.c_str(), model2->name.c_str());
				model1->setVelocity(model1->getVelocity() * -1.0f);
				model2->setVelocity(model2->getVelocity() * -1.0f);
			}


		}
	}


	// update Positions from Velocities
	for(std::vector<Model*>::iterator it = modelList.begin(); it != modelList.end(); ++it)
	{
		Model* model = *it;
		model->position += float(simLength) * model->getVelocity();
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "  %s->position: %f, %f, %f", model->name.c_str(), model->position.x, model->position.y, model->position.z);
	}

}
// end::updateSimulation[]

// tag::render[]
void Pong::render()
{
	Game::render();

	glslProgram->use();

	for(std::vector<Camera*>::iterator it = cameraList.begin(); it != cameraList.end(); ++it)
	{
		Camera* camera = *it;
		camera->activate();

		for(std::vector<Model*>::iterator it = modelList.begin(); it != modelList.end(); ++it)
		{
			Model* model = *it;
			model->draw();
		}
	}


}
// end::render[]
