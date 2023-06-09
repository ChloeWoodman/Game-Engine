#include "Textures.h"

namespace GE {
    void Textures::loadTexture(const std::string& filename)
    {
        SDL_Surface* surfaceImage = IMG_Load(filename.c_str());
        if (surfaceImage == nullptr) {
            std::cerr << "Texture loading failed for " << filename << "\n";
            return;
        }

        width_ = surfaceImage->w;
        height_ = surfaceImage->h;

        format_ = surfaceImage->format->format;

        switch (format_) {
        case SDL_PIXELFORMAT_RGBA32: format_ = GL_RGBA;
            break;

        case SDL_PIXELFORMAT_RGB24: format_ = GL_RGB;
            break;

        default:
            format_ = GL_RGB;
            break;
        }

        glGenTextures(1, &texture_name_);

        glBindTexture(GL_TEXTURE_2D, texture_name_);

        glTexImage2D(GL_TEXTURE_2D, 0, format_, width_, height_, 0, format_,
            GL_UNSIGNED_BYTE, surfaceImage->pixels);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //x
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //y
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // U
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // V
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // R

        SDL_FreeSurface(surfaceImage);
    }
}