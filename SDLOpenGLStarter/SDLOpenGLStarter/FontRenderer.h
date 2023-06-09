#pragma once
#include <string>
#include "Textures.h"
#include <SDL_ttf.h>

namespace GE {
	class FontRenderer
	{
	public:
		FontRenderer();
		~FontRenderer();
		void init();
		Textures* renderText(const std::string& text, const std::string& fontname, int size);
		void destroy();
	private:
		TTF_Font* m_font = nullptr;
	};
}