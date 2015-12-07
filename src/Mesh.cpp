#include "Mesh.hpp"

Mesh::Mesh(GLSLProgram * glslProgram)
          :
		  glslProgram(glslProgram)
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

			glEnableVertexAttribArray(glslProgram->positionLocation); //enable attribute at index positionLocation
			glEnableVertexAttribArray(glslProgram->vertexColorLocation); //enable attribute at index vertexColorLocation

			// tag::glVertexAttribPointer[]
			SDL_assert((sizeof(myVertex().position) %  alignof(myVertex)) == 0); //check will align without padding
			SDL_assert((sizeof(myVertex().color) %  alignof(myVertex)) == 0); //check will align without padding

			glVertexAttribPointer(glslProgram->positionLocation,    3, GL_FLOAT, GL_FALSE, sizeof(myVertex), (GLvoid *) 0);
			glVertexAttribPointer(glslProgram->vertexColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(myVertex), (GLvoid *) sizeof(myVertex().position));
			// end::glVertexAttribPointer[]

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it
}
