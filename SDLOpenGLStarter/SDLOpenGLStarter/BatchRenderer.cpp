#include "BatchRenderer.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <SDL_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderUtils.h"

namespace GE {
	// A batch vertex is a new vertex struct which includes
	// a texture number to associate the vertex with a texture
	// so the correct texture is selected when rendering this
	// and other vertices in a batched model
	struct BatchVertex {
		float x, y, z, u, v;
		int texIdx;

		//possible future normals implementation for lighting?
		// 
		//constructor
		BatchVertex(float _x, float _y, float _z, float _u, float _v, int _texIdx) {
			//location
			x = _x;
			y = _y;
			z = _z;

			//colour
			u = _u;
			v = _v;

			//texture idx
			texIdx = _texIdx;
		}

		//set vertex to origin
		BatchVertex() {
			x = y = z = 0.f;
			u = v = 0.f;
			texIdx = 0;
		}

	};


	void BatchRenderer::init() {
		// New! Load shader source from files.  Need the new ShaderUtils files
		std::string v_shader_source = loadShaderSourceCode("batch.vert");
		std::string f_shader_source = loadShaderSourceCode("batch.frag");

		// Due to the unique way OpenGL handles shader source, OpenGL expects
		// an array of strings.  In this case, create an array of the
		// loaded source code strings and pass to compileProgram for compilation
		const GLchar* v_source_array[] = { v_shader_source.c_str() };
		const GLchar* f_source_array[] = { f_shader_source.c_str() };

		// Compile shaders into a program
		if (!compileProgram(v_source_array, f_source_array, &programId)) {
			std::cerr << "Problem building batch renderer program.  Check console log for more information." << std::endl;
		}

		// Now get a link to the vertexPos2D so we can link the attribute 
		// // to our vertices when rendering 
		vertexPos3DLocation = glGetAttribLocation(programId, "vertexPos3DLocation");
		// Check for errors
		if (vertexPos3DLocation == -1) {
			std::cerr << "Problem getting vertexPos3D" << std::endl;
		}


		vertexUVLocation = glGetAttribLocation(programId, "vUV");
		// Check for errors
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		vertexTextureId = glGetAttribLocation(programId, "vTextureId");
		// Check for errors
		if (vertexTextureId == -1) {
			std::cerr << "Problem getting vTextureId" << std::endl;
		}

		viewUniformId = glGetUniformLocation(programId, "viewMat");
		projectionUniformId = glGetUniformLocation(programId, "projMat");
		transformUniformId = glGetUniformLocation(programId, "transformMat");
		samplerId = glGetUniformLocation(programId, "textures");
	}

