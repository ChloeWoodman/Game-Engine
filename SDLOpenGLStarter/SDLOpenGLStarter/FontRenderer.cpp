#include "FontRenderer.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <iostream>

namespace GE {
	FontRenderer::FontRenderer() = default;

	FontRenderer::~FontRenderer() = default;

    void FontRenderer::init()
    {
        TTF_Init(); // initialize SDL_ttf library
    }

    Textures* FontRenderer::renderText(const std::string& text, const std::string& fontname, int font_size)
    {
        m_font = TTF_OpenFont(fontname.c_str(), font_size); 
        if (m_font == nullptr) {
            std::cerr << "Error creating font " << text << "! " << TTF_GetError() << std::endl;
            return nullptr; 
        }

        SDL_Color White = { 255, 255, 255 }; // create a white color
        SDL_Surface* surfaceImage = TTF_RenderText_Solid(m_font, text.c_str(), White); // render the text onto an SDL_Surface
        if (surfaceImage == nullptr) {
            std::cerr << "Error rendering text " << text << "! " << TTF_GetError() << std::endl; // print an error message if the text can't be rendered
            return nullptr; // return nullptr if the text can't be rendered
        }

        SDL_Surface* converted = SDL_ConvertSurfaceFormat(surfaceImage, SDL_PIXELFORMAT_RGBA32, 0); // convert the SDL_Surface to the format used by OpenGL
        if (converted != nullptr) {
            SDL_FreeSurface(surfaceImage); // free the original SDL_Surface
            surfaceImage = converted; // use the converted SDL_Surface instead
        }
        else {
            std::cerr << "Error converting font " << text << "! " << SDL_GetError() << std::endl; // print an error message if the font can't be converted
            return nullptr; // return nullptr if the font can't be converted
        }

        int width = surfaceImage->w; // get the width of the image
        int height = surfaceImage->h; // get the height of the image
        if (SDL_MUSTLOCK(surfaceImage)) {
            SDL_LockSurface(surfaceImage); // lock the SDL_Surface if necessary
        }

        int bytesPerRow = width * surfaceImage->format->BytesPerPixel; // calculate the number of bytes per row
        char* temp = new char[bytesPerRow]; // create a temporary buffer for swapping rows
        for (int y = 0; y < height / 2; y++) {
            memcpy(temp, ((char*)surfaceImage->pixels) + ((height - y - 1) * bytesPerRow), bytesPerRow); // copy the bottom row to the temporary buffer
            memcpy(((char*)surfaceImage->pixels) + ((height - y - 1) * bytesPerRow), (char*)surfaceImage->pixels + (y * bytesPerRow), bytesPerRow); // copy the top row to the bottom row
            memcpy((char*)surfaceImage->pixels + (y * bytesPerRow), temp, bytesPerRow); // copy the temporary buffer to the top row
        }
        delete[] temp; // free the temporary buffer

        if (SDL_MUSTLOCK(surfaceImage)) {
            SDL_UnlockSurface(surfaceImage); // unlock the SDL_Surface if necessary
        }

        // Define the pixel format for the texture as RGBA
        Uint32 format = GL_RGBA;

        // Generate a texture object and get its name
        GLuint textureName;
        glGenTextures(1, &textureName);

        // Bind the texture object to the 2D texture target
        glBindTexture(GL_TEXTURE_2D, textureName);

        // Define the texture image by specifying the pixel format, size, and data
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
            GL_UNSIGNED_BYTE, surfaceImage->pixels);

        // Set texture filtering options for when the texture is scaled
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Free the surface image since its data is already copied to the texture
        SDL_FreeSurface(surfaceImage);

        // Create a new Textures object using the texture name and return it
        Textures* texture = new Textures(textureName);
        return texture;

	}

	void FontRenderer::destroy()
	{
		TTF_Quit();
	}
}