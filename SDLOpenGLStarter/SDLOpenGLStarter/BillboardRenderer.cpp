#include "BillboardRenderer.h"


namespace GE {


	// Billboard quad.  Quad is a square so use scale
	// to get correct dimensions.  Is on 0,0,0
	Vertex billboard[] = {
		Vertex(0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		Vertex(-0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),

		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f)
	};

	BillboardRenderer::BillboardRenderer(Camera* c)
	{
		pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;

		scale_x = scale_y = scale_z = 1.0f;

		cam = c;
	}

	void BillboardRenderer::init()
	{

		//create the vertex shader first.
		//order doesn't matter but shaders must be created and compiled before program attachment
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//the shader code as described in class
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec4 vertexPosition;\n"
			"in vec2 vUV;\n"
			"uniform mat4 transform;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"out vec2 uv;\n"
			"void main() {\n"
				"vec4 v = vec4(vertexPosition.xyz, 1);\n"
				"gl_Position = projection * view * transform * v;\n"
				"uv = vUV;\n"
			"};\n" };


		//do the same for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//the shader code as described in class
		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"in vec2 uv;\n"
			"uniform sampler2D sampler;\n"
			"out vec4 billboardFragmentColour;\n"
			"void main()\n"
			"{\n"
				"//texture\n"
				"vec3 textureColour = texture(sampler, uv).xyz;\n"
				"billboardFragmentColour = texture(sampler, uv).rgba;\n"
				"if(billboardFragmentColour.a == 0.0f)\n"
				"{\n"
					"discard;"
				"}\n"
			"};\n" };


		//source to OpenGL for compiling
		GLCALL(glShaderSource(vertexShader, 1, V_ShaderCode, NULL));
		GLCALL(glShaderSource(fragmentShader, 1, F_ShaderCode, NULL));

		if (!compileProgram(V_ShaderCode, F_ShaderCode, &programId)) {
			std::cerr << "Error compiling shader" << std::endl;
		}

		GLCALL(glUseProgram(programId));


		// Now get a link to the vertexPos3D so we can link the attribute
		// to our vertices when rendering
		vertexPos3DLocation = glGetAttribLocation(programId, "vertexPosition");

		// Now link to the vUV attribute
		vertexUVLocation = glGetAttribLocation(programId, "vUV");

		// Link the uniforms to the member fields
		transformUniformId = glGetUniformLocation(programId, "transform");
		viewUniformId = glGetUniformLocation(programId, "view");
		projectionUniformId = glGetUniformLocation(programId, "projection");
		samplerId = glGetUniformLocation(programId, "sampler");

		// Create the vertex buffer for the quad
		glGenBuffers(1, &vboTriangle);
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);

		// Transfer vertices to graphics memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(billboard), billboard, GL_STATIC_DRAW);

		glUseProgram(0);
	}

	void BillboardRenderer::draw()
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Calculate the transformation matrix for the object.  Start with the identity matrix
		// No rotation as going to look down the z axis
		glm::mat4 transformationMat = glm::mat4(1.0f);

		transformationMat = glm::translate(transformationMat, glm::vec3(getPosX(), getPosY(), getPosZ()));
		transformationMat = glm::rotate(transformationMat, glm::radians(270.0f - cam->getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMat = glm::scale(transformationMat, glm::vec3(getScaleX(), getScaleY(), 0.0f));

		// Render object in the same way as a regular model
		// Get the view and projection matrices
		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		// Select the program into the rendering context
		glUseProgram(programId);

		// Set the uniforms in the shader
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);

		// vertex pos
		glEnableVertexAttribArray(vertexPos3DLocation);
		glVertexAttribPointer(vertexPos3DLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		//uv location
		glEnableVertexAttribArray(vertexUVLocation);
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		//texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->GetTextureName());
		glUniform1i(samplerId, 0);

		// Draw the billboard
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Unselect the attribute from the context
		glDisableVertexAttribArray(vertexPos3DLocation);
		glDisableVertexAttribArray(vertexUVLocation);

		// Unselect the program from the context
		glUseProgram(0);

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}

}