	bool BatchRenderer::addModelToBatch(const char* modelFilename, const char* textureFilename, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
	{
		// Calculate transformation matrix as model needs to be
		// translated before it is added to the batch
		glm::mat4 transform = glm::mat4(1.f);

		transform = glm::translate(transform, pos);
		transform = glm::rotate(transform, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, scale);


		// Temporary vector for storing model
		std::vector<BatchVertex> loadedVertices;

		// Load model data from file using assimp as usual
		Assimp::Importer imp;
		//Load mdoel
		const aiScene* pScene = imp.ReadFile(modelFilename, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);

		// Check file was opended correctly
		if (!pScene) {
			return false;
		}
		// Get the vertices from the mdoel object and put them in the temporary vector
		for (int MeshIdx = 0; MeshIdx < pScene->mNumMeshes; MeshIdx++) {
			const aiMesh* mesh = pScene->mMeshes[MeshIdx];

			for (int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++) {
				const aiFace& face = mesh->mFaces[faceIdx];

				// Extract vertices from a triangle
				for (int vertIdx = 0; vertIdx < 3; vertIdx++) {
					const aiVector3D* pos = &mesh->mVertices[face.mIndices[vertIdx]];

					const aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[vertIdx]];

					// Before putting them in the vector of vertices
					// Transform them into world space
					glm::vec4 vertex = glm::vec4(pos->x, pos->y, pos->z, 1.0f);

					vertex = transform * vertex;

					// create a new object in the shape array based on the extracted vertex
					loadedVertices.push_back(BatchVertex(vertex.x, vertex.y, vertex.z, uv.x, uv.y, numModels));
				}
			}
		}

		// Load associated texture and put in texture array
		GLuint textureName = loadTexture(textureFilename);

		if (textureName == -1) {
			std::cerr << "Error loading texture file" << textureFilename << std::endl;
			return false;
		}

		textures.push_back(textureName);

		// Enable a texture unit for the texture
		textureUnits.push_back(textures.size() - 1);

		//First time a model has been added to the batch
		if (vbo == -1) {

			// Create the vertex buffer
			totalVertices = loadedVertices.size();

			glGenBuffers(1, &vbo);

			//create the buffer
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, totalVertices * sizeof(BatchVertex), loadedVertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}


		// Otherwise add the model to the end of the batch
		else {
			// Create a new buffer big enough to store
			// the models already loaded plus the latest
			GLuint newVbo;

			glGenBuffers(1, &newVbo);
			glBindBuffer(GL_ARRAY_BUFFER, newVbo);

			// Make new buffer equal to the size of the old buffer + size of loaded model
			glBufferData(GL_ARRAY_BUFFER, (totalVertices + loadedVertices.size()) * sizeof(BatchVertex), nullptr, GL_STATIC_DRAW);

			// Copy loaded model after old data
			glBufferSubData(GL_ARRAY_BUFFER, totalVertices * sizeof(BatchVertex), loadedVertices.size() * sizeof(BatchVertex), loadedVertices.data());

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Bind which vertex buffer to copy from and store to
			glBindBuffer(GL_COPY_READ_BUFFER, vbo);
			glBindBuffer(GL_COPY_WRITE_BUFFER, newVbo);

			// Copy old data into new buffer
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, totalVertices * sizeof(BatchVertex));

			// Unbind source/destination buffers
			glBindBuffer(GL_COPY_READ_BUFFER, 0);
			glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

			// Delete old buffer
			glDeleteBuffers(1, &vbo);
			totalVertices += loadedVertices.size();
			vbo = newVbo;
		}
		numModels++;
	}

	void BatchRenderer::draw(Camera* cam)
	{
		glEnable(GL_CULL_FACE);

		// Select the program into the rendering context
		glUseProgram(programId);

		glm::mat4 transformationMat = glm::mat4(1.0f);

		glm::mat4 viewMat = cam->getViewMatrix(); //glm::lookAt(position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projectionMat = cam->getProjectionMatrix(); //glm::perspective(fov, aspectRatio, near, far);

		//send view and projection matrics to uniforms for transforming verticies into worl
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		// Select the vertex buffer object into the context
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexPos3DLocation);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer
		// and store in vertexPos2DLocation attribute
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void*)offsetof(BatchVertex, x));

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexUVLocation);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer
		// and store in vertexPos2DLocation attribute
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void*)offsetof(BatchVertex, u));

		// Enable the vertexTextureId attribute to allow texture to be selected for a vertex
		glEnableVertexAttribArray(vertexTextureId);

		// Note this uses glVertexAttribIPointer function instead of glVertexAttribPointer as glVertexAttribPointer
		// converts integer values to float values even though they are defined and stored as ints
		// This is a bit weird but it does say so on the glVertexAttribPointer doc page.
		glVertexAttribIPointer(vertexTextureId, 1, GL_INT, sizeof(BatchVertex), (void*)offsetof(BatchVertex, texIdx));

		// Populate texture units with textures
		// First, update the sampler array with the texture
		// units which will be used.  Texture units are
		// indicated by numbers 0, 1, 2 and so on
		
		GLint *units = textureUnits.data();

		glUniform1iv(samplerId, textureUnits.size(), units);

		// Now bind the textures to the texture unit
		int textureUnitNum = 0;
		for (GLuint textureName : textures) {
			// Active the texture unit GL_TEXTURE0 + number of texture unit
			glActiveTexture(GL_TEXTURE0 + textureUnitNum);

			// Bind texture
			glBindTexture(GL_TEXTURE_2D, textureName);

			textureUnitNum++;
		}

		// Draw the batch
		glDrawArrays(GL_TRIANGLES, 0, totalVertices);

		// Unselect the attribute from the context 
		glDisableVertexAttribArray(vertexUVLocation);
		glDisableVertexAttribArray(vertexPos3DLocation);

		// Disable texture id
		glDisableVertexAttribArray(vertexTextureId);

		// Unselect vertex buffer 
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Unselect the program from the context 
		glUseProgram(0);

		glDisable(GL_CULL_FACE);
	}

	GLuint BatchRenderer::loadTexture(const char* textureFilename) {
		// Load texture data from file
		SDL_Surface* surfaceImage = IMG_Load(textureFilename);

		// Check it was loaded okay
		if (surfaceImage == nullptr) {
			return -1;
		}

		// Get the dimensions, need for OpenGL
		int width = surfaceImage->w;
		int height = surfaceImage->h;

		// Get the format from the SDL object, will be in SDL Image's
		// format
		Uint32 format = surfaceImage->format->format;

		// Determine OpenGL format from SDL format
		switch (format) {
			// RGBA where there are 8 bits per pixel, 32 bits in total
		case SDL_PIXELFORMAT_RGBA32: format = GL_RGBA;
			break;

			// RGB where there are 8 bits per pixel, 24 bits in total
		case SDL_PIXELFORMAT_RGB24: format = GL_RGB;
			break;

			// Would have more formats here.  There are alternative
			// ways to achieve the same mapping.  Left as an exercise
			// to find out

			// Default is RGB but should really
			// use as many cases as expected texture data formats
		default:
			format = GL_RGB;
			break;
		}

		GLuint textureName;

		// Create a texture name for the texture
		glGenTextures(1, &textureName);

		// Select created texture for subsequent texture operations
		// to setup the texture for OpenGL
		glBindTexture(GL_TEXTURE_2D, textureName);

		// Copy the pixel data from the SDL_Surface object to the OpenGL texture
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surfaceImage->pixels);

		// Configure how the texture will be manipulated when it needs to be reduced or increased (magnified)
		// when rendering onto an object.  GL_LINEAR is weighted average of the colours around the texture co-ords
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Release the surface object and memory associated with it
		// as no longer need it
		SDL_FreeSurface(surfaceImage);

		return textureName;
	}


}
