#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Camera.h"
#include "Utils.h"
#include "Model.h"
#include "Terrain.h"
#include "Textures.h"
#include "SkyDome.h"
#include "ShaderUtils.h"
#include <glm/gtc/type_ptr.hpp>

namespace GE {

	class ModelRenderer
	{
	public:
		ModelRenderer(Camera* cam);
		virtual ~ModelRenderer();

		//init method to create the shaders and vertex buffer object
		void init();

		//update method to update the state of the object
		//e.g., in other types of objects they may have animations
		//which need updating every frame
		void update();

		//draw method to render
		void draw(Camera* cam, Model* model);

		void drawTerrain(Terrain* terrain);

		void drawSkyDome(SkyDome* sky);

		//release method to release objects such as vertex buffer object
		void destroy();

		//Accessors
		//Position
		float getPosX() {
			return pos_x;
		}

		float getPosY() {
			return pos_y;
		}

		float getPosZ() {
			return pos_z;
		}

		//rotation
		float getRotX() {
			return rot_x;
		}

		float getRotY() {
			return rot_y;
		}

		float getRotZ() {
			return rot_z;
		}

		//scale
		float getScaleX() {
			return scale_x;
		}

		float getScaleY() {
			return scale_y;
		}

		float getScaleZ() {
			return scale_z;
		}

		//mutator methods
		void setPos(float x, float y, float z) {
			pos_x = x;
			pos_y = y;
			pos_z = z;
		}

		void setRotation(float rx, float ry, float rz) {
			rot_x = rx;
			rot_y = ry;
			rot_z = rz;
		}

		void setScale(float sx, float sy, float sz) {
			scale_x = sx;
			scale_y = sy;
			scale_z = sz;
		}

		//texture methods
		void setTexture(Textures* _tex) {
			tex = _tex;
		}
	private:
		//model object to renderrer
		Model* model;

		//texture to apply to model
		Textures* tex;

		//camera
		Camera* cam;

		//Member fields
		//this member stores program object that contains the shaders
		GLuint programId;

		//This member stores the attribute to select into the pipeline
		//to link to the triangle verticles to the pipeline
		//link tovColour attribute which recieives a colour and passes to fColour for fragment shader
		GLint vertexPos3DLocation, vertexUVLocation;

		//link to normal attribute
		GLint vertexNormal;

		//this member stores the triangle vertex buffer object containing the vertices
		//transferred from this code to the graphics memory
		GLuint vboTriangle;


		//Location, rotation and scale variables
		float pos_x, pos_y, pos_z;
		float rot_x, rot_y, rot_z;
		float scale_x, scale_y, scale_z;

		//GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;
		//for camera position
		GLuint viewPosId;

		//light
		GLuint lightColourId, ambientId, lightPosId, shininessId, specularStrengthId;
	
		GLuint enableLightingUniformId; 
	};
}
