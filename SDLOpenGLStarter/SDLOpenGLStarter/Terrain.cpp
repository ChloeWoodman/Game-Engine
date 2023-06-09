#include "Terrain.h"

namespace GE {
	Terrain::Terrain() {
		// Load heightmap from file
		SDL_Surface* heightMap = IMG_Load("./textures/terrain-heightmap.png");
		const int height = heightMap->h;
		const int width = heightMap->w;
		unsigned char* imageData = (unsigned char*)heightMap->pixels;
		const int channels = heightMap->format->BytesPerPixel;
		const float heightScaling = 100.0f;
		const float scaling = 100.0f;

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		float x, y, z, u, v, nx, ny, nz;

		// Calculate vertices
		for (int row = 0; row < height; row++) {
			for (int column = 0; column < width; column++) {
				unsigned char redPixel = imageData[((row * width) + column) * channels];

				x = (float)column / (float)(width - 1) * scaling;
				z = (float)row / (float)(height - 1) * scaling;
				y = redPixel / 255.0f * heightScaling;

				u = (float)column / (float)(width - 1);
				v = (float)(height - row) / (float)(height - 1);
				nx = 0.0f;
				ny = 0.0f;
				nz = 0.0f;
				vertices.push_back(Vertex(x, y, z, u, v, nx, ny, nz));
			}
		}

		// Calculate indices
		unsigned int c, n;
		for (int row = 0; row < height - 1; row++) {
			for (int column = 0; column < width - 1; column++) {
				c = row * width + column;
				n = (row + 1) * width + column;

				indices.push_back(c);
				indices.push_back(n);
				indices.push_back(c + 1);
				indices.push_back(c + 1);
				indices.push_back(n);
				indices.push_back(n + 1);
			}
		}

		// Generate vertex and index buffers
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		indexCount = indices.size();

		// Free surface memory
		SDL_FreeSurface(heightMap);
	}
}