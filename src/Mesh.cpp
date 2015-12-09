#include "Mesh.hpp"

Mesh::Mesh(GLSLProgram * glslProgram, std::vector<Vertex> vertexData )
          :
		  glslProgram(glslProgram),
		  vertexData(vertexData)
{


	glGenBuffers(1, &vertexDataBufferObject);
	glGenVertexArrays(1, &vertexArrayObject);
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "Mesh created. VBO: %i, VAO: %i", vertexDataBufferObject, vertexArrayObject);

	bufferData();
	setupVertexArrayObject();
}

void Mesh::draw(GLenum mode)
{
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(mode, 0, vertexData.size());
}

void Mesh::bufferData(std::vector<Vertex> newVertexData)
{
	SDL_assert_release(vertexDataBufferObject != 0);

	if (newVertexData.size() > 0)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "Mesh updated. VBO: %i, VAO: %i", vertexDataBufferObject, vertexArrayObject);
		vertexData = newVertexData;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* vertexData.size(), vertexData.data(), GL_STATIC_DRAW);
}

void Mesh::setupVertexArrayObject()
{
	//setup how to access data and from where
	glBindVertexArray(vertexArrayObject); // settings are stored in a VAO

		glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject); // data source

			glEnableVertexAttribArray(glslProgram->vertexPositionLocation);
			glEnableVertexAttribArray(glslProgram->vertexColorLocation);
      glEnableVertexAttribArray(glslProgram->vertexUVLocation);


			// tag::glVertexAttribPointer[]
			SDL_assert((sizeof(Vertex().position) %  alignof(Vertex)) == 0); //check will align without padding
			SDL_assert((sizeof(Vertex().color) %  alignof(Vertex)) == 0); //check will align without padding
      SDL_assert((sizeof(Vertex().uv) %  alignof(Vertex)) == 0); //check will align without padding


			glVertexAttribPointer(glslProgram->vertexPositionLocation,    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);
			glVertexAttribPointer(glslProgram->vertexColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) sizeof(Vertex().position));
      glVertexAttribPointer(glslProgram->vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (sizeof(Vertex().position) + sizeof(Vertex().color)));
			// end::glVertexAttribPointer[]

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it
}
