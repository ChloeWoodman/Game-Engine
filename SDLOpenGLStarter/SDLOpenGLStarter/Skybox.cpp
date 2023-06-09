#pragma once
#include <SDL_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Skybox.h"
#include "ShaderUtils.h"

namespace GE {
	

	const float SIDE = 1.0f;

	//CubeVertex cube[] = {
	//	//front face
	//	CubeVertex(-SIDE, SIDE, -SIDE),
	//	CubeVertex(-SIDE, -SIDE, -SIDE),
	//	CubeVertex(SIDE, -SIDE, -SIDE),
	//};

	CubeVertex cube[] = {
		// Front face
		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),

		// Back face
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(SIDE, -SIDE, SIDE),

		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, SIDE, SIDE),

		// Left face
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE,  SIDE, -SIDE),

		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE,  SIDE),

		// Right face
		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE,  SIDE),

		// Top face
		CubeVertex(-SIDE, SIDE,  SIDE),
		CubeVertex(SIDE, SIDE,  SIDE),
		CubeVertex(SIDE, SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE,  SIDE),

		// Bottom face
		CubeVertex(-SIDE, -SIDE,  SIDE),
		CubeVertex(SIDE, -SIDE,  SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE,  SIDE),
	};

	void Skybox::createCubemap(std::vector<std::string> filenames) {
		glGenTextures(1, &skyboxCubeMapName);

		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMapName);

		//load texture data from files into cubemap
		for (int faceNum = 0; faceNum < 6; faceNum++) {
			SDL_Surface* surfaceImage = IMG_Load(filenames[faceNum].c_str());

			//check it was loaded ok
			if (surfaceImage == nullptr) {
				return;
			}

			//get the format from the SDL object, will be in SDL Image's format
			GLenum format = surfaceImage->format->format;

			//determine OpenGL format from SDL format
			switch (format) {
				//RGBA where there 8 bits per pixel, 32 bits in total 
			case SDL_PIXELFORMAT_RGBA32: format = GL_RGBA;
				break;

				//RGB where there are 8 bits per pixel, 24 bits in total
			case SDL_PIXELFORMAT_RGB24: format = GL_RGB;
				break;

			//would have moreformats here. There are alternative
			//ways to archieve the same mapping.. Left as an excercise to find out

			//default is RGB but should really use as many cases as expected texture data formats
			default:
				format = GL_RGB;
				break;
			}
			// copy the pixel data from SDL surface object to the OpenGL texture
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceNum, 0, format, std::min(surfaceImage->w, surfaceImage->h), std::min(surfaceImage->w, surfaceImage->h)
				, 0, format, GL_UNSIGNED_BYTE, surfaceImage->pixels);

			//release the surface object and memory associated with it as no longer needed
			SDL_FreeSurface(surfaceImage);
		}
		//Configure how the texture will be manipulated when it needs to be reduced or increased (magnified) when rendering onto
		//a object. GL_LINEAR is weighted average of colours around texture co-ords
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Clamp the text edge to deal with inaccuracies. If the xyz co-ords go over
		//under 0/1, then sampler will wrap a texture resulting in artefacting at the edges
		//this sets the cubmap to use clamping to repeat the last line of the texture
		//to deal with inaccuracies
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void Skybox::createCubeVBO() {
		//create the vertex buffer object
		glGenBuffers(1, &vboSkybox);
		glBindBuffer(GL_ARRAY_BUFFER, vboSkybox);

		//transfer vertices to graphics memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

		//release from pipeline
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Skybox::createSkyboxProgram() {
		//shader code
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"out vec3 texCoord;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main() {\n"
			"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"v = projection * view * v;\n"
			"gl_Position = v;\n"
			"texCoord = vertexPos3D;\n"
			"}\n"
		};

		//Fragment shader source code
		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"in vec3 texCoord;\n"
			"uniform samplerCube sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main()\n"
			"{\n"
			"fragmentColour = vec4(texture(sampler, texCoord).rgba);\n"
			"}\n"
		};

		bool result = compileProgram(V_ShaderCode, F_ShaderCode, &skyboxProgramId);

		//check result
		if (!result) {
			std::cerr << "Failed to create Skybox renderer program. Check console for errors." << std::endl;

			return;
		}

		//now get a link to the vertexPos3D so we can link the attribute
		//to our vertices when rendering
		vertexLocation = glGetAttribLocation(skyboxProgramId, "vertexPos3D");

		//check for errors
		if (vertexLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		
			return;
		}

		//link the uniforms to the member fields
		viewUniformId = glGetUniformLocation(skyboxProgramId, "view");
		projectionUniformId = glGetUniformLocation(skyboxProgramId, "projection");
		samplerId = glGetUniformLocation(skyboxProgramId, "sampler");
	}

	void Skybox::draw(Camera* cam) {
		bool isDepthTestEnable = glIsEnabled(GL_DEPTH_TEST);

		glDisable(GL_DEPTH_TEST);

		glm::mat4 camView = cam->getViewMatrix();
		glm::mat4 projection = cam->getProjectionMatrix();

		//zero out translation as just interested in rotation to make
		//sure the skybox is oriented around the origin
		camView[3][0] = 0.0f;
		camView[3][1] = 0.0f;
		camView[3][2] = 0.0f;

		//select the program into the rendering context
		glUseProgram(skyboxProgramId);

		glBindVertexArray(vao);

		//set the uniforms in the shader
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(camView));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projection));

		//select vertex buffer object into context
		glBindBuffer(GL_ARRAY_BUFFER, vboSkybox);
	
		//enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexLocation);

		//define the structure of a vertex for OpenGL to select values from vertex buffer & store in vertexLocation attribute.
		//Note, there is no attribute pointer for uvs because for a skybox the texture co-ordinates are taken from the vertex using a cube map
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)offsetof(CubeVertex, x));
	
		//select the texture
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMapName);

		//draw the model
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cube) / sizeof(CubeVertex));

		//unselect the attribute from the context
		glDisableVertexAttribArray(vertexLocation);

		//unselect the program from the context
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);

		if (isDepthTestEnable) {
			glEnable(GL_DEPTH_TEST);
		}
	}

	void Skybox::destroy() {
		glDeleteProgram(skyboxProgramId);

		glDeleteBuffers(1, &vboSkybox);

		glDeleteTextures(1, &skyboxCubeMapName);
	}
}