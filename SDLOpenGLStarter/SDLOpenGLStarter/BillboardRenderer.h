#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "Camera.h"
#include "Utils.h"
#include "Textures.h"
#include "ShaderUtils.h"
#include "Vertex.h"


namespace GE {
	class BillboardRenderer
	{
	public:
		BillboardRenderer(Camera* c);

		~BillboardRenderer() {
			glDeleteBuffers(1, &vboTriangle);
		}

		void init();
		void draw();

		float getPosX()
		{
			return pos_x;
		}

		float getPosY()
		{
			return pos_y;
		}

		float getPosZ()
		{
			return pos_z;
		}

		float getRotX()
		{
			return rot_x;
		}

		float getRotY()
		{
			return rot_y;
		}

		float getRotZ()
		{
			return rot_z;
		}

		float getScaleX()
		{
			return scale_x;
		}

		float getScaleY()
		{
			return scale_y;
		}

		float getScaleZ()
		{
			return scale_z;
		}

		void setPos(float x, float y, float z)
		{
			pos_x = x;
			pos_y = y;
			pos_z = z;
		}

		void setRotation(float rx, float ry, float rz)
		{
			rot_x = rx;
			rot_y = ry;
			rot_z = rz;
		}

		void setScale(float sx, float sy, float sz)
		{
			scale_x = sx;
			scale_y = sy;
			scale_z = sz;
		}

		void setScaleX(float newScaleX)
		{
			scale_x = newScaleX;
		}

		void setScaleY(float newScaleY)
		{
			scale_y = newScaleY;
		}

		void setMaterial(Textures* mat)
		{
			material = mat;
		}

	private:
		// Member fields
		// This member stores the program object that contains the shaders
		GLuint programId;

		// This member stores the attribute to select into the pipeline
		// to link the triangle vertices to the pipeline
		//GLint vertexLocation;

		// Link to vColour attribute which receives a colour
		// and passes to fColour for fragment shader
		//GLint vertexUVLocation;

		// This member stores the triangle vertex buffer object containing the vertices
		// transferred from this code to the graphics memory
		GLuint vboTriangle;

		//rotation,position and scale
		float pos_x, pos_y, pos_z;
		float rot_x, rot_y, rot_z;
		float scale_x, scale_y, scale_z;


		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId, viewUniformId, projectionUniformId, samplerId, vertexPos3DLocation, vertexUVLocation;

		Camera* cam;
		Textures* material;
	};
}
