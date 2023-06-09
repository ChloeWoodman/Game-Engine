#include "ModelRenderer.h"
#include <iostream>


namespace GE {

	//constructor
	ModelRenderer::ModelRenderer(Camera* cam) {
		//Initialise location, rotation and scale to default values
		pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;
		scale_x = scale_y = scale_z = 1.0f;
		
		this->cam = cam;
	}
	//deconstructor
	ModelRenderer::~ModelRenderer() {
	}

	//this is a helper function that allows us to see
	//shader compiler error messages should out shaders fail to compile
	void ModeldisplayShaderCompilerError(GLuint shaderId) {
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
	void ModelRenderer::init() {
		//create the vertex shader first.
		//order doesn't matter but shaders must be created and compiled before program attachment
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//the shader code as described in class
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"in vec3 vertexNormal;\n"
			"in vec2 vUV;\n"
			"out vec2 uv;\n"
			"out vec3 posW;\n"
			"out vec3 outNormal;\n"
			"uniform mat4 viewMat;\n"
			"uniform mat4 projMat;\n"
			"uniform mat4 transformMat;\n"
			"void main() {\n"
			"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"posW = vec3(viewMat * transformMat * v);\n"
			"outNormal = vec3(viewMat * transformMat * vec4(vertexNormal, 0.0f));\n"
			"gl_Position = projMat * viewMat * transformMat * v;\n"
			"uv = vUV;\n"
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

			ModeldisplayShaderCompilerError(vertexShader);

			return;
		}

		//do the same for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//the shader code as described in class
		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"in vec2 uv;\n"
			"in vec3 outNormal;\n"
			"in vec3 posW;\n"
			//light paremeters
			"uniform int enableLighting;\n"
			"uniform vec3 lightColour;\n"
			"uniform vec3 ambient;\n"
			"uniform vec3 lightPos;\n"
			"uniform float shininess;\n"
			//intensity of specular light
			"uniform float specularStrength;\n"
			"uniform sampler2D sampler;\n"
			"uniform vec3 viewPos;\n"
			"out vec4 fragmentColour;\n"
			"void main() {\n"
			"vec4 texColour = texture(sampler, uv).rgba;\n"
			"vec3 normalizedNormal = normalize(outNormal);\n"
			"if (enableLighting == 1) {\n"
			"vec3 lightDirection = normalize(lightPos - posW);\n"
			"float diffIllum = max(dot(normalizedNormal, lightDirection), 0.0f);\n"
			"vec3 diffuse = diffIllum * lightColour;\n"
			"vec3 viewDir = normalize(viewPos - posW);\n"
			"vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);\n"
			"float spec = pow(max(dot(viewDir, reflectDirection), 0.0f), shininess);\n"
			"vec3 specular = specularStrength * spec * lightColour;\n"
			"vec3 finalColour = (ambient + diffuse) * texColour.rgb + specular;\n"
			"fragmentColour = vec4(finalColour, texColour.a);\n"
			"} else {\n"
			"fragmentColour = texture(sampler, uv).rgba; } \n"
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

			ModeldisplayShaderCompilerError(fragmentShader);

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

		//Now link to the vUV attribute
		vertexUVLocation = glGetAttribLocation(programId, "vUV");

		//check for errors
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}


		vertexNormal = glGetAttribLocation(programId, "vertexNormal");
		//check for errors
		if (vertexNormal == -1) {
			std::cerr << "Problem getting vertexNormal" << std::endl;
		}

		//Get uniform id in shader so C++ program can send data to it
		viewUniformId = glGetUniformLocation(programId, "viewMat");
		projectionUniformId = glGetUniformLocation(programId, "projMat");
		samplerId = glGetUniformLocation(programId, "sampler");
		viewPosId = glGetUniformLocation(programId, "viewPos");
		lightColourId = glGetUniformLocation(programId, "lightColour");
		ambientId = glGetUniformLocation(programId, "ambient");
		lightPosId = glGetUniformLocation(programId, "lightPos");
		shininessId = glGetUniformLocation(programId, "shininess");
		specularStrengthId = glGetUniformLocation(programId, "specularStrength");
		// get the uniform location for enableLighting
		enableLightingUniformId = glGetUniformLocation(programId, "enableLighting");

		//Tranformation matrix uniform
		transformUniformId = glGetUniformLocation(programId, "transformMat");

