#include "Mesh.hpp"

Mesh::Mesh(GLSLProgram * glslProgram)
          :
		  glslProgram(glslProgram)
{
	//setup a GL VertexArrayObject that stores how to access data and from where
	glGenVertexArrays(1, &vertexArrayObject); //create a Vertex Array Object
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "Vertex Array Object created OK! ID is: %i", vertexArrayObject);

	glBindVertexArray(vertexArrayObject); //make the just created vertexArrayObject the active one

		glGenBuffers(1, &vertexDataBufferObject);
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "vertexDataBufferObject created OK! ID is: %i", vertexDataBufferObject);

		glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject);

			glEnableVertexAttribArray(glslProgram->positionLocation); //enable attribute at index positionLocation
			glEnableVertexAttribArray(glslProgram->vertexColorLocation); //enable attribute at index vertexColorLocation

			bufferData();
			// tag::glVertexAttribPointer[]
			SDL_assert((sizeof(Vertex().position) %  alignof(Vertex)) == 0); //check will align without padding
			SDL_assert((sizeof(Vertex().color) %  alignof(Vertex)) == 0); //check will align without padding

			glVertexAttribPointer(glslProgram->positionLocation,    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);
			glVertexAttribPointer(glslProgram->vertexColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) sizeof(Vertex().position));
			// end::glVertexAttribPointer[]

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it
}

void Mesh::draw(GLenum mode)
{
	glDrawArrays(mode, 0, vertexData.size());
}

void Mesh::bufferData()
{
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* vertexData.size(), vertexData.data(), GL_STATIC_DRAW);
}
