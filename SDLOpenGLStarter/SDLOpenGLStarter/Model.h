#pragma once
#include <GL/glew.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
//#include <SDL.h>
#include <iostream>
#include <vector>
#include "Utils.h"
#include "Vertex.h"


namespace GE {

	// create a model class to load 3D models
	class Model {
	public:
		// constructor
		Model() {
			vbo = 0;
			numVertices = 0;
		}

		// destructor to free up the memory
		~Model() {
			glDeleteBuffers(1, &vbo);
		}

		// load vertices from a file
		bool loadFromFile(const char* filename);

		// returns the VBO to make a vertex buffer based on model vertices
		GLuint getVertices() {
			return vbo;
		}

		// returns the number of vertices in model for rendering purposes
		int getNumVertices() {
			return numVertices;
		}

	private:
		// private member variables
		GLuint vbo;
		int numVertices;
	};
}

