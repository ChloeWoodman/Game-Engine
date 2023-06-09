#pragma once

#include <SDL_image.h>
#include <vector>
#include "Model.h"

// Add comments to the code
namespace GE {
	class Terrain
	{
	public:
		Terrain();
		~Terrain();
		// Add const to the get functions and remove unnecessary return values
		GLuint getVbo() const { return vbo; }
		GLuint getIbo() const { return ibo; }
		GLuint getIndexCount() const { return indexCount; }

	private:
		// Add unsigned int to indexCount
		GLuint vbo, ibo;
		unsigned int indexCount;
	};
}