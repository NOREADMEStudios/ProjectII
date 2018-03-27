#ifndef __MOFULEFONTS_H__
#define __MODULEFONTS_H__

#include "Module.h"
#include "..\SDL\include\SDL_pixels.h"
#include <list>
#include "UI\Label.h"

#define DEFAULT_FONT "fonts/open_sans/OpenSans-Regular.ttf"
#define DEFAULT_FONT_SIZE 40

struct SDL_Texture;
struct _TTF_Font;

class ModuleFonts : public Module
{
public:

	ModuleFonts();

	// Destructor
	virtual ~ModuleFonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);
	bool Unload(_TTF_Font* font);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL, Label::RenderMode render_mode = Label::RenderMode::BLENDED, SDL_Color bg_color = { 0, 0, 0, 0 });

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	std::list<_TTF_Font*>	fonts;
	_TTF_Font*				defaultFont;
	_TTF_Font*				wow_font;
};


#endif // __MODULEFONTS_H__