#include "Texture.hpp"
#include <SDL2/SDL_image.h>

Texture::Texture(std::string imagePath )
          :
		  imagePath(imagePath)
{
    SDL_Surface* image = IMG_Load(imagePath.c_str());
    if (image == nullptr)
    {
    	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_ERROR, "Load Image failed for %s", imagePath.c_str());
    }
    else
    {
      SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "Loaded Image from %s: (%d,%d)", imagePath.c_str(), image->w, image->h);
    	invert_image(image->pitch, image->h, image->pixels);
    }

    glGenTextures(1, &textureID); //generate a texture ID and store it
  	glBindTexture(GL_TEXTURE_2D, textureID);
  	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
  	glGenerateMipmap(GL_TEXTURE_2D);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  	glBindTexture(GL_TEXTURE_2D, 0);

  	SDL_FreeSurface(image);

	  SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG, "texture created. ID: %i", textureID);
}


// tag::invert_image[]
// from http://www.gribblegames.com/articles/game_programming/sdlgl/invert_sdl_surfaces.html - licensed as GPL (unchanged here)
void Texture::invert_image(int pitch, int height, void* image_pixels)
{
	int index;
	void* temp_row;
	int height_div_2;

	temp_row = (void *)malloc(pitch);
	if(NULL == temp_row)
	{
		SDL_SetError("Not enough memory for image inversion");
	}
	//if height is odd, don't need to swap middle row
	height_div_2 = (int) (height * .5);
	for(index = 0; index < height_div_2; index++) 	{
		//uses string.h
		memcpy((Uint8 *)temp_row,
			(Uint8 *)(image_pixels) +
			pitch * index,
			pitch);

		memcpy(
			(Uint8 *)(image_pixels) +
			pitch * index,
			(Uint8 *)(image_pixels) +
			pitch * (height - index-1),
			pitch);
		memcpy(
			(Uint8 *)(image_pixels) +
			pitch * (height - index-1),
			temp_row,
			pitch);
	}
	free(temp_row);
}
