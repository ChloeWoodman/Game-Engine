#pragma once
#include <GL/glew.h>
#include <vector>
#include "Camera.h"
#include "Textures.h"
#include "Model.h"

namespace GE {
	// Define a structure for the instance data
	struct InstancePosRotScaleLife {
		float posX, posY, posZ;
		float rotX, rotY, rotZ;
		float scaleX, scaleY, scaleZ;
	};

	class InstancedRenderer
	{
	public:
		InstancedRenderer();
		~InstancedRenderer();

		void init();

		void destroy();

		void drawInstanced(Camera* cam, Model* m, Textures* tex);

		void updateInstanced(const std::vector<InstancePosRotScaleLife>& instances);

		void setInstanceData(const std::vector<InstancePosRotScaleLife>& instances);

	private:
		// Member fields
		// This member stores the program object that contains the shaders
		GLuint programId;

		// This member stores the attribute to select into the pipeline
		// to link the triangle vertices to the pipeline
		GLint vertexLocation;

		// Link to vColour attribute which receives a colour
		// and passes to fColour for fragment shader
		GLint vertexUVLocation;

		// Link instance matrix attribute
		GLint instanceMatLocation;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;

		int numInstances;

		GLuint instanceMatrixBuffer;

	};
}

