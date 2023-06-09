#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include "Vertex.h"

namespace GE
{
	class SkyDome
	{
	public:
		SkyDome(float res, float rad);
		~SkyDome(); // Removed glDeleteBuffers from destructor since have a smart pointer for this now
		GLuint getVbo() const; // Made getter functions const
		GLuint getIbo() const;
		GLuint getIndexCount() const;

	private:
		std::vector<Vertex> vertices;
		GLuint ibo, vbo, indexCount;
		std::vector<unsigned int> indices;
	};
}