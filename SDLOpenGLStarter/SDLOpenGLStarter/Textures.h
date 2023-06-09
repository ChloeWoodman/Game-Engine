#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

namespace GE {
    class Textures {
    public:
        // Constructor that loads texture into OpenGL
        explicit Textures(const std::string& filename) {
            // Initialize fields
            width_ = 0;
            height_ = 0;
            texture_name_ = 0;

            // Load texture into OpenGL
            loadTexture(filename);
        }

        // Constructor for creating a texture object from an existing OpenGL texture object
        // Assumes certain properties of the texture object
        explicit Textures(GLuint name) {
            texture_name_ = name;

            glBindTexture(GL_TEXTURE_2D, texture_name_);

            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width_);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height_);

            glBindTexture(GL_TEXTURE_2D, 0);

            format_ = GL_RGBA;
        }

        ~Textures() {}

        // Accessor methods
        int GetWidth() const {
            return width_;
        }
        int GetHeight() const {
            return height_;
        }

        Uint32 GetFormat() const {
            return format_;
        }

        GLuint GetTextureName() const {
            return texture_name_;
        }


    private:
        // Load texture from file
        void loadTexture(const std::string& filename);

        int width_, height_;

        // Format
        Uint32 format_;

        // OpenGL name for texture object
        GLuint texture_name_;
    };
}
