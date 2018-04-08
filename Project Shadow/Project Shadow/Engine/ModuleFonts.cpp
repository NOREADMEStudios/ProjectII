#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"

#include "..\SDL\include\SDL.h"
#include "..\SDL_TTF\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

ModuleFonts::ModuleFonts() : Module()
{
	name="fonts";
}

// Destructor
ModuleFonts::~ModuleFonts()
{}

// Called before render is available
bool ModuleFonts::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if(TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		defaultFont = Load(path, size);		
	}

	return ret;
}

// Called before quitting
bool ModuleFonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");
	std::list<TTF_Font*>::iterator item;

	for(item = fonts.begin(); item != fonts.end(); item++)
	{
		TTF_CloseFont(*item);
	}

	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const ModuleFonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFont(path, size);

	if(font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.push_back(font);
	}

	return font;
}

bool ModuleFonts::Unload(_TTF_Font * font)
{
	bool ret = true;
	if (font != nullptr) {
		TTF_CloseFont(font);
		fonts.remove(font);
	}
	else ret = false;
	return ret;
}

// Print text using font
SDL_Texture* ModuleFonts::Print(const char* text, SDL_Color color, TTF_Font* font, Label::RenderMode render_mode, SDL_Color bg_color)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = nullptr;
	switch (render_mode) {
	case Label::RenderMode::SOLID:
		surface = TTF_RenderText_Solid((font) ? font : defaultFont, text, color);
		break;
	case Label::RenderMode::BLENDED:
		surface = TTF_RenderText_Blended((font) ? font : defaultFont, text, color);
		break;
	case Label::RenderMode::SHADED:
		surface = TTF_RenderText_Shaded((font) ? font : defaultFont, text, color, bg_color);
		break;
	default:
		surface = TTF_RenderText_Solid((font) ? font : defaultFont, text, color);
		break;
	}

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->textures->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool ModuleFonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if(TTF_SizeText((font) ? font : defaultFont, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}