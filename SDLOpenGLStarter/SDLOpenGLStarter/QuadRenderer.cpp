#include "QuadRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GE {
	
	Vertex vertData[] = {

		//x, y, z, u, v, nx, ny, nz;
		Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),

		Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),

		//// Back face, note that points are in counter clockwise order
		Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),

		Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	};

	// Class default constructor
	QuadRenderer::QuadRenderer(Camera* c) {
		pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;
		scale_x = scale_y = scale_z = 1.0f;

		cam = c;
	}

	// Class destructor
	QuadRenderer::~QuadRenderer() {

	}

	// Helper function that displays shader compiler error messages
	void displayShaderCompileError(GLuint shaderId) {
		// Get the length of the error message string
		GLint MsgLen = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &MsgLen);

		// if the message is longer than one character, then display it
		if (MsgLen > 1) {
			// Create a character buffer to store the error message chars
			GLchar* Msg = new GLchar[MsgLen + 1];
			// Retrieve error message from OpenGL internal log
			glGetShaderInfoLog(shaderId, MsgLen, NULL, Msg);
			// Display the error message
			std::cerr << "Error compiling shader: " << Msg << std::endl;
			// release memory allocated to the message string
			delete[] Msg;
		}
	}

	// Create and compiles the shaders, create the project and links it, 
	// create the VBO
	void QuadRenderer::init()
	{
		//create the vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//The shader code
		const GLchar* V_ShaderCode[] =
		{
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"in vec2 vUV;\n"
			"out vec2 uv;\n"
			"uniform mat4 transform;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main() {\n"
			"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"v = projection * view * transform * v;\n"
			"gl_Position = v;\n"
			"uv = vUV;\n"
			"}\n"
		};

		//copy the source to OpenGL ready to compile
		glShaderSource(vertexShader, 1, V_ShaderCode, NULL);

		//compile the code
		glCompileShader(vertexShader);

		//check compiler errors
		GLint isShaderCompiledOK = GL_FALSE;

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		//has the sahder failed to compile?
		if (isShaderCompiledOK != GL_TRUE)
		{
			std::cerr << "Unable to compile triangle vertex shader" << std::endl;

			displayShaderCompileError(vertexShader);

			return;
		}

		//doing the same for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//fragment shader source code
		const GLchar* F_ShaderCode[] =
		{
			"#version 140\n"
			"in vec2 uv;\n"
			"uniform sampler2D sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main()\n"
			"{\n"
			"fragmentColour = texture(sampler, uv).rgba;\n"
			"}\n"
		};

		//transfer shader code
		glShaderSource(fragmentShader, 1, F_ShaderCode, NULL);

		//compile it
		glCompileShader(fragmentShader);

		//check for errors
		isShaderCompiledOK = GL_FALSE;

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		if (isShaderCompiledOK != GL_TRUE)
		{
			std::cerr << "Unable to compile triangle fragment shader" << std::endl;

			displayShaderCompileError(fragmentShader);

			return;
		}

		//create the program object
		programId = glCreateProgram();

		//Attach shaders to the program object.
		glAttachShader(programId, vertexShader);
		glAttachShader(programId, fragmentShader);

		glLinkProgram(programId);

		GLint isProgramLinked = GL_FALSE;

		//checkign if program is linked
		glGetProgramiv(programId, GL_LINK_STATUS, &isProgramLinked);

		if (isProgramLinked != GL_TRUE)
		{
			std::cerr << "Unable to link program." << std::endl;
		}

		//linking the verticies when rendering
		vertexPos2DLocation = glGetAttribLocation(programId, "vertexPos3D");

		if (vertexPos2DLocation == -1)
		{
			std::cerr << "problem getting vertexPos3D" << std::endl;
		}

		//Link the uniforms to member fields
		transformUniformId = glGetUniformLocation(programId, "transform");
		viewUniformId = glGetUniformLocation(programId, "view");
		projectionUniformId = glGetUniformLocation(programId, "projection");

		//create the vertex buffer objects
		glGenBuffers(1, &vboQuad);
		glBindBuffer(GL_ARRAY_BUFFER, vboQuad);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertData), vertData, GL_STATIC_DRAW);

		vertexUVLocation = glGetAttribLocation(programId, "vUV");

		if (vertexUVLocation == -1)
		{
			std::cerr << "Problem getting vUV" << std::endl;
		}

		samplerId = glGetUniformLocation(programId, "sampler");
	}

	// Update is where any animation would occur for example
	void QuadRenderer::update() {

	}

	// Draws the quad from the buffer object
	void QuadRenderer::draw() {
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 transformationMat = glm::mat4(1.0f);

		transformationMat = glm::translate(transformationMat, glm::vec3(pos_x, pos_y, pos_z));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMat = glm::scale(transformationMat, glm::vec3(scale_x, scale_y, scale_z));

		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		glUseProgram(programId);

		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		glBindBuffer(GL_ARRAY_BUFFER, vboQuad);

		glEnableVertexAttribArray(vertexPos2DLocation);
		glVertexAttribPointer(vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));


		glEnableVertexAttribArray(vertexUVLocation);
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, quadtex->GetTextureName());

		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertData) / sizeof(Vertex));

		glDisableVertexAttribArray(vertexPos2DLocation);
		glDisableVertexAttribArray(vertexUVLocation);
		glUseProgram(0);

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}

	// Release objects allocated memory, program and VBO
	void QuadRenderer::destroy() {
		glDeleteProgram(programId);
		glDeleteBuffers(1, &vboQuad);
	}
}