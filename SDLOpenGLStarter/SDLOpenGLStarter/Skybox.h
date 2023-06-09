#pragma once
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <vector>
#include <string>
#include "Camera.h"


namespace GE {

	//Note that cube for skybox has its own vertex struct
	//Major difference is there is no colour or uv
	//This is because cube is sampled from cube vertices
	struct CubeVertex {
		float x, y, z;

		CubeVertex() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		CubeVertex(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}
	};

	class Skybox
	{
	public:
		Skybox(std::string front_fname, std::string back_fname,
			std::string right_fname, std::string left_fname, std::string top_fname, std::string bottom_fname) {
			std::vector<std::string> filenames;

			filenames.push_back(right_fname);
			filenames.push_back(left_fname);
			filenames.push_back(top_fname);
			filenames.push_back(bottom_fname);
			filenames.push_back(front_fname);
			filenames.push_back(back_fname);

			createCubemap(filenames);
			createCubeVBO();
			createSkyboxProgram();
		}
		~Skybox() {}

		void draw(Camera* cam);

		void destroy();

	private:
		void createCubemap(std::vector<std::string> filename);
		void createCubeVBO();
		void createSkyboxProgram();

		GLuint skyboxCubeMapName;

		//member fields
		//this member stores the program object that contains the shaders
		GLuint skyboxProgramId;

		//this member stores the attribute to select into the pipeline
		//to link tyhe triangle vertices to the pipeline
		GLuint vertexLocation;

		//ths member stores the triangle vertex buffer object containing the vertices
		//transferred from this code to the pipeline
		GLuint vboSkybox;

		//GLSL uniform variables for the transformation, view and projection matrices
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId, vao;
	};
}