#pragma once
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "Textures.h"

namespace GE {
	class FrameBuffer
	{
	public:
		FrameBuffer(int w, int h) {
			width = w;
			height = h;

			fboScene = 1;
			sceneTexture = nullptr;
			depthBufferName = 0;
		}

		~FrameBuffer() {
			// Release the texture, depth buffer and framebuffer objects
			delete sceneTexture;
			glDeleteRenderbuffers(1, &depthBufferName);
			glDeleteFramebuffers(1, &fboScene);
		}

		bool init();

		void beginRender() {
			// Bind the framebuffer to the pipeline
			// All objects rendered from this point go to the
			// framebuffer until glBindBuffer is used with a different framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, fboScene);
		}

		void endRender() {
			//Bind the default framebuffer.  No more objects
			// will be rendered to the framebuffer encapsulated in this class
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// Provides access to the texture object
		// associated with the framebuffer
		Textures* getTexture() {
			return sceneTexture;
		}

	private:
		GLuint fboScene, depthBufferName;

		Textures* sceneTexture;

		int width, height;
	};

}