		//create the vertex buffer object
		glGenBuffers(1, &vboTriangle);
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);

		//transfer vertices to graphics memory
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		//tidy up after setting up buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//update does nothing for this example
	void ModelRenderer::update() {

	}

	//draw renders the triangles from the buffer object
	void ModelRenderer::draw(Camera* cam, Model *model) {
		glEnable(GL_CULL_FACE);
		//glEnable(GL_BLEND);
		//glEnable(GL_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST);

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

		//yes lighting
		glUniform1i(enableLightingUniformId, 1);

		//Set the uniforms in the shader
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		//pass the camera posiition to fragment shader viewPos
		glUniform3f(viewPosId, cam->getPosX(), cam->getPosY(), cam->getPosZ());

		glUniform3f(lightColourId, 1.0f, 1.0f, 1.0f);
		glUniform3f(ambientId, 0.2f, 0.0f, 0.0f);
		glUniform3f(lightPosId, 0.0f, -25.0f, 0.0f);
		glUniform3f(shininessId, 64.0f, 64.0f, 64.0f);
		glUniform3f(specularStrengthId, 0.2f, 0.2f, 0.2f);

		//select the vertex buffer object into context
		glBindBuffer(GL_ARRAY_BUFFER, model->getVertices());

		//Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexPos3DLocation);
		//define the structure of a vertex from OpenGL to select values from vertex buffer
		// //and store in vertexPos3DLocation attribute
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		//enable the attribute to be passed colours from the vertex buffer location
		glEnableVertexAttribArray(vertexUVLocation);
		//and store in vColour attribute
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		glEnableVertexAttribArray(vertexNormal);
		
		//define where pipelin will find vertex normal in a vertex in vertex buffer
		//to place inside vertexNormal attribute
		glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));

		//select the texture
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, tex->GetTextureName());

		//draw the model
		glDrawArrays(GL_TRIANGLES, 0, model->getNumVertices());

		//unselect the attribute from the context
		glDisableVertexAttribArray(vertexPos3DLocation);
		glDisableVertexAttribArray(vertexUVLocation);
		glDisableVertexAttribArray(vertexNormal);

		//unselect vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//unselect the program from the context
		glUseProgram(0);

		glDisable(GL_CULL_FACE);
	}

	//draw terrain
	void ModelRenderer::drawTerrain(Terrain* terrain)
	{
		glUseProgram(programId);

		//glm::vec3 rotation(rot_x, 0, 0), translation(0, 0, 0), scale(1, 1, 1);

		// Create and manipulate the Model Matrix
		glm::mat4 modelMatrix = glm::mat4(1.0f); // Identity matrix
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(pos_x, pos_y, pos_z));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale_x, scale_y, scale_x));

		// set enableLighting to false
		glUniform1i(enableLightingUniformId, 0);

		// Set Uniforms/Attributes
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(cam->getProjectionMatrix()));

		// Vertex positions
		glBindBuffer(GL_ARRAY_BUFFER, terrain->getVbo());
		//
		glEnableVertexAttribArray(vertexPos3DLocation);
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		// UV coordinates
		glEnableVertexAttribArray(vertexUVLocation);
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));


		// Texture
		glBindTexture(GL_TEXTURE_2D, tex->GetTextureName());
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);

		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->getIbo());

		// Drawing Function
		glDrawElements(GL_TRIANGLES, terrain->getIndexCount(), GL_UNSIGNED_INT, nullptr);

		// Remember to clean up and deselect everything! 
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindTexture(GL_TEXTURE_2D, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		glUseProgram(NULL);
	}

	void ModelRenderer::drawSkyDome(SkyDome* sky)
	{
		glUseProgram(programId);

		glm::vec3 rotation(0, 0, 0), translation(0, 0, 0), scale(1, -1, 1);

		// Create and manipulate the Model Matrix
		glm::mat4 modelMatrix = glm::mat4(1.0f); // Identity matrix
		//modelMatrix = glm::rotate(modelMatrix, rotation.x * 180.0f / pi, glm::vec3(1.0f, 0.0f, 0.0f));
		//modelMatrix = glm::rotate(modelMatrix, rotation.y * 180.0f / pi, glm::vec3(0.0f, 1.0f, 0.0f));
		//modelMatrix = glm::rotate(modelMatrix, rotation.z * 180.0f / pi, glm::vec3(0.0f, 0.0f, 1.0f));
		//modelMatrix = glm::translate(modelMatrix, translation);
		modelMatrix = glm::scale(modelMatrix, scale);
		
		//no lighting
		glUniform1i(glGetUniformLocation(programId, "enableLighting"), 0);

		// Set Uniforms/Attributes
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(cam->getProjectionMatrix()));

		// Vertex positions
		glBindBuffer(GL_ARRAY_BUFFER, sky->getVbo());
		//
		glEnableVertexAttribArray(vertexPos3DLocation);
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		// UV coordinates
		glEnableVertexAttribArray(vertexUVLocation);
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));


		// Texture
		glBindTexture(GL_TEXTURE_2D, tex->GetTextureName());
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);

		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky->getIbo());

		// Drawing Function
		glDrawElements(GL_TRIANGLES, sky->getIndexCount(), GL_UNSIGNED_INT, nullptr);

		// Remember to clean up and deselect everything! 
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindTexture(GL_TEXTURE_2D, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		glUseProgram(NULL);
	}

	//releases objects allocated from program and vertex buffer object
	void ModelRenderer::destroy() {
		glDeleteProgram(programId);
		glDeleteBuffers(1, &vboTriangle);
	}

}
