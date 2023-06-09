#include "TriangleRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GE {
	
	Vertex vertexData[] = {
	Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),

	Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),

	// Right side face
	Vertex(0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),

	Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),

	//// Back face, note that points are in counter clockwise order
	Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),

	Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),

	//// Left side face, note that points are in counter clockwise order
	Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),

	Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),

	//// Top face
	Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),

	Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),

	//// Bottom face, note that points are in counter clockwise order
	Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),

	Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	};

	//constructor
	TriangleRenderer::TriangleRenderer() {
		//Initialise location, rotation and scale to default values
		pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;
		scale_x = scale_y = scale_z = 1.0f;
	}
	//deconstructor
	TriangleRenderer::~TriangleRenderer() {
	}

	//this is a helper function that allows us to see
	//shader compiler error messages should out shaders fail to compile
	void displayShaderCompilerError(GLuint shaderId) {
		//first get the length of error message string
		GLint MsgLen = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &MsgLen);

		//only display the message is more than 1 character was turned
		if (MsgLen > 1) {
			//create character buffer to store error message characters
			GLchar* Msg = new GLchar[MsgLen + 1];

			//get the error message so we can see what the problem
			//was with the shader
			std::cerr << "Error compliling shader" << Msg << std::endl;

			//release the memory allocated to the string
			delete[] Msg;
		}
	}

	//creates and compiles the shades, creates the project and links it
	//creates the vertex buffer object. Method may be not too cohesive
	void TriangleRenderer::init() {
		//create the vertex shader first.
		//order doesn't matter but shaders must be created and compiled before program attachment
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//the shader code as described in class
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"in vec4 vColour;\n"
			"out vec4 fColour;\n"
			"uniform mat4 viewMat;\n"
			"uniform mat4 projMat;\n"
			"uniform mat4 transformMat;\n"
			"void main() {\n"
			"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"gl_Position = projMat * viewMat * transformMat * v;\n"
			"fColour = vColour;\n"
			"}\n" };

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
			std::cerr << "Unable to compile vertex shader" << std::endl;

			displayShaderCompilerError(vertexShader);

			return;
		}
		
		//do the same for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//the shader code as described in class
		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"in vec4 fColour;\n"
			"out vec4 fragmentColour;\n"
			"void main() {\n"
			"fragmentColour = fColour;\n"
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
			std::cerr << "Unable to compile fragment shader" << std::endl;

			displayShaderCompilerError(fragmentShader);

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
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}

		vertexColourLocation = glGetAttribLocation(programId, "vColour");

		//check for errors
		if (vertexColourLocation == -1) {
			std::cerr << "Problem getting vColour" << std::endl;
		}

		//Get uniform id in shader so C++ program can send data to it
		viewUniformId = glGetUniformLocation(programId, "viewMat");
		projectionUniformId = glGetUniformLocation(programId, "projMat");
		//Tranformation matrix uniform
		transformUniformId = glGetUniformLocation(programId, "transformMat");

		//create the vertex buffer object
		glGenBuffers(1, &vboTriangle);
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);

		//transfer vertices to graphics memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		//tidy up after setting up buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//update does nothing for this example
	void TriangleRenderer::update() {
		
	}

	//draw renders the triangles from the buffer object
	void TriangleRenderer::draw(Camera* cam) {
		/*glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glEnable(GL_SRC_ALPHA);*/

		//transormations starts as identitiy matrix. This is initialisation
		glm::mat4 transformationMat = glm::mat4(1.0f);

		//Translate (move) vertices
		transformationMat = glm::translate(transformationMat, glm::vec3(pos_x, pos_y, pos_z));
		//rotate
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
		//size
		transformationMat = glm::scale(transformationMat, glm::vec3(scale_x, scale_y, scale_z));

		//get the view and projection matrices
		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		
		//select the program into the rendering context
		glUseProgram(programId);

		//Set the uniforms in the shader
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		

		//select the vertex buffer object into context
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);

		//Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexPos3DLocation);
		//define the structure of a vertex from OpenGL to select values from vertex buffer
		// //and store in vertexPos3DLocation attribute
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		//enable the attribute to be passed colours from the vertex buffer location
		glEnableVertexAttribArray(vertexColourLocation);
		//and store in vColour attribute
		glVertexAttribPointer(vertexColourLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
	
		//draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexData) / sizeof(Vertex) );

		//unselect the attribute from the context
		glDisableVertexAttribArray(vertexPos3DLocation);
		glDisableVertexAttribArray(vertexColourLocation);

		//unselect vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//unselect the program from the context
		glUseProgram(0);

		////Create view and projection matrices
		//float fov = 45.0f;
		//float aspectRatio = 640.0f / 480.0f;
		//float near = 0.1f;
		//float far = 10000.0f;
		//glm::vec3 position = glm::vec3(0.0f, 0.0f, +5.0f);
		//glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::mat4 viewMatrix = glm::lookAt(position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 projectionMatrix = glm::perspective(fov, aspectRatio, near, far);

		////send view and projection matrices to uniforms for transorming vertices into world
		//glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		//glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


		//Send transformation matrix to uniform for transforming vertices into world
		//glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	}

	//releases objects allocated from program and vertex buffer object
	void TriangleRenderer::destroy() {
		//glDeleteProgram(programId);
		//glDeleteBuffers(1, &vboTriangle);
	}

}