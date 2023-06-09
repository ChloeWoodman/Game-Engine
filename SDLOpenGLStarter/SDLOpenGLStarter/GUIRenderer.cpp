#include "GUIRenderer.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include "ShaderUtils.h"
#include "Vertex.h"
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GE {

	// Constructor
	GUIRenderer::GUIRenderer() {
		programId = 0;
		vertexLocation = 0;
		vertexUVLocation = 0;
		projectionUniformId = 0;
		samplerId = 0;
		projectionMat = glm::mat4(1.0f);
	}

	// Initialise
	void GUIRenderer::init(int window_width, int window_height)
	{
		
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//the shader code as described in class
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec4 vertexPos3D;\n"
			"in vec2 vUV;\n"
			"uniform mat4 projection;\n"
			"out vec2 uv;\n"
			"void main() {\n"
				"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
				"gl_Position = projection * v;\n"
				"uv = vUV;\n"
			"};\n" };
	

		//copy the source to OpenGL ready for compilation
		glShaderSource(vertexShader, 1, V_ShaderCode, NULL);

		//compile the code
		glCompileShader(vertexShader);

		//Check for compiler errors
		//Presume shader didn't compile
		GLint isShaderCompiledOK = GL_FALSE;

		//Get the compile status from OpenGL
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		//has the shader failed to compile?
		if (isShaderCompiledOK != GL_TRUE) {
			//yes, so display error message
			std::cerr << "Unable to compile GUI vertex shader" << std::endl;
			return;
		}

		//do the same for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//the shader code as described in class
		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"in vec2 uv;\n"
			"uniform sampler2D sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main() {\n"
			"fragmentColour = texture(sampler, uv).rgba;\n"
			"}\n" };

		//transfer the shader code
		glShaderSource(fragmentShader, 1, F_ShaderCode, NULL);

		//compile the code
		glCompileShader(fragmentShader);

		//Check for compiler errors
		//Presume shader didn't compile
		isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		//has the shader failed to compile?
		if (isShaderCompiledOK != GL_TRUE) {
			//yes, so display error message
			std::cerr << "Unable to compile GUI fragment shader" << std::endl;

			return;
		}

		programId = glCreateProgram();

		//attach shaders to the program object
		glAttachShader(programId, vertexShader);
		glAttachShader(programId, fragmentShader);

		//now link the program to create an executable program we
		//and use to render the object
		//program executable will exist in graphics memory
		glLinkProgram(programId);


		//check for linking errors
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE) {
			std::cerr << "Failed to link program" << std::endl;
		}

		//now get a link to the vertexPos3D so we can link the attribute to our vertcies when rendering
		vertexPos3DLocation = glGetAttribLocation(programId, "vertexPos3D");

		//check for errors
		if (vertexPos3DLocation == -1) {
			std::cerr << "Problem getting vertexPos3D" << std::endl;
		}


		// Now link to the vUV attribute
		vertexUVLocation = glGetAttribLocation(programId, "vUV");

		// Check for errors
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		// Link the uniforms to the member fields
		projectionUniformId = glGetUniformLocation(programId, "projection");
		samplerId = glGetUniformLocation(programId, "sampler");

		// Difference: Create orthographic camera
		projectionMat = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f);
	}

	// Main rendering function which draws a texture to GUI
	// All GUI objects must be a texture
	void GUIRenderer::drawTexture(int x, int y, Textures* text, float scaleX, float scaleY)
	{
		// Get the width and height of the text in pixels
		int texW = scaleX;
		int texH = scaleY;

		// A quad is created on the fly for the text in screen co-ordinates, not
		// A quad is created on the fly for the text in screen co-ordinates, not
		// world co-ordinates
		GLuint vboTextQuad;

		Vertex textQuad[] = {
			Vertex(x, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			Vertex(x, y + texH, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			Vertex(x + texW, y + texH, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),

			Vertex(x + texW, y + texH, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			Vertex(x + texW, y, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			Vertex(x, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		};

		// Create the vertex buffer for the quad
		glGenBuffers(1, &vboTextQuad);
		glBindBuffer(GL_ARRAY_BUFFER, vboTextQuad);

		// Transfer vertices to graphics memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(textQuad), textQuad, GL_STATIC_DRAW);

		// Render text
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Setup pipeline for quad
		// Select the program into the rendering context
		glUseProgram(programId);

		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexLocation);
		// Select the vertex buffer object into the context
		glBindBuffer(GL_ARRAY_BUFFER, vboTextQuad);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer
		// and store in vertexPos2DLocation attribute
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexUVLocation);
		// Define where the vertex specification will find the colour data and how much
		// Colour data is four float values, located at where the r member is.  Stride is a vertex apart
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, text->GetTextureName());

		// Draw the model
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Tidy up pipeline
		glBindTexture(GL_TEXTURE_2D, 0);

		// Unselect the attribute from the context
		glDisableVertexAttribArray(vertexLocation);
		glDisableVertexAttribArray(vertexUVLocation);

		// Unselect the program from the context
		glUseProgram(0);

		// Release the vertex buffer
		glDeleteBuffers(1, &vboTextQuad);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}

	void GUIRenderer::destroy()
	{
		glDeleteProgram(programId);
	}
